#!/usr/bin/env python3
"""Show disassembly context for accesses to one structure offset.

This deliberately works from the project symbol map and an extracted overlay, so
it can find ARM/Thumb accesses that Hex-Rays renders with different expressions.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path

from capstone import CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, Cs


SYMBOL_RE = re.compile(
    r"^(?P<name>\S+) kind:function\((?P<mode>arm|thumb),size=(?P<size>0x[0-9a-fA-F]+)\) "
    r"addr:(?P<address>0x[0-9a-fA-F]+)$"
)


def parse_int(value: str) -> int:
    return int(value, 0)


def token_pattern(text: str) -> re.Pattern[str]:
    """Match disassembly text without accepting a longer numeric token."""

    return re.compile(
        rf"(?<![0-9A-Za-z_]){re.escape(text)}(?![0-9A-Za-z_])",
        re.IGNORECASE,
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary", type=Path)
    parser.add_argument("symbols", type=Path)
    parser.add_argument("offset", type=parse_int)
    parser.add_argument(
        "--image-base",
        type=parse_int,
        help="binary load address (defaults to the lowest mapped function)",
    )
    parser.add_argument("--before", type=int, default=2)
    parser.add_argument("--after", type=int, default=8)
    parser.add_argument(
        "--contains",
        help="only show contexts containing this token-bounded disassembly text",
    )
    args = parser.parse_args()

    binary = args.binary.read_bytes()
    functions = []
    for line in args.symbols.read_text(encoding="utf-8").splitlines():
        match = SYMBOL_RE.match(line)
        if match:
            functions.append(
                (
                    match.group("name"),
                    match.group("mode"),
                    int(match.group("address"), 16),
                    int(match.group("size"), 16),
                )
            )

    if not functions:
        raise SystemExit("symbol map contains no recognized functions")

    image_base = args.image_base
    if image_base is None:
        image_base = min(address for _, _, address, _ in functions)
    offset_patterns = (
        token_pattern(f"#0x{args.offset:x}"),
        token_pattern(f"#{args.offset}"),
    )
    contains_pattern = token_pattern(args.contains) if args.contains else None
    match_count = 0

    for name, mode, address, size in functions:
        start = address - image_base
        if start < 0 or start + size > len(binary):
            continue
        decoder = Cs(CS_ARCH_ARM, CS_MODE_THUMB if mode == "thumb" else CS_MODE_ARM)
        instructions = list(decoder.disasm(binary[start : start + size], address))
        matching_indexes = [
            index
            for index, instruction in enumerate(instructions)
            if any(pattern.search(instruction.op_str) for pattern in offset_patterns)
        ]
        for index in matching_indexes:
            first = max(0, index - args.before)
            last = min(len(instructions), index + args.after + 1)
            if contains_pattern and not any(
                contains_pattern.search(f"{instruction.mnemonic} {instruction.op_str}")
                for instruction in instructions[first:last]
            ):
                continue
            match_count += 1
            print(f"{name} ({mode}, 0x{address:08X})")
            for context_index in range(first, last):
                instruction = instructions[context_index]
                marker = ">" if context_index == index else " "
                print(
                    f"{marker} 0x{instruction.address:08X}: "
                    f"{instruction.mnemonic:<8} {instruction.op_str}"
                )
            print()

    print(f"matches: {match_count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
