"""Find headers that use a std type without including its header.

    /work/.venv/bin/python tools/check_includes.py

## Why

Windows failed to build on `std::string` in SamplerVoice.h, which used it without
including <string>. GCC pulls the header in transitively through another include, so the
file compiled on macOS and Linux and failed only under MSVC. That is the worst shape of
portability bug: invisible on the machine being developed on.

This checks the headers in src/ for that class of fault. It is deliberately conservative:
it only reports a missing include when the type is used and no include that could supply
it is present, so a transitive include that genuinely provides the type is not reported
as an error.

## What it does not do

It does not parse C++. A `std::vector` inside a comment or a string would be counted, and
a header included for its own sake rather than for the type is not understood. It is a
smell detector for a real class of bug, not a compiler -- the compiler is the authority,
and this just aims to catch on Linux what would otherwise only fail on Windows.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"

# std type -> the header that declares it. Only the ones that actually need an include;
# types like std::size_t come from <cstddef> via everything and are noisy.
NEEDS = {
    "std::string": "<string>",
    "std::vector": "<vector>",
    "std::array": "<array>",
    "std::map": "<map>",
    "std::unordered_map": "<unordered_map>",
    "std::set": "<set>",
    "std::shared_ptr": "<memory>",
    "std::unique_ptr": "<memory>",
    "std::make_shared": "<memory>",
    "std::make_unique": "<memory>",
    "std::function": "<functional>",
    "std::optional": "<optional>",
    "std::tuple": "<tuple>",
    "std::atomic": "<atomic>",
    "std::filesystem": "<filesystem>",
    "std::error_code": "<system_error>",
    "std::getenv": "<cstdlib>",
    "std::pow": "<cmath>",
    "std::sqrt": "<cmath>",
    "std::fabs": "<cmath>",
    "std::sin": "<cmath>",
    "std::cos": "<cmath>",
    "std::log10": "<cmath>",
    "std::tanh": "<cmath>",
    "std::exp": "<cmath>",
    "std::snprintf": "<cstdio>",
    "std::fopen": "<cstdio>",
    "std::stoi": "<string>",
    "std::to_string": "<string>",
    "std::abs": "<cstdlib>",
    "std::min": "<algorithm>",
    "std::max": "<algorithm>",
    "std::clamp": "<algorithm>",
    "std::sort": "<algorithm>",
    "std::fill": "<algorithm>",
    "std::fill_n": "<algorithm>",
}


def strip_comments(text):
    """Remove // and /* */ comments, so a type named in prose is not counted as a use."""
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    text = re.sub(r"//[^\n]*", " ", text)
    return text


def includes_of(text):
    return set(re.findall(r"#\s*include\s*[<\"]([^>\"]+)[>\"]", text))


def main():
    problems = []
    checked = 0
    for path in sorted(SRC.rglob("*.h")) + sorted(SRC.rglob("*.cpp")):
        raw = path.read_text()
        body = strip_comments(raw)
        have = includes_of(raw)
        checked += 1
        for sym, header in NEEDS.items():
            if not re.search(re.escape(sym) + r"\b", body):
                continue
            name = header.strip("<>")
            if name in have:
                continue
            # <memory> also provides the C++14 aliases; <string> provides stoi/to_string.
            if header == "<memory>" and "memory" in have:
                continue
            problems.append((str(path.relative_to(ROOT)), sym, header))

    print(f"  headers and sources checked: {checked}")
    if not problems:
        print("  ok    every std type used has its header included")
        return 0

    # One line per file, listing what is missing. Grouped because a file usually wants one fix.
    by_file = {}
    for f, sym, header in problems:
        by_file.setdefault(f, set()).add(header)
    for f in sorted(by_file):
        print(f"  FAIL  {f}: missing {' '.join(sorted(by_file[f]))}")
    print()
    print("  A missing standard include is invisible on GCC, which pulls the header in")
    # The count is a hard assertion, so compute it rather than describe it loosely.
    print(f"  transitively, and fails on MSVC. {' '.join(sorted({f for f, _, _ in problems}))}")
    return 1


if __name__ == "__main__":
    sys.exit(main())
