"""Configure a raw PiT EUR overlay-2 database for 32-bit ARM analysis.

Run this script from the repository root with IDA's text-mode executable after
loading the uncompressed overlay at 0x02065D40. It imports the maintained DSD
symbol map, defines known function extents, and leaves the database ready for
interactive Hex-Rays analysis.
"""

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


OVERLAY_BASE = 0x02065D40
OVERLAY_TEXT_END = 0x020BE504
OVERLAY_RODATA_END = 0x020BE740
OVERLAY_STORED_END = 0x020C0660
OVERLAY_BSS_END = 0x020C0FC0
SYMBOLS_PATH = Path("config/eur/arm9/overlays/ov002/symbols.txt")

SYMBOL_RE = re.compile(
    r"^(?P<name>\S+)\s+kind:(?P<kind>\w+)"
    r"(?:\((?P<details>[^)]*)\))?\s+addr:(?P<address>0x[0-9a-fA-F]+)"
    r"(?:\s+.*)?$"
)
SIZE_RE = re.compile(r"(?:^|,)size=(0x[0-9a-fA-F]+)(?:,|$)")

BATTLE_AI_TYPES = r"""
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;

typedef struct BattleAIState BattleAIState;
typedef struct BattleAITask BattleAITask;
typedef struct BattleTaskPool BattleTaskPool;

struct BattleAIState {
    const void *script;
    u8 unk_004[0xA4];
    u16 scratch_a8;
    u16 scratch_aa;
    u8 unk_0ac[4];
    u16 owner_id;
    u16 flags;
    s16 order;
    u16 order_tie_break;
    const void *continuation_script;
    u16 continuation_order;
    u16 continuation_tie_break;
};

struct BattleAITask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleAIState *state;
    u16 id;
    u16 padding_12;
};

struct BattleTaskPool {
    BattleAITask *active;
    BattleAITask *free;
};
"""

FUNCTION_TYPES = {
    "BattleAI_HandleVmResult":
        "int BattleAI_HandleVmResult(BattleAITask *, int, BattleAIState *);",
    "BattleAI_StartReactionScript": "void BattleAI_StartReactionScript(int);",
    "BattleAI_StartActionScript": "void BattleAI_StartActionScript(int);",
    "BattleAI_StartPartyVmSlot4": "void BattleAI_StartPartyVmSlot4(void);",
    "BattleAI_StartPartyVmSlot3": "void BattleAI_StartPartyVmSlot3(void);",
    "BattleAI_StartPartyVmSlot2": "void BattleAI_StartPartyVmSlot2(void);",
    "BattleAI_StartPartyVmSlot1": "void BattleAI_StartPartyVmSlot1(void);",
    "BattleAI_InitStateFromScriptBlock":
        "void BattleAI_InitStateFromScriptBlock(BattleAIState *, const u16 *, int);",
    "BattleScriptState_GetByObjectId":
        "BattleAIState *BattleScriptState_GetByObjectId(u16);",
    "BattlePool_ReturnNode":
        "void BattlePool_ReturnNode(BattleTaskPool *, BattleAITask *);",
    "BattlePool_TakeNode": "BattleAITask *BattlePool_TakeNode(BattleTaskPool *);",
    "BattleTask_BindOwnerSlot":
        "BattleAITask *BattleTask_BindOwnerSlot(BattleAITask *, BattleAITask **);",
    "BattleTask_Release": "void BattleTask_Release(BattleAITask *);",
    "BattleTaskList_Insert":
        "BattleAITask *BattleTaskList_Insert(BattleTaskPool *, BattleAITask *);",
    "BattleTaskPool_Allocate":
        "BattleAITask *BattleTaskPool_Allocate(BattleTaskPool *);",
    "BattleTaskPool_Init": "void BattleTaskPool_Init(BattleTaskPool *, int, u32);",
}


def read_symbols(path: Path) -> list[tuple[str, str, int, int | None]]:
    symbols = []
    for line_number, raw_line in enumerate(path.read_text().splitlines(), 1):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        match = SYMBOL_RE.match(line)
        if match is None:
            raise ValueError(f"{path}:{line_number}: unsupported symbol line: {line}")

        details = match.group("details") or ""
        size_match = SIZE_RE.search(details)
        size = int(size_match.group(1), 0) if size_match else None
        symbols.append(
            (
                match.group("name"),
                match.group("kind"),
                int(match.group("address"), 0),
                size,
            )
        )
    return symbols


def configure_arm32() -> ida_segment.segment_t:
    if not ida_idp.set_processor_type("ARM:ARMv5TE", ida_idp.SETPROC_USER):
        if not ida_idp.set_processor_type("ARM", ida_idp.SETPROC_USER):
            raise RuntimeError("IDA rejected the ARM processor module")

    ida_ida.inf_set_app_bitness(32)
    segment = ida_segment.getseg(OVERLAY_BASE)
    if segment is None:
        raise RuntimeError(f"no raw segment at 0x{OVERLAY_BASE:08X}")
    if not ida_segment.set_segm_addressing(segment, 1):
        raise RuntimeError("could not switch the overlay segment to 32-bit addressing")

    if not ida_segment.set_segm_end(
        segment.start_ea, OVERLAY_TEXT_END, ida_segment.SEGMOD_KEEP
    ):
        raise RuntimeError("could not split the overlay text segment")
    ida_segment.set_segm_name(segment, "ov002_text")
    ida_segment.set_segm_class(segment, "CODE")

    for start, end, name, segment_class in (
        (OVERLAY_TEXT_END, OVERLAY_RODATA_END, "ov002_rodata", "CONST"),
        (OVERLAY_RODATA_END, OVERLAY_STORED_END, "ov002_data", "DATA"),
        (OVERLAY_STORED_END, OVERLAY_BSS_END, "ov002_bss", "BSS"),
    ):
        if not ida_segment.add_segm(0, start, end, name, segment_class):
            raise RuntimeError(f"could not create IDA segment {name}")
        new_segment = ida_segment.getseg(start)
        if new_segment is None or not ida_segment.set_segm_addressing(new_segment, 1):
            raise RuntimeError(f"could not configure IDA segment {name} as 32-bit")

    thumb_register = ida_idp.str2reg("T")
    if thumb_register >= 0:
        ida_segregs.set_default_sreg_value(segment, thumb_register, 0)
    return segment


def import_symbols(symbols: list[tuple[str, str, int, int | None]]) -> tuple[int, int]:
    named = 0
    functions = 0
    for name, kind, address, size in symbols:
        if not (OVERLAY_BASE <= address < OVERLAY_BSS_END):
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


def import_battle_types(symbols: list[tuple[str, str, int, int | None]]) -> int:
    errors = idc.parse_decls(BATTLE_AI_TYPES, 0)
    if errors:
        raise RuntimeError(f"IDA reported {errors} error(s) parsing battle AI types")

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

    symbols = read_symbols(SYMBOLS_PATH)
    ida_auto.enable_auto(False)
    segment = configure_arm32()
    named, functions = import_symbols(symbols)
    typed = import_battle_types(symbols)
    ida_auto.enable_auto(True)
    ida_auto.plan_and_wait(segment.start_ea, segment.end_ea)
    ida_kernwin.msg(
        "PiT overlay 2 import complete: "
        f"{named} names, {functions} functions, {typed} typed battle APIs, "
        f"ARM32 at 0x{OVERLAY_BASE:08X}\n"
    )
    ida_pro.qexit(0)


if __name__ == "__main__":
    main()
