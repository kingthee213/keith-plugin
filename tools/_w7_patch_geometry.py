#!/usr/bin/env python3
from pathlib import Path
path = Path("tools/generate_editor_geometry.py")
text = path.read_text()
start = text.index("# OSC")
end = text.index("for p in range(111):")
placement = open("tools/_w7_placement_snippet.py").read() if Path("tools/_w7_placement_snippet.py").exists() else ""
