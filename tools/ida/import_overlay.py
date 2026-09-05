"""Configure a raw PiT EUR overlay database for 32-bit ARM analysis.

The overlay ID is supplied as the script argument. Section bounds and symbols
come from the maintained DSD configuration, so this works for every non-empty
overlay without embedding ROM-derived bytes in the repository.
"""

from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path

import ida_auto
import ida_bytes
import ida_funcs
import ida_ida
import ida_idp
import ida_kernwin
import ida_name
import ida_pro
import ida_segment
import ida_segregs
import ida_ua
import idc


SECTION_RE = re.compile(
    r"^\s+(?P<name>\.\S+)\s+start:(?P<start>0x[0-9a-fA-F]+)\s+"
    r"end:(?P<end>0x[0-9a-fA-F]+)\s+kind:(?P<kind>\w+)"
)
SYMBOL_RE = re.compile(
    r"^(?P<name>\S+)\s+kind:(?P<kind>\w+)"
    r"(?:\((?P<details>[^)]*)\))?\s+addr:(?P<address>0x[0-9a-fA-F]+)"
    r"(?:\s+.*)?$"
)
SIZE_RE = re.compile(r"(?:^|,)size=(0x[0-9a-fA-F]+)(?:,|$)")


@dataclass(frozen=True)
class Section:
    name: str
    start: int
    end: int
    kind: str


def read_sections(path: Path) -> list[Section]:
    sections = []
    for line in path.read_text().splitlines():
        # Indented entries before the first translation-unit heading describe
        # the overlay image itself. Later indented entries are delink ranges.
        if line and not line[0].isspace():
            break
        match = SECTION_RE.match(line)
        if match:
            sections.append(
                Section(
                    match.group("name"),
                    int(match.group("start"), 0),
                    int(match.group("end"), 0),
                    match.group("kind"),
                )
            )
    return sections


def read_symbols(path: Path) -> list[tuple[str, str, int, int | None]]:
    symbols = []
    for line_number, raw_line in enumerate(path.read_text().splitlines(), 1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        match = SYMBOL_RE.match(line)
        if match is None:
            raise ValueError(f"{path}:{line_number}: unsupported symbol: {line}")
        details = match.group("details") or ""
        size_match = SIZE_RE.search(details)
        symbols.append(
            (
                match.group("name"),
                match.group("kind"),
                int(match.group("address"), 0),
                int(size_match.group(1), 0) if size_match else None,
            )
        )
    return symbols


def segment_class(section: Section) -> str:
    if section.kind == "code":
        return "CODE"
    if section.kind == "bss":
        return "BSS"
    if section.name in {".rodata", ".ctor"}:
        return "CONST"
    return "DATA"


def configure_arm32(overlay_id: int, sections: list[Section]) -> tuple[int, int]:
    if not sections:
        raise RuntimeError(f"overlay {overlay_id} has no mapped sections")
    if not ida_idp.set_processor_type("ARM:ARMv5TE", ida_idp.SETPROC_USER):
        if not ida_idp.set_processor_type("ARM", ida_idp.SETPROC_USER):
            raise RuntimeError("IDA rejected the ARM processor module")

    ida_ida.inf_set_app_bitness(32)
    first = sections[0]
    segment = ida_segment.getseg(first.start)
    if segment is None:
        raise RuntimeError(f"no raw segment at 0x{first.start:08X}")
    if not ida_segment.set_segm_addressing(segment, 1):
        raise RuntimeError("could not switch the overlay to 32-bit addressing")

    mapped = [section for section in sections if section.start < section.end]
    for index, section in enumerate(mapped):
        end = mapped[index + 1].start if index + 1 < len(mapped) else section.end
        name = f"ov{overlay_id:03d}_{section.name[1:]}"
        if index == 0:
            if not ida_segment.set_segm_end(
                segment.start_ea, end, ida_segment.SEGMOD_KEEP
            ):
                raise RuntimeError(f"could not split segment {name}")
            current = segment
        else:
            if not ida_segment.add_segm(
                0, section.start, end, name, segment_class(section)
            ):
                raise RuntimeError(f"could not create segment {name}")
            current = ida_segment.getseg(section.start)
            if current is None or not ida_segment.set_segm_addressing(current, 1):
                raise RuntimeError(f"could not configure segment {name}")
        ida_segment.set_segm_name(current, name)
        ida_segment.set_segm_class(current, segment_class(section))

    thumb_register = ida_idp.str2reg("T")
    if thumb_register >= 0:
        for section in mapped:
            current = ida_segment.getseg(section.start)
            if current is not None:
                ida_segregs.set_default_sreg_value(current, thumb_register, 0)
    return first.start, max(section.end for section in mapped)


def import_symbols(
    symbols: list[tuple[str, str, int, int | None]], start: int, end: int
) -> tuple[int, int]:
    named = 0
    functions = 0
    for name, kind, address, size in symbols:
        if not (start <= address < end):
            continue
        if kind == "function" and size is not None:
            existing = ida_funcs.get_func(address)
            if existing is not None:
                ida_funcs.del_func(address)
            ida_bytes.del_items(address, ida_bytes.DELIT_SIMPLE, size)
            ida_ua.create_insn(address)
            if ida_funcs.add_func(address, address + size):
                functions += 1
        if ida_name.set_name(address, name, ida_name.SN_FORCE | ida_name.SN_NOCHECK):
            named += 1
    return named, functions


def main() -> None:
    if len(idc.ARGV) != 2:
        raise RuntimeError("usage: import_overlay.py OVERLAY_ID")
    overlay_id = int(idc.ARGV[1], 0)
    config_dir = Path(f"config/eur/arm9/overlays/ov{overlay_id:03d}")
    delinks_path = config_dir / "delinks.txt"
    symbols_path = config_dir / "symbols.txt"
    if not delinks_path.is_file() or not symbols_path.is_file():
        raise RuntimeError(
            "run IDA from the repository root; overlay configuration is missing"
        )

    sections = read_sections(delinks_path)
    symbols = read_symbols(symbols_path)
    ida_auto.enable_auto(False)
    start, end = configure_arm32(overlay_id, sections)
    named, functions = import_symbols(symbols, start, end)
    ida_auto.enable_auto(True)
    for section in sections:
        if section.kind == "code":
            ida_auto.plan_and_wait(section.start, section.end)
    ida_kernwin.msg(
        f"PiT overlay {overlay_id} import complete: {named} names, "
        f"{functions} functions, ARM32 at 0x{start:08X}\n"
    )
    ida_pro.qexit(0)


if __name__ == "__main__":
    main()
