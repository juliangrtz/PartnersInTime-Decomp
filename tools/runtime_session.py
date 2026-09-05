#!/usr/bin/env python3
"""Run an interactive py-desmume session and save a compatible runtime state."""

from __future__ import annotations

import argparse
from pathlib import Path

from desmume.emulator import DeSmuME


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
            if not emulator.backup.import_file(str(args.battery_save.resolve())):
                raise RuntimeError(f"failed to import battery save: {args.battery_save}")
        if args.state is not None:
            emulator.savestate.load_file(str(args.state.resolve()))

        window = emulator.create_sdl_window(
            auto_pause=True,
            use_opengl_if_possible=not args.software_renderer,
        )
        emulator.resume()
        print("py-desmume session running; close the emulator window to capture the state")
        while not window.has_quit():
            window.process_input()
            emulator.cycle(with_joystick=True)
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
