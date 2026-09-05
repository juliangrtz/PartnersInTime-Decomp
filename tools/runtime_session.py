#!/usr/bin/env python3
"""Run an interactive py-desmume session and save a compatible runtime state."""

from __future__ import annotations

import argparse
import ctypes
import os
import sys
from pathlib import Path

from desmume.controls import Keys, keymask
from desmume.emulator import DeSmuME


WINDOWS_KEY_BINDINGS = {
    0x58: Keys.KEY_A,       # X
    0x5A: Keys.KEY_B,       # Z
    0xA1: Keys.KEY_SELECT,  # right Shift
    0x0D: Keys.KEY_START,   # Enter
    0x27: Keys.KEY_RIGHT,
    0x25: Keys.KEY_LEFT,
    0x26: Keys.KEY_UP,
    0x28: Keys.KEY_DOWN,
    0x57: Keys.KEY_R,       # W
    0x51: Keys.KEY_L,       # Q
    0x53: Keys.KEY_X,       # S
    0x41: Keys.KEY_Y,       # A
}


def windows_keypad_mask() -> int:
    """Read physical Windows keys, bypassing py-desmume's broken SDL key table."""
    user32 = ctypes.windll.user32
    user32.GetForegroundWindow.restype = ctypes.c_void_p
    user32.GetWindowThreadProcessId.argtypes = [
        ctypes.c_void_p,
        ctypes.POINTER(ctypes.c_ulong),
    ]
    foreground_process = ctypes.c_ulong()
    user32.GetWindowThreadProcessId(
        user32.GetForegroundWindow(), ctypes.byref(foreground_process)
    )
    if foreground_process.value != os.getpid():
        return 0
    return sum(
        keymask(ds_key)
        for virtual_key, ds_key in WINDOWS_KEY_BINDINGS.items()
        if user32.GetAsyncKeyState(virtual_key) & 0x8000
    )


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, required=True, help="legally obtained PiT ROM or rebuilt ROM")
    parser.add_argument("--battery-save", type=Path, help="raw .sav battery save to import")
    parser.add_argument("--state", type=Path, help="py-desmume-compatible .dst state to load")
    parser.add_argument("--save-state", type=Path, required=True, help="state written when the window closes")
    parser.add_argument("--screenshot", type=Path, help="optional screenshot written on exit")
    parser.add_argument(
        "--software-renderer",
        action="store_true",
        help="disable OpenGL for the simple SDL window",
    )
    return parser


def require_file(path: Path | None, description: str) -> None:
    if path is not None and not path.is_file():
        raise SystemExit(f"missing {description}: {path}")


def main() -> int:
    args = build_argument_parser().parse_args()
    require_file(args.rom, "ROM")
    require_file(args.battery_save, "battery save")
    require_file(args.state, "savestate")

    args.save_state.parent.mkdir(parents=True, exist_ok=True)
    if args.screenshot is not None:
        args.screenshot.parent.mkdir(parents=True, exist_ok=True)

    emulator = DeSmuME()
    try:
        emulator.open(str(args.rom.resolve()), auto_resume=False)
        if args.battery_save is not None:
            if not emulator.backup.import_file(
                str(args.battery_save.resolve()),
                force_size=args.battery_save.stat().st_size,
            ):
                raise RuntimeError(f"failed to import battery save: {args.battery_save}")
        if args.state is not None:
            emulator.savestate.load_file(str(args.state.resolve()))

        window = emulator.create_sdl_window(
            auto_pause=True,
            use_opengl_if_possible=not args.software_renderer,
        )
        emulator.resume()
        print("py-desmume session running; close the emulator window to capture the state")
        print("controls: arrows=D-pad, X=A, Z=B, Enter=Start, Right Shift=Select")
        print("          Q=L, W=R, S=DS X, A=DS Y")
        while not window.has_quit():
            window.process_input()
            if sys.platform == "win32":
                # py-desmume 0.0.9 copies its u32 SDL key table into a u16
                # buffer. Overwrite the resulting bogus mask with a direct
                # physical-key poll until that upstream bug is fixed.
                emulator.input.keypad_update(windows_keypad_mask())
            emulator.cycle(with_joystick=sys.platform != "win32")
            window.draw()

        emulator.pause()
        emulator.savestate.save_file(str(args.save_state.resolve()))
        if args.screenshot is not None:
            emulator.screenshot().save(args.screenshot)
        print(f"wrote compatible state: {args.save_state.resolve()}")
        if args.screenshot is not None:
            print(f"wrote screenshot: {args.screenshot.resolve()}")
    finally:
        emulator.destroy()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
