"""Check that the identity a host sees is the identity we compiled.

Three things have to agree or the AudioUnit cannot find its own bundle and dies:

  config.h              -> PLUG_MFR_ID, PLUG_UNIQUE_ID, BUNDLE_DOMAIN, BUNDLE_MFR,
                           BUNDLE_NAME (the last three build BUNDLE_ID at compile time)
  resources/*-Info.plist -> CFBundleIdentifier for the format, plus the AudioComponents entry
  the built binary       -> the BUNDLE_ID string, compiled in from config.h

They agreed by hand once and then drifted, because a config.h edit does not reliably
recompile the plugin object for the AU target. That is the failure this catches. If the
object is stale, delete build/build/*-au.build/Release/Objects-normal/*/Keith.o.

Usage: python3 tools/verify_plugin_identity.py
"""
import plistlib
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config.h"
OUT = ROOT / "build" / "out" / "Release"

failures = []
checks = 0


def check(ok, label, detail=""):
    global checks
    checks += 1
    if ok:
        print(f"PASS {label}")
    else:
        print(f"FAIL {label} {detail}")
        failures.append(label)


def parse_config():
    text = CONFIG.read_text()
    values = {}
    for key in ("PLUG_NAME", "PLUG_MFR", "BUNDLE_DOMAIN", "BUNDLE_MFR", "BUNDLE_NAME"):
        match = re.search(rf'^#define\s+{key}\s+"([^"]*)"', text, re.M)
        if not match:
            sys.exit(f"config.h has no {key}")
        values[key] = match.group(1)
    for key in ("PLUG_UNIQUE_ID", "PLUG_MFR_ID"):
        match = re.search(rf"^#define\s+{key}\s+'([^']*)'", text, re.M)
        if not match:
            sys.exit(f"config.h has no {key}")
        values[key] = match.group(1)
    return values


def bundle_identifier(cfg, api_ext):
    # IPlug_include_in_plug_hdr.h: BUNDLE_ID = DOMAIN.MFR.API_EXT.NAME
    return f"{cfg['BUNDLE_DOMAIN']}.{cfg['BUNDLE_MFR']}.{api_ext}.{cfg['BUNDLE_NAME']}"


def main():
    cfg = parse_config()

    formats = [
        ("AU", "audiounit", OUT / "Keith.component" / "Contents" / "Info.plist",
         OUT / "Keith.component" / "Contents" / "MacOS" / "Keith"),
        ("VST3", "vst3", OUT / "Keith.vst3" / "Contents" / "Info.plist",
         OUT / "Keith.vst3" / "Contents" / "MacOS" / "Keith"),
    ]

    for label, api_ext, plist_path, binary_path in formats:
        if not plist_path.exists():
            print(f"SKIP {label} not built at {plist_path}")
            continue

        expected = bundle_identifier(cfg, api_ext)
        with plist_path.open("rb") as handle:
            plist = plistlib.load(handle)

        check(plist.get("CFBundleIdentifier") == expected,
              f"{label} CFBundleIdentifier matches config.h", 
              f"plist={plist.get('CFBundleIdentifier')} config={expected}")
        check(plist.get("CFBundleName") == cfg["BUNDLE_NAME"],
              f"{label} CFBundleName is {cfg['BUNDLE_NAME']}",
              f"got {plist.get('CFBundleName')}")

        if binary_path.exists():
            strings = subprocess.run(["strings", "-a", str(binary_path)],
                                     capture_output=True, text=True).stdout
            check(expected in strings,
                  f"{label} binary carries the compiled BUNDLE_ID",
                  "stale object - delete build/build/*-{au,vst3}.build/Release/Objects-normal/*/Keith.o and rebuild")

    au_plist = OUT / "Keith.component" / "Contents" / "Info.plist"
    if au_plist.exists():
        with au_plist.open("rb") as handle:
            components = plistlib.load(handle).get("AudioComponents") or []
        check(len(components) == 1, "AU publishes exactly one AudioComponent",
              f"got {len(components)}")
        if components:
            entry = components[0]
            check(entry.get("manufacturer") == cfg["PLUG_MFR_ID"],
                  f"AU manufacturer is {cfg['PLUG_MFR_ID']}", f"got {entry.get('manufacturer')}")
            check(entry.get("subtype") == cfg["PLUG_UNIQUE_ID"],
                  f"AU subtype is {cfg['PLUG_UNIQUE_ID']}", f"got {entry.get('subtype')}")
            check(entry.get("name") == f"{cfg['PLUG_MFR']}: {cfg['PLUG_NAME']}",
                  f"AU name is '{cfg['PLUG_MFR']}: {cfg['PLUG_NAME']}'", f"got {entry.get('name')}")

    print(f"\nverify_plugin_identity: {checks - len(failures)} passed, {len(failures)} failed "
          f"(total {checks})")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
