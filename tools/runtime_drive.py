#!/usr/bin/env python3
"""Drive deterministic DS inputs into a py-desmume session."""

from __future__ import annotations

import argparse
from pathlib import Path

from desmume.emulator import DeSmuME

from runtime_inputs import action_mask, parse_action


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, required=True, help="legally obtained PiT ROM")
    parser.add_argument("--battery-save", type=Path, help="raw .sav battery save to import")
    parser.add_argument("--state", type=Path, help="py-desmume-compatible .dst state to load")
    parser.add_argument(
        "--action",
        type=parse_action,
        action="append",
        default=[],
        metavar="KEY[:FRAMES]",
        help="ordered input; KEY is a DS button or wait (repeatable)",
    )
    parser.add_argument("--save-state", type=Path, required=True, help="resulting .dst state")
    parser.add_argument("--screenshot", type=Path, help="optional resulting screen image")
    parser.add_argument(
        "--trace-directory",
        type=Path,
        help="optional screenshot directory, captured after every action",
    )
    return parser


def require_file(path: Path | None, description: str) -> None:
    if path is not None and not path.is_file():
        raise SystemExit(f"missing {description}: {path}")


def run_frames(emulator: DeSmuME, count: int) -> None:
    for _ in range(count):
        emulator.cycle(with_joystick=False)


def main() -> int:
    args = build_argument_parser().parse_args()
    require_file(args.rom, "ROM")
    require_file(args.battery_save, "battery save")
    require_file(args.state, "savestate")
    args.save_state.parent.mkdir(parents=True, exist_ok=True)
    if args.screenshot is not None:
        args.screenshot.parent.mkdir(parents=True, exist_ok=True)
    if args.trace_directory is not None:
        args.trace_directory.mkdir(parents=True, exist_ok=True)

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

        emulator.resume()
        for index, (name, frames) in enumerate(args.action):
            emulator.input.keypad_update(action_mask(name))
            run_frames(emulator, frames)
            emulator.input.keypad_update(0)
            run_frames(emulator, 1)
            print(f"action {index:02d}: {name} for {frames} frame(s)")
            if args.trace_directory is not None:
                emulator.screenshot().save(
                    args.trace_directory / f"{index:02d}_{name}_{frames}.png"
                )
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
