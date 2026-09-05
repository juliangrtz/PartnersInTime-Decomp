"""Shared deterministic DS-input parsing for py-desmume tools."""

from __future__ import annotations

import argparse

from desmume.controls import Keys, keymask


DS_KEYS = {
    "a": Keys.KEY_A,
    "b": Keys.KEY_B,
    "select": Keys.KEY_SELECT,
    "start": Keys.KEY_START,
    "right": Keys.KEY_RIGHT,
    "left": Keys.KEY_LEFT,
    "up": Keys.KEY_UP,
    "down": Keys.KEY_DOWN,
    "r": Keys.KEY_R,
    "l": Keys.KEY_L,
    "x": Keys.KEY_X,
    "y": Keys.KEY_Y,
}


def parse_action(value: str) -> tuple[str, int]:
    name, separator, frames_text = value.lower().partition(":")
    if name != "wait" and name not in DS_KEYS:
        raise argparse.ArgumentTypeError(f"unknown DS input: {name}")
    try:
        frames = int(frames_text) if separator else 1
    except ValueError as error:
        raise argparse.ArgumentTypeError("frame count must be an integer") from error
    if frames < 1:
        raise argparse.ArgumentTypeError("frame count must be positive")
    return name, frames


def action_mask(name: str) -> int:
    return 0 if name == "wait" else keymask(DS_KEYS[name])
