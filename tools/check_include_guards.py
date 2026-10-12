"""Find a header with no include guard that is included more than once.

    /work/.venv/bin/python tools/check_include_guards.py

## Why

Three headers added for the sampler and wavetable pages had NO include guard at all:

    KeithWaveView.h, KeithSampleBrowser.h, KeithWavetableFrames.h

and each was reached TWICE from the same translation unit:

    EditorialLayout.h   #include "KeithWaveView.h"       <- directly
    EditorialLayout.h   #include "KeithWaveView.cpp.h"   <- which includes it again

A header with no guard that is included twice in one translation unit defines its classes
twice. The compiler either rejects it outright or keeps the first definition and drops the
rest -- and the second case produces a build that succeeds while some of the header's
content is silently absent, which is far worse to diagnose.

This is the shape of bug that is invisible in review and obvious in hindsight.

## What it checks

For every header under src/:

  1. does it carry `#pragma once` or a conventional `#ifndef` guard?
  2. if it has no guard, is it reachable more than once from a single root? This is checked
     by walking `#include "..."` transitively and counting how many distinct paths reach it.

A header with no guard reached by only ONE path is reported as a warning, not a failure --
it is fragile, not yet broken.

## What it does not do

It does not parse C++ and it does not follow system includes. It resolves quoted includes
relative to the including file's directory and to src/, which covers how this project
includes its own headers.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"


def includes_of(path):
    """Quoted includes, in order, from a file."""
    text = path.read_text(errors="replace")
    return re.findall(r'#\s*include\s+"([^"]+)"', text)


def has_guard(path):
    text = path.read_text(errors="replace")
    head = text.lstrip()
    if head.startswith("#pragma once"):
        return True
    # A conventional guard: #ifndef NAME / #define NAME in the first few lines.
    first = "\n".join(text.splitlines()[:6])
    m = re.search(r"#\s*ifndef\s+(\w+)", first)
    if m and re.search(r"#\s*define\s+" + m.group(1) + r"\b", text):
        return True
    return False


def resolve(name, from_dir):
    """Resolve a quoted include to a real file, or None."""
    for base in (from_dir, SRC, ROOT):
        cand = (base / name).resolve()
        if cand.is_file():
            return cand
    return None


def root_of(path):
    """The top-level include chain a header belongs to: the first src file that includes it."""
    return path.name


def reach_counts(entry):
    """How many distinct include paths reach each file, starting from `entry`."""
    seen_paths = {}
    stack = [(entry.resolve(), (entry.name,))]
    while stack:
        cur, chain = stack.pop()
        if cur in seen_paths:
            seen_paths[cur] += 1
        else:
            seen_paths[cur] = 1
        if len(chain) > 24:      # guard against a cycle
            continue
        for inc in includes_of(cur):
            nxt = resolve(inc, cur.parent)
            if nxt and nxt.is_file() and nxt not in chain:
                stack.append((nxt, chain + (nxt.name,)))
    return seen_paths


def main():
    headers = sorted(SRC.rglob("*.h"))
    unguarded = [h for h in headers if not has_guard(h)]
    guarded = len(headers) - len(unguarded)

    print(f"  headers checked : {len(headers)}")
    print(f"  guarded         : {guarded}")
    print(f"  unguarded       : {len(unguarded)}")

    if not unguarded:
        print("  ok    every header carries a guard")
        return 0

    # For each unguarded header, is it reachable more than once from the translation units
    # that actually get compiled? Those are the .cpp files, which is where a double definition
    # becomes a real problem.
    entries = sorted(SRC.rglob("*.cpp"))
    failures = []
    warnings = []
    for h in unguarded:
        worst = 0
        for e in entries:
            counts = reach_counts(e)
            worst = max(worst, counts.get(h.resolve(), 0))
        if worst > 1:
            failures.append((h, worst))
        else:
            warnings.append(h)

    print()
    for h in warnings:
        print(f"  WARN  {h.relative_to(ROOT)} has no guard (reached once today)")

    if failures:
        for h, n in failures:
            print(f"  FAIL  {h.relative_to(ROOT)} has no include guard and is reached {n}x "
                  f"in one translation unit -- its classes would be defined {n} times")
        print()
        print("  Add `#pragma once` as the first line of the file.")
        return 1

    print()
    print("  ok    no unguarded header is reached twice")
    return 0


if __name__ == "__main__":
    sys.exit(main())
