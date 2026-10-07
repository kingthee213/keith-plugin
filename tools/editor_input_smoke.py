"""Exercise numeric entry and drag in an explicitly selected Keith test process.

Run with --pid <test PID> at the native 1280x800 editor size. Changes cutoff to
5000 Hz and volume attack to 20 or 21 ms; never targets the foreground application.
"""
import argparse
import ctypes
from ctypes import wintypes as W
import json
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pid', type=int, required=True)
    args = parser.parse_args()
    user = ctypes.WinDLL('user32', use_last_error=True)
    callback = ctypes.WINFUNCTYPE(W.BOOL, W.HWND, W.LPARAM)
    user.SendMessageTimeoutW.argtypes = [W.HWND, W.UINT, W.WPARAM, W.LPARAM,
                                        W.UINT, W.UINT, ctypes.POINTER(ctypes.c_size_t)]
    user.SendMessageTimeoutW.restype = W.LPARAM
    children = []

    @callback
    def collect_child(hwnd, _):
        children.append(hwnd)
        return True

    @callback
    def collect_window(hwnd, _):
        pid = W.DWORD()
        user.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
        if pid.value == args.pid:
            user.EnumChildWindows(hwnd, collect_child, 0)
        return True

    def class_name(hwnd):
        value = ctypes.create_unicode_buffer(100)
        user.GetClassNameW(hwnd, value, len(value))
        return value.value

    user.EnumWindows(collect_window, 0)
    graphics = [h for h in children if class_name(h) == 'IPlugWndClass']
    assert len(graphics) == 1, f'Expected one Keith editor, found {len(graphics)}'
    graphics = graphics[0]
    rect = W.RECT()
    user.GetClientRect(graphics, ctypes.byref(rect))
    assert (rect.right, rect.bottom) == (1280, 800), 'Use the native editor size'

    def send(hwnd, message, wparam=0, lparam=0):
        result = ctypes.c_size_t()
        ok = user.SendMessageTimeoutW(hwnd, message, wparam, lparam,
                                      0x0002, 3000, ctypes.byref(result))
        assert ok, f'Target stopped responding to {message:#x}'
        return result.value

    def click(x, y):
        position = (y << 16) | x
        send(graphics, 0x201, 1, position)
        send(graphics, 0x202, 0, position)

    def field():
        children.clear()
        user.EnumChildWindows(graphics, collect_child, 0)
        entries = [h for h in children if class_name(h) == 'Edit']
        assert len(entries) == 1, f'Expected one numeric field, found {len(entries)}'
        return entries[0]

    def read(hwnd):
        text = ctypes.create_unicode_buffer(256)
        send(hwnd, 0xD, len(text), ctypes.addressof(text))
        return float(text.value)

    def finish(hwnd, key):
        send(hwnd, 0x100, key)
        deadline = time.monotonic() + 3
        while user.IsWindow(hwnd) and time.monotonic() < deadline:
            time.sleep(0.02)
        assert not user.IsWindow(hwnd), 'Numeric field did not close'

    def cancel(hwnd):
        finish(hwnd, 0x1B)

    def enter(hwnd, value):
        text = ctypes.create_unicode_buffer(str(value))
        send(hwnd, 0xC, 0, ctypes.addressof(text))
        finish(hwnd, 0x0D)

    # Dismiss a prior test's editor without altering its current parameter.
    existing = [h for h in children if class_name(h) == 'Edit']
    if existing:
        cancel(existing[0])

    def nav(index):
        # Six cells across kNavBar (34, 144, 1022, 185), gap 8.
        width = (1022 - 34 - 8 * 5) / 6
        x = int(34 + index * (width + 8) + width / 2)
        click(x, 164)

    nav(0)  # 01 Source.
    click(1143, 429)
    initial = read(field())
    cancel(field())
    send(graphics, 0x201, 1, (356 << 16) | 1143)
    send(graphics, 0x200, 1, (386 << 16) | 1143)
    send(graphics, 0x202, 0, (386 << 16) | 1143)
    click(1143, 429)
    dragged = read(field())
    assert dragged < initial, (initial, dragged)
    enter(field(), 5000)
    click(1143, 429)
    cutoff = read(field())
    assert cutoff == 5000, cutoff
    cancel(field())

    nav(1)  # 02 Modulation.
    click(134, 420)  # LFO rate value.
    rate_before = read(field())
    rate_target = 3.5 if abs(rate_before - 2.5) < 0.001 else 2.5
    enter(field(), rate_target)
    click(134, 420)
    rate_after = read(field())
    assert rate_after == rate_target, rate_after
    cancel(field())
    click(395, 462)  # LFO shape, SQUARE tab.

    nav(2)  # 03 Filter.
    click(850, 670)  # Filter drive value.
    drive_before = read(field())
    drive_target = 10 if abs(drive_before - 25) < 0.001 else 25
    enter(field(), drive_target)
    click(850, 670)
    drive_after = read(field())
    assert drive_after == drive_target, drive_after
    cancel(field())

    nav(0)
    click(932, 450)  # OSC SYNC, SYNC tab.
    click(834, 450)  # OSC SYNC, FREE tab.

    nav(3)  # 04 Envelopes, default volume envelope.
    click(170, 249)  # Explicitly select volume rather than cached pitch/filter.
    click(200, 630)
    attack_before = read(field())
    attack_target = 21 if attack_before == 20 else 20
    enter(field(), attack_target)
    click(200, 630)
    attack_after = read(field())
    assert attack_after == attack_target, attack_after
    cancel(field())
    nav(0)
    print(json.dumps({'status': 'pass', 'pid': args.pid,
                      'cutoff_drag': [initial, dragged],
                      'cutoff_numeric_roundtrip': cutoff,
                      'lfo_rate_numeric_roundtrip': [rate_before, rate_after],
                      'filter_drive_numeric_roundtrip': [drive_before, drive_after],
                      'volume_attack_numeric_roundtrip': [attack_before, attack_after]}))


if __name__ == '__main__':
    main()
