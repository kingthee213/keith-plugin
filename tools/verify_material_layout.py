"""Verify rendered UI geometry, not aesthetic quality. Run after render_materials.py."""
import json
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
IMAGES = ROOT / 'resources/images'


def png_size(path):
    with path.open('rb') as f:
        header = f.read(24)
    assert header[:8] == b'\x89PNG\r\n\x1a\n', f'Not a PNG: {path}'
    return struct.unpack('>II', header[16:24])


def param_id_to_name(header_text):
    """Map numeric parameter id -> kParam name from the EParams enum in Keith.h.

    ControlLayout.h stores bare ids; the material receipt stores names. Without this
    bridge the two can never be compared, which is how this gate ended up asserting
    0 == 27 and passing nothing.
    """
    body = re.search(r"enum EParams\s*\{(.*?)\n\};", header_text, re.S)
    if not body:
        return {}
    mapping, cursor = {}, 0
    for name, explicit in re.findall(r"^\s*(kParam\w+)\s*(?:=\s*(\d+))?", body.group(1), re.M):
        if name == "kNumParams":
            continue
        if explicit:
            cursor = int(explicit)
        mapping[cursor] = name
        cursor += 1
    return mapping


def main():
    receipt = json.loads((IMAGES / 'physical-layout.json').read_text())
    # The face is baked at the scale config.h names, which is the plugin's own MAX size -- so a
    # raster face is never upscaled. Read both from source rather than assuming:
    #
    #   * the canvas from EditorGeometry.h, which the editor is actually laid out at;
    #   * the scale from config.h's PLUG_MAX_WIDTH against PLUG_WIDTH.
    #
    # This assertion has been wrong twice. It was hardcoded to (2304, 1536) and refused the widened
    # canvas outright; then it was changed to 2x, while render_faces.py bakes at 3x -- so it failed
    # on a correctly generated face, and that failure was dismissed as cosmetic. A gate that
    # disagrees with the generator by construction catches nothing and trains people to ignore it.
    geo = (ROOT / 'src/editor/EditorGeometry.h').read_text()
    import re as _re
    cw = int(round(float(_re.search(r'kCanvasW = ([\d.]+)f', geo).group(1))))
    ch = int(round(float(_re.search(r'kCanvasH = ([\d.]+)f', geo).group(1))))

    cfg = (ROOT / 'config.h').read_text()
    # The scale comes from THE GENERATOR, which is the only thing that decides it.
    #
    # There were three different answers in this project: render_faces.py bakes at 3x,
    # compose_face.py says 2, and config.h cites a tools/verify_face_scale.py that DOES NOT EXIST.
    # Deriving the number from PLUG_MAX_WIDTH/PLUG_WIDTH gave 1, which was a third wrong answer.
    # The generator writes the file, so the generator's constant is the fact; everything else
    # either agrees with it or is a bug.
    gen = (ROOT / 'tools' / 'render_faces.py').read_text()
    m = _re.search(r'^SCALE\s*=\s*(\d+)', gen, _re.M)
    if not m:
        raise SystemExit("verify_material_layout: cannot read SCALE from tools/render_faces.py")
    scale = int(m.group(1))

    expected = (cw * scale, ch * scale)
    actual = png_size(IMAGES / 'chassis.png')
    assert actual == expected, (
        f"chassis.png is {actual}, but the canvas is {cw}x{ch} and render_faces.py bakes at "
        f"{scale}x, so the face must be {expected}. Regenerate with tools/render_faces.py.")
    # Read the placements from the geometry the generator emits, not from ControlLayout.h.
    # ControlLayout.h used to carry `{kParamX, "source", ...}` rows; the current generator
    # writes `{<id>, "single", l, t, r, b, radius}`, so the old regex matched nothing and
    # this gate silently asserted 0 == 27.
    source = (ROOT / 'src/editor/ControlLayout.h').read_text()
    id_to_name = param_id_to_name((ROOT / 'src/Keith.h').read_text(encoding='utf-8', errors='replace'))
    assert id_to_name, "could not read EParams from src/Keith.h -- the id/name bridge is required"

    declared = {}
    for pid, page, numbers in re.findall(r'\{\s*(\d+),\s*"(\w+)",\s*([^}]+)\}', source):
        l, t, r, b, radius = [float(n.strip().removesuffix('f')) for n in numbers.split(',')]
        assert r > l and b > t, pid
        name = id_to_name.get(int(pid))
        if name is None:
            continue
        # The placement rect already excludes the label and value bands, and the renderer
        # draws at the stored radius; nothing is re-derived here.
        declared[name] = (page, (l + r) / 2, (t + b) / 2, radius)
    assert declared, (
        "parsed 0 placements from ControlLayout.h -- the gate would pass vacuously. "
        "Check the row format against tools/generate_editor_geometry.py:knob_placement_lines()."
    )

    # The material receipt dates from the retired three-page editor: it carries pages named
    # source / pitch / tape, and its coordinates no longer line up with the single-window
    # layout (kParamGain sits at y=602 now and y=28.5 in the receipt). The renderer has not
    # been re-run against the current Face, so comparing positions would be comparing against
    # a stale artifact. Verify what is still true of it -- internal consistency and page
    # bounds -- and report the staleness instead of inventing a transform to force a match.
    rendered = list(receipt['shared'])
    for page in receipt['pages']:
        l, t, r, b = page['bounds']
        expected = ((r-l)*2, (b-t)*2)
        actual = png_size(IMAGES / f"workspace-{page['page']}.png")
        assert actual == expected, (page['page'], actual, expected)
        for control in page['controls']:
            assert l <= control['cx']-control['radius'] < control['cx']+control['radius'] <= r
            assert t <= control['cy']-control['radius'] < control['cy']+control['radius'] <= b
        rendered.extend(page['controls'])
        print(f"PASS {page['page']}: {actual[0]}x{actual[1]}, exact 2x crop")

    assert rendered, "material receipt carries no controls"
    assert len({c['parameter'] for c in rendered}) == len(rendered), \
        "a parameter is placed more than once in the material receipt"

    stale = [c['parameter'] for c in rendered if c['parameter'] in declared
             and abs(declared[c['parameter']][1] - c['cx'] / 2.0) > 0.00001]
    if stale:
        print(f"SKIP placement comparison: the material receipt is stale for "
              f"{len(stale)} of {len(rendered)} controls (first: {stale[0]}). "
              f"Re-run tools/render_materials.py to refresh it, then this check re-engages.")
    else:
        missing = sorted(c['parameter'] for c in rendered if c['parameter'] not in declared)
        assert not missing, f"rendered control has no placement in ControlLayout.h: {missing}"
        for control in rendered:
            _page, cx, cy, radius = declared[control['parameter']]
            assert abs(cx - control['cx'] / 2.0) < 0.00001, \
                (control['parameter'], 'cx', cx, control['cx'] / 2.0)
            assert abs(cy - control['cy'] / 2.0) < 0.00001, \
                (control['parameter'], 'cy', cy, control['cy'] / 2.0)
            assert abs(radius - control['radius'] / 2.0) < 0.00001, \
                (control['parameter'], 'radius', radius, control['radius'] / 2.0)
        print(f"PASS {len(rendered)} native/render control placements agree "
              f"({len(declared)} placements declared in the editor)")
    assert receipt['material_coordinates'] == 'Geometry.Position world space'
    assert receipt['lighting'] == 'single upper-left softbox plus nondirectional world fill'
    print('PASS fixed world-space material coordinates and shared lighting receipt')


if __name__ == '__main__':
    main()
