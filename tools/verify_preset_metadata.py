#!/usr/bin/env python3
"""Gate the neutral preset surface against internal emulated-family provenance."""
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parent.parent
source = (ROOT / 'src' / 'Keith.cpp').read_text(encoding='utf-8')
body = re.search(r'kPresetSpecs\[kNumPresets\]\s*=\s*\{(.*?)\n\};', source, re.S)
if not body:
    print('FAIL preset table not found')
    raise SystemExit(1)
rows = re.findall(r'\{\s*"([^"]+)"\s*,\s*(\d+)\s*,\s*"([^"]+)"\s*,\s*"([^"]+)"\s*\}', body.group(1))
expected_refs = ['Triton', 'Triton Extreme', 'Korg M1', 'Korg Wavestation', 'Korg TR']
expected_families = list(range(1, 6))
if len(rows) != 15:
    print(f'FAIL expected 15 metadata rows, found {len(rows)}')
    raise SystemExit(1)
for family, reference in zip(expected_families, expected_refs):
    selected = [row for row in rows if int(row[1]) == family]
    assert selected, f'Family {family} has no presets'
    assert all(row[2] == reference for row in selected), (family, selected)
    assert rows.index(selected[0]) == sum(len([r for r in rows if int(r[1]) == f]) for f in expected_families if f < family)
for name, family, reference, category in rows:
    assert re.match(r'^Family 0[1-5] / \d{3} ', name), name
    assert category and reference
print('PASS 15 presets grouped by emulated family before neutral sound category')
print('PASS internal order: ' + ' -> '.join(expected_refs))
