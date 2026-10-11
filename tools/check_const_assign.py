"""Find members written inside a method declared const.

    /work/.venv/bin/python tools/check_const_assign.py

## Why

src/Keith.cpp failed to compile on both platforms with:

    mResourceRoot = path.Get();
    error: no viable overloaded '=' -- 'this' has type 'const std::string'

SampleDir() is declared const, so every member it touches is const inside it, and the
cache it writes cannot be assigned. The fix is `mutable` on the member -- or a non-const
method, which is usually wrong.

That is a mechanical, findable fault, and it cost two CI runs. This finds the class of it
rather than that one instance: scan a const method's body for assignments to members that
are not declared mutable.

## What it does not do

It does not parse C++. It reads member declarations from the class body and assignment
targets from method bodies by pattern. A member assigned through a pointer, a reference,
or a helper called from the const method is not seen, and neither is one assigned in a
lambda. It is a smell detector aimed at a fault that has already shipped twice; the
compiler remains the authority.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SOURCES = [ROOT / "src" / "Keith.h", ROOT / "src" / "Keith.cpp"]


def members_of(header_text):
    """Member name -> is it mutable. Read from the class body, one line each."""
    out = {}
    for m in re.finditer(r"^[ \t]+(mutable\s+)?[\w:<>,\s\*&]+?\b(m[A-Z]\w*)\s*(?:\{[^}]*\}|=[^;]*)?;",
                         header_text, re.M):
        out[m.group(2)] = bool(m.group(1))
    return out


def const_methods(text):
    """(name, body) for each method defined with a trailing const."""
    for m in re.finditer(r"\b(\w+)\s*\([^;{]*\)\s*const\s*(?:noexcept\s*)?\{", text):
        name = m.group(1)
        # brace-match the body
        depth = 0
        i = m.end() - 1
        while i < len(text):
            if text[i] == "{":
                depth += 1
            elif text[i] == "}":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        yield name, text[m.end():i]


def main():
    header = SOURCES[0].read_text()
    members = members_of(header)
    impl = SOURCES[1].read_text()

    problems = []
    checked = 0
    for name, body in const_methods(impl):
        # Only bodies that name a Keith member; a const helper elsewhere is not our business.
        touched = {m for m in members if re.search(r"\b" + m + r"\b", body)}
        if not touched:
            continue
        checked += 1
        for m in sorted(touched):
            if members[m]:
                continue   # declared mutable, so writing it is intended
            if re.search(r"\b" + m + r"\s*(?:=(?!=)|\+=|-=|\*=|/=)", body):
                problems.append((name, m))

    print(f"  const methods touching members: {checked}")
    if not problems:
        print("  ok    no non-mutable member is written inside a const method")
        return 0

    for method, member in problems:
        print(f"  FAIL  {method}() is const but writes {member}")
    print()
    print("  Either declare the member mutable, or the write does not belong in a const")
    print("  method. Do not fix it by dropping the const: callers pass a const Keith, and")
    print("  dropping it moves the failure to the call site instead of removing it.")
    return 1


if __name__ == "__main__":
    sys.exit(main())
