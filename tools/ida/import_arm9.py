"""Configure a raw PiT EUR resident-ARM9 database for 32-bit ARM analysis."""

from __future__ import annotations

import re
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


ARM9_BASE = 0x02004000
ARM9_TEXT_END = 0x02048EF8
ARM9_INIT_END = 0x02048F08
ARM9_RODATA_END = 0x0204FF6C
ARM9_CTOR_END = 0x0204FF80
ARM9_STORED_END = 0x02059F40
ARM9_BSS_END = 0x02065D40
SYMBOLS_PATH = Path("config/eur/arm9/symbols.txt")

SYMBOL_RE = re.compile(
    r"^(?P<name>\S+)\s+kind:(?P<kind>\w+)"
    r"(?:\((?P<details>[^)]*)\))?\s+addr:(?P<address>0x[0-9a-fA-F]+)"
    r"(?:\s+.*)?$"
)
SIZE_RE = re.compile(r"(?:^|,)size=(0x[0-9a-fA-F]+)(?:,|$)")

SCRIPT_VM_TYPES = r"""
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

typedef struct ScriptVm ScriptVm;
typedef struct ScriptVmState ScriptVmState;
typedef struct ScriptVmCommand ScriptVmCommand;

struct ScriptVm {
    u8 unknown_00[0x0C];
    const u32 *command_descriptors;
};

struct ScriptVmState {
    const u16 *script;
    u8 unknown_04[0xA6];
    u16 delay;
};

struct ScriptVmCommand {
    u16 opcode;
    u16 result_variable;
    u16 argument_modes;
    u16 padding_06;
    s32 arguments[16];
};
"""

FUNCTION_TYPES = {
    "VM_ReadVariable":
        "s32 VM_ReadVariable(u16, ScriptVm *, const u16 **);",
    "VM_Run": "int VM_Run(ScriptVm *, ScriptVmState *);",
    "VM_CheckJumpCondition": "int VM_CheckJumpCondition(int, s32, s32);",
    "VM_ExecuteCommand":
        "int VM_ExecuteCommand(ScriptVm *, ScriptVmState *, ScriptVmCommand *);",
    "VM_ReadCommand":
        "void VM_ReadCommand(ScriptVm *, const u16 **, ScriptVmCommand *);",
}


def read_symbols() -> list[tuple[str, str, int, int | None]]:
    symbols = []
    for line_number, raw_line in enumerate(SYMBOLS_PATH.read_text().splitlines(), 1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        match = SYMBOL_RE.match(line)
        if match is None:
            raise ValueError(
                f"{SYMBOLS_PATH}:{line_number}: unsupported symbol line: {line}"
            )
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


def configure_arm32() -> ida_segment.segment_t:
    if not ida_idp.set_processor_type("ARM:ARMv5TE", ida_idp.SETPROC_USER):
        if not ida_idp.set_processor_type("ARM", ida_idp.SETPROC_USER):
            raise RuntimeError("IDA rejected the ARM processor module")
    ida_ida.inf_set_app_bitness(32)
    segment = ida_segment.getseg(ARM9_BASE)
    if segment is None:
        raise RuntimeError(f"no raw segment at 0x{ARM9_BASE:08X}")
    if not ida_segment.set_segm_addressing(segment, 1):
        raise RuntimeError("could not switch ARM9 to 32-bit addressing")
    if not ida_segment.set_segm_end(
        segment.start_ea, ARM9_TEXT_END, ida_segment.SEGMOD_KEEP
    ):
        raise RuntimeError("could not split the ARM9 text segment")
    ida_segment.set_segm_name(segment, "arm9_text")
    ida_segment.set_segm_class(segment, "CODE")

    for start, end, name, segment_class in (
        (ARM9_TEXT_END, ARM9_INIT_END, "arm9_init", "CODE"),
        (ARM9_INIT_END, ARM9_RODATA_END, "arm9_rodata", "CONST"),
        (ARM9_RODATA_END, ARM9_CTOR_END, "arm9_ctor", "CONST"),
        (ARM9_CTOR_END, ARM9_STORED_END, "arm9_data", "DATA"),
        (ARM9_STORED_END, ARM9_BSS_END, "arm9_bss", "BSS"),
    ):
        if not ida_segment.add_segm(0, start, end, name, segment_class):
            raise RuntimeError(f"could not create IDA segment {name}")
        new_segment = ida_segment.getseg(start)
        if new_segment is None or not ida_segment.set_segm_addressing(new_segment, 1):
            raise RuntimeError(f"could not configure IDA segment {name}")

    thumb_register = ida_idp.str2reg("T")
    if thumb_register >= 0:
        ida_segregs.set_default_sreg_value(segment, thumb_register, 0)
    return segment


def import_symbols(symbols: list[tuple[str, str, int, int | None]]) -> tuple[int, int]:
    named = 0
    functions = 0
    for name, kind, address, size in symbols:
        if not (ARM9_BASE <= address < ARM9_BSS_END):
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


def import_vm_types(symbols: list[tuple[str, str, int, int | None]]) -> int:
    errors = idc.parse_decls(SCRIPT_VM_TYPES, 0)
    if errors:
        raise RuntimeError(f"IDA reported {errors} error(s) parsing script VM types")
    addresses = {name: address for name, _, address, _ in symbols}
    applied = 0
    for name, declaration in FUNCTION_TYPES.items():
        address = addresses.get(name)
        if address is not None and idc.SetType(address, declaration):
            applied += 1
    return applied


def main() -> None:
    if not SYMBOLS_PATH.is_file():
        raise RuntimeError(
            f"run IDA from the repository root; missing {SYMBOLS_PATH.as_posix()}"
        )
    symbols = read_symbols()
    ida_auto.enable_auto(False)
    segment = configure_arm32()
    named, functions = import_symbols(symbols)
    typed = import_vm_types(symbols)
    ida_auto.enable_auto(True)
    ida_auto.plan_and_wait(segment.start_ea, segment.end_ea)
    ida_kernwin.msg(
        "PiT resident ARM9 import complete: "
        f"{named} names, {functions} functions, {typed} typed VM APIs, "
        f"ARM32 at 0x{ARM9_BASE:08X}\n"
    )
    ida_pro.qexit(0)


if __name__ == "__main__":
    main()
