"""The oscillator waveform, generated from the two live parameters.

SHAPE (ids 14 and 18) is a continuous 0-100 % knob and PW (ids 17 and 21) is 5-95 %
(`docs/PARAM-MANIFEST.md`). Nothing here is fixed or decorative: every point of a drawn
trace is computed from those two values, so turning SHAPE morphs the picture and turning
PW narrows or widens it.

Mapping
    shape   0 %  -> the corners are fully rounded, so the pulse reads as a soft wave
    shape 100 %  -> hard square corners
    pw           -> the duty cycle of the high part, so it narrows as pw falls

Shared by `tools/mockup_states.py` (and so `tools/osc_wireframe_in_B.py`) and
`tools/wireframe.py` (and so `tools/wireframe_osc.py`).
"""

import math


def smoothstep(t):
    t = min(1.0, max(0.0, t))
    return t * t * (3.0 - 2.0 * t)


def wave_value(phase, shape, pw):
    """Wave value in [-1, 1] at `phase` cycles, for shape 0-100 % and pw 5-95 %.

    A pulse of duty `pw` whose two corners are rounded by an edge of width
    `0.5 * (1 - shape) * min(duty, 1 - duty)` cycles, so shape 100 % is a hard square
    and shape 0 % is a smooth rounded pulse.
    """
    duty = min(0.95, max(0.05, pw / 100.0))
    shape = min(100.0, max(0.0, shape))
    edge = 0.5 * (1.0 - shape / 100.0) * min(duty, 1.0 - duty)
    if edge <= 1e-9:
        return 1.0 if (phase % 1.0) < duty else -1.0
    k = math.floor(phase)
    return -1.0 + 2.0 * (smoothstep((phase - k) / edge) - smoothstep((phase - k - duty) / edge))


def wave_points(x, y, w, h, shape, pw, cycles=2.0, amp=0.32):
    """Trace points for a scope tile at (x, y) of size (w, h).

    `cycles` is the fixed width of the display window in cycles; the shape of the trace
    inside it comes only from `shape` and `pw`.
    """
    w, h = int(w), int(h)
    n = max(8, w - 4)
    return [(x + 2 + i,
             y + h / 2.0 - wave_value(i / (n - 1.0) * cycles, shape, pw) * h * amp)
            for i in range(n)]
