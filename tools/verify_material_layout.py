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


def main():
    receipt = json.loads((IMAGES / 'physical-layout.json').read_text())
    assert png_size(IMAGES / 'chassis.png') == (2560, 1600)
    source = (ROOT / 'src/editor/ControlLayout.h').read_text()
    declared = {}
    for parameter, page, numbers in re.findall(r'\{(kParam\w+), "(\w+)", ([^}]+)\}', source):
        l, t, r, b, maximum = [float(n.strip().removesuffix('f')) for n in numbers.split(',')]
        height = b - t - 39
        assert height > 0 and r > l, parameter
        declared[parameter] = (page, (l+r)/2, t+height/2, min(min(r-l, height)*0.4, maximum))
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
    assert len(rendered) == len(declared) == 27
    assert len({c['parameter'] for c in rendered}) == len(rendered)
    for control in rendered:
        page, cx, cy, radius = declared[control['parameter']]
        assert page == control['page']
        assert abs(cx-control['cx']) < 0.00001
        assert abs(cy-control['cy']) < 0.00001
        assert abs(radius-control['radius']) < 0.00001
    assert receipt['material_coordinates'] == 'Geometry.Position world space'
    assert receipt['lighting'] == 'single upper-left softbox plus nondirectional world fill'
    print('PASS 27 native/render control placements agree')
    print('PASS fixed world-space material coordinates and shared lighting receipt')


if __name__ == '__main__':
    main()
