"""Targeted Windows input for Keith's native graphics child, without global cursor movement.
Use only against an explicitly authorized Keith test process. Capture/read back after each action.
"""
import argparse
import ctypes
import json
import time
from ctypes import wintypes as W


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--pid', type=int, required=True)
    parser.add_argument('action', choices=['click', 'drag', 'wheel'])
    parser.add_argument('points', nargs='+', type=int)
    args = parser.parse_args()
    user = ctypes.WinDLL('user32', use_last_error=True)
    user.SetProcessDPIAware()
    callback_type = ctypes.WINFUNCTYPE(W.BOOL, W.HWND, W.LPARAM)
    user.SendMessageTimeoutW.argtypes = [W.HWND, W.UINT, W.WPARAM, W.LPARAM, W.UINT, W.UINT, ctypes.POINTER(ctypes.c_size_t)]
    user.SendMessageTimeoutW.restype = W.LPARAM
    targets = []

    @callback_type
    def child(hwnd, _):
        name = ctypes.create_unicode_buffer(256)
        user.GetClassNameW(hwnd, name, 256)
        if name.value == 'IPlugWndClass':
            targets.append(hwnd)
        return True

    @callback_type
    def window(hwnd, _):
        pid = W.DWORD()
        user.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
        if pid.value == args.pid:
            title = ctypes.create_unicode_buffer(256)
            user.GetWindowTextW(hwnd, title, 256)
            if title.value == 'Keith':
                user.EnumChildWindows(hwnd, child, 0)
        return True

    user.EnumWindows(window, 0)
    assert len(targets) == 1, f'Expected one Keith graphics child, found {len(targets)}'
    hwnd = targets[0]
    rect = W.RECT()
    user.GetClientRect(hwnd, ctypes.byref(rect))
    assert (rect.right, rect.bottom) == (1280, 800), 'Probe requires the unscaled design size'

    def send(message, flags, x, y):
        result = ctypes.c_size_t()
        ok = user.SendMessageTimeoutW(hwnd, message, flags, (int(y) << 16) | (int(x) & 65535),
                                      2, 3000, ctypes.byref(result))
        assert ok, 'Keith input handler timed out'

    if args.action == 'click':
        assert len(args.points) == 2
        x, y = args.points
        send(0x200, 0, x, y)
        send(0x201, 1, x, y)
        send(0x202, 0, x, y)
    elif args.action == 'drag':
        assert len(args.points) == 4
        x, y, tx, ty = args.points
        send(0x200, 0, x, y)
        send(0x201, 1, x, y)
        for i in range(1, 21):
            send(0x200, 1, round(x+(tx-x)*i/20), round(y+(ty-y)*i/20))
            time.sleep(0.015)
        send(0x202, 0, tx, ty)
    else:
        assert len(args.points) == 3
        x, y, ticks = args.points
        point = W.POINT(x, y)
        user.ClientToScreen(hwnd, ctypes.byref(point))
        send(0x20A, ((ticks*120)&65535) << 16, point.x, point.y)
    print(json.dumps({'pid': args.pid, 'graphics_hwnd': hwnd, 'action': args.action,
                      'points': args.points, 'delivery': 'targeted Win32 messages',
                      'verification': 'Capture the exact window to verify the effect'}))


if __name__ == '__main__':
    main()
