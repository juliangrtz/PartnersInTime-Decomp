#!/usr/bin/env python3
"""Reject private game content and proprietary binaries tracked by Git."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[1]
PRIVATE_PREFIXES = (
    "private/",
    "tools/mwccarm/",
    "build/",
    "asm/",
)
PRIVATE_DATA_ROOTS = ("data/", "extract/")
ALLOWED_DATA_FILES = {"data/README.md", "extract/.gitignore", "extract/README.md"}
PRIVATE_SUFFIXES = {
    ".nds", ".srl", ".bin", ".dat", ".narc", ".ncgr", ".nclr", ".nscr",
    ".sdat", ".sav", ".dsv", ".dst", ".duc", ".idb", ".i64", ".id0",
    ".id1", ".id2", ".nam", ".til", ".bkp", ".exe", ".dll", ".lib",
    ".png", ".bmp", ".gif", ".jpg", ".jpeg", ".webp", ".wav", ".mp3",
    ".ogg", ".flac", ".mid", ".sseq", ".swar", ".sbnk", ".strm",
}


def tracked_files() -> list[str]:
    result = subprocess.run(
        ["git", "ls-files", "-z"],
        cwd=ROOT,
        check=True,
        capture_output=True,
    )
    return [path.decode("utf-8") for path in result.stdout.split(b"\0") if path]


def reason_private(path: str) -> str | None:
    lower = path.lower()
    if lower.startswith(PRIVATE_PREFIXES):
        return "private or generated directory"
    if lower.startswith(PRIVATE_DATA_ROOTS) and path not in ALLOWED_DATA_FILES:
        return "ROM-derived data directory"
    if PurePosixPath(lower).suffix in PRIVATE_SUFFIXES:
        return "private binary, tool, database, or media format"
    return None


def main() -> int:
    violations = [
        (path, reason)
        for path in tracked_files()
        if (reason := reason_private(path)) is not None
    ]
    if not violations:
        print("Public-content audit passed: no known private content is tracked.")
        return 0

    print("Public-content audit failed:", file=sys.stderr)
    for path, reason in violations:
        print(f"  {path}: {reason}", file=sys.stderr)
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
