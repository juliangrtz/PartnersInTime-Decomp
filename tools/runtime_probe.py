#!/usr/bin/env python3
"""Capture repeatable DeSmuME runtime evidence for decompilation work."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
from dataclasses import dataclass
from datetime import datetime, timezone
from importlib.metadata import version as package_version
from pathlib import Path
from typing import Any, Callable

import yaml
from desmume.emulator import DeSmuME

from runtime_inputs import action_mask, parse_action


PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_OVERLAY_CONFIG = PROJECT_ROOT / "extract/eur/arm9_overlays/overlays.yaml"
DEFAULT_OVERLAY_DIRECTORY = PROJECT_ROOT / "extract/eur/arm9_overlays"
DEFAULT_SYMBOL_ROOT = PROJECT_ROOT / "config/eur/arm9"
DEFAULT_ENEMY_STATS = PROJECT_ROOT / "data/eur/stats/enemies.json"
MAIN_RAM_START = 0x02000000
MAIN_RAM_END = 0x02400000
# ARM9 CPU-visible ranges. VRAM contents depend on the current bank mapping;
# capture the display registers as well when interpreting a graphics dump.
CAPTURE_REGIONS = (
    (MAIN_RAM_START, MAIN_RAM_END),
    (0x04000000, 0x04002000),  # display and other I/O registers
    (0x05000000, 0x05000800),  # main/sub BG and OBJ palettes
    (0x06000000, 0x06080000),  # main BG
    (0x06200000, 0x06220000),  # sub BG
    (0x06400000, 0x06440000),  # main OBJ
    (0x06600000, 0x06620000),  # sub OBJ
    (0x06800000, 0x068A4000),  # banks mapped to LCDC
    (0x07000000, 0x07000800),  # main/sub OAM
)
SYMBOL_PATTERN = re.compile(r"^(\S+)\s+kind:function[^\n]*\saddr:(0x[0-9a-fA-F]+)")

# European overlay 2 globals and offsets recovered by the matching C sources.
BATTLE_SYSTEM_GLOBAL = 0x020C0714
BATTLE_CONTEXT_GLOBAL = 0x020C0718
BATTLE_CONTEXT_SIZE = 0x3F608
BATTLE_PARTY_ACTOR_TABLE_OFFSET = 0x4FF8
BATTLE_ENEMY_ACTOR_TABLE_OFFSET = 0x6508
BATTLE_BACKGROUND_ID_OFFSET = 0x3A
BATTLE_AI_CONTROL_MASK_OFFSET = 0x10C
BATTLE_RUNTIME_FLAGS_OFFSET = 0xD3A0
BATTLE_PARTY_SLOT_NAMES = ("Mario slot", "Luigi slot", "Baby Mario slot", "Baby Luigi slot")
BATTLE_PARTY_FORMATIONS = {
    0: ("Mario", "Mario"),
    1: ("Luigi", "Luigi"),
    2: ("Baby Mario", "Baby Mario"),
    3: ("Baby Luigi", "Baby Luigi"),
    4: ("Mario", "Mario carrying"),
    5: ("Luigi", "Luigi carrying"),
}


@dataclass(frozen=True)
class MemoryRange:
    name: str
    start: int
    end: int


def parse_int(value: str) -> int:
    return int(value, 0)


def parse_memory_range(value: str) -> MemoryRange:
    if "=" in value:
        name, bounds = value.split("=", 1)
    else:
        name = "range"
        bounds = value
    start_text, separator, end_text = bounds.partition(":")
    if not separator:
        raise argparse.ArgumentTypeError("range must use [NAME=]START:END")
    start = parse_int(start_text)
    end = parse_int(end_text)
    if not re.fullmatch(r"[A-Za-z0-9_-]+", name) or start >= end:
        raise argparse.ArgumentTypeError("range name must be a filename-safe label and START must be below END")
    if not any(low <= start < end <= high for low, high in CAPTURE_REGIONS):
        raise argparse.ArgumentTypeError("range must lie within one ARM9 RAM, I/O, palette, VRAM or OAM region")
    return MemoryRange(name=name, start=start, end=end)


def parse_watch(value: str) -> tuple[int, int]:
    address_text, separator, size_text = value.partition(":")
    address = parse_int(address_text)
    size = parse_int(size_text) if separator else 1
    if size < 1:
        raise argparse.ArgumentTypeError("watch size must be positive")
    return address, size


def sha1_file(path: Path) -> str:
    digest = hashlib.sha1()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def read_bytes(emulator: DeSmuME, start: int, end: int) -> bytes:
    value = emulator.memory.read(start, end, 1, False)
    if not isinstance(value, bytes):
        raise RuntimeError(f"expected a byte range for {start:#010x}:{end:#010x}")
    return value


def read_u8(emulator: DeSmuME, address: int) -> int:
    return int(emulator.memory.unsigned.read_byte(address))


def read_u16(emulator: DeSmuME, address: int) -> int:
    return int(emulator.memory.unsigned.read_short(address))


def read_s16(emulator: DeSmuME, address: int) -> int:
    return int(emulator.memory.signed.read_short(address))


def read_u32(emulator: DeSmuME, address: int) -> int:
    return int(emulator.memory.unsigned.read_long(address))


def read_s32(emulator: DeSmuME, address: int) -> int:
    return int(emulator.memory.signed.read_long(address))


def to_s32(value: int) -> int:
    value &= 0xFFFFFFFF
    return value if value < 0x80000000 else value - 0x100000000


def is_main_ram_pointer(address: int, size: int = 1) -> bool:
    return MAIN_RAM_START <= address and address + size <= MAIN_RAM_END


def is_arm9_work_ram_pointer(address: int, size: int = 1) -> bool:
    """Accept main RAM and the game's 16 KiB DTCM stack region."""
    return is_main_ram_pointer(address, size) or (
        0x027E0000 <= address and address + size <= 0x027E4000
    )


def capture_scene_object(emulator: DeSmuME, address: int) -> dict[str, Any] | None:
    if not is_main_ram_pointer(address, 0x104):
        return None
    return {
        "address": f"{address:#010x}",
        "position": {
            "x": read_s16(emulator, address + 0x004),
            "y": read_s16(emulator, address + 0x006),
            "z": read_s16(emulator, address + 0x008),
        },
        "motion_target": {
            "x": read_s16(emulator, address + 0x010),
            "y": read_s16(emulator, address + 0x012),
            "z": read_s16(emulator, address + 0x014),
        },
        "resource": f"{read_u32(emulator, address + 0x0BC):#010x}",
        "loaded_resource_id": read_s32(emulator, address + 0x0E0),
        "animation_id": read_s16(emulator, address + 0x0E4),
        "actor_id": read_u16(emulator, address + 0x0EC),
        "linked_actor_id": read_u16(emulator, address + 0x0EE),
        "render_state": f"{read_u16(emulator, address + 0x0F0):#06x}",
        "flags": f"{read_u32(emulator, address + 0x0F4):#010x}",
    }


def load_enemy_name_hints(path: Path) -> dict[int, str]:
    if not path.exists():
        return {}
    document = json.loads(path.read_text(encoding="utf-8"))
    return {
        int(record["record_id"]): str(record["name_hint"])
        for record in document.get("records", [])
        if "record_id" in record and "name_hint" in record
    }


def capture_enemy_stats(
    emulator: DeSmuME, resource_slot: int, name_hints: dict[int, str]
) -> dict[str, Any] | None:
    if not is_main_ram_pointer(resource_slot, 0x0C):
        return None
    stats = read_u32(emulator, resource_slot)
    if not is_main_ram_pointer(stats, 0x2C):
        return None
    stat_index = read_u16(emulator, resource_slot + 0x0A)
    return {
        "request": f"{resource_slot:#010x}",
        "address": f"{stats:#010x}",
        "record_id": stat_index,
        "name_hint": name_hints.get(stat_index),
        "name_id": read_u16(emulator, stats + 0x00),
        "packed_object_data_id": f"{read_u16(emulator, stats + 0x02):#06x}",
        "level": read_u8(emulator, stats + 0x05),
        "max_hp": read_u16(emulator, stats + 0x06),
        "power": read_u16(emulator, stats + 0x08),
        "defense": read_u16(emulator, stats + 0x0A),
        "speed": read_u16(emulator, stats + 0x0C),
        "traits": f"{read_u16(emulator, stats + 0x0E):#06x}",
        "experience": read_u16(emulator, stats + 0x20),
        "coins": read_u16(emulator, stats + 0x22),
    }


def capture_battle_actor(
    emulator: DeSmuME,
    address: int,
    actor_id: int,
    slot_kind: str,
    slot_index: int,
    label: str,
    name_hints: dict[int, str],
) -> dict[str, Any]:
    actor_size = 0x84 if slot_kind == "party" else 0x2A0
    if not is_main_ram_pointer(address, actor_size):
        return {
            "actor_id": actor_id,
            "slot_kind": slot_kind,
            "slot_index": slot_index,
            "label": label,
            "address": f"{address:#010x}",
            "valid_pointer": False,
        }

    flags = read_u16(emulator, address + 0x24)
    resource_slot = read_u32(emulator, address + 0x6C)
    actor: dict[str, Any] = {
        "actor_id": actor_id,
        "slot_kind": slot_kind,
        "slot_index": slot_index,
        "label": label,
        "address": f"{address:#010x}",
        "valid_pointer": True,
        "active": slot_kind == "party" or resource_slot != 0,
        "max_hp": read_s16(emulator, address + 0x04),
        "current_hp": read_s16(emulator, address + 0x06),
        "speed": read_s16(emulator, address + 0x08),
        "power": read_s16(emulator, address + 0x0A),
        "defense": read_s16(emulator, address + 0x0C),
        "base_speed": read_s16(emulator, address + 0x10),
        "base_power": read_s16(emulator, address + 0x12),
        "base_defense": read_s16(emulator, address + 0x14),
        "pending_damage": read_s16(emulator, address + 0x1E),
        "target_actor_id": read_s16(emulator, address + 0x20),
        "damage_scale_q8": read_s16(emulator, address + 0x22),
        "flags": f"{flags:#06x}",
        "decoded_flags": {
            "level": flags & 0x7F,
            "knocked_out": bool(flags & 0x0100),
            "hit_locked": bool(flags & 0x0200),
            "resource_bound": bool(flags & 0x0800),
            "excluded_from_targeting": bool(flags & 0x2000),
        },
        "resource_slot": f"{resource_slot:#010x}",
        "scene_object": capture_scene_object(emulator, read_u32(emulator, address)),
    }
    if slot_kind == "party":
        actor["party_state_flags"] = f"{read_u16(emulator, address + 0x74):#06x}"
        formation_index = read_u16(emulator, address + 0x7E)
        member_name, formation_name = BATTLE_PARTY_FORMATIONS.get(
            formation_index, (label, f"unknown {formation_index}")
        )
        actor["slot_label"] = label
        actor["label"] = member_name
        actor["formation_index"] = formation_index
        actor["formation_name"] = formation_name
        actor["linked_object_id"] = read_u16(emulator, address + 0x80)
    else:
        actor["defeat_effect_kind"] = read_s16(emulator, address + 0x298)
        enemy_state_flags = read_u32(emulator, address + 0x29C)
        actor["enemy_state_flags"] = f"{enemy_state_flags:#010x}"
        actor["decoded_enemy_state_flags"] = {
            "item_drop_processed": bool(enemy_state_flags & 0x01),
            "flag_01": bool(enemy_state_flags & 0x02),
            "flag_02": bool(enemy_state_flags & 0x04),
            "flag_03": bool(enemy_state_flags & 0x08),
            "damage_immune": bool(enemy_state_flags & 0x10),
            "flag_05": bool(enemy_state_flags & 0x20),
            "traits": (enemy_state_flags >> 6) & 0x03,
        }
        stats = capture_enemy_stats(emulator, resource_slot, name_hints)
        actor["configured_stats"] = stats
        if stats is not None:
            if stats["name_hint"] is not None:
                actor["label"] = stats["name_hint"]
            actor["max_hp_delta_from_configured"] = actor["max_hp"] - stats["max_hp"]
    return actor


def capture_battle_state(emulator: DeSmuME, enemy_stats_path: Path) -> dict[str, Any]:
    battle_system = read_u32(emulator, BATTLE_SYSTEM_GLOBAL)
    context = read_u32(emulator, BATTLE_CONTEXT_GLOBAL)
    result: dict[str, Any] = {
        "battle_system": f"{battle_system:#010x}",
        "context": f"{context:#010x}",
        "valid_context": is_main_ram_pointer(context, BATTLE_CONTEXT_SIZE),
    }
    if not result["valid_context"]:
        return result

    name_hints = load_enemy_name_hints(enemy_stats_path)
    actors = []
    for slot_index, name in enumerate(BATTLE_PARTY_SLOT_NAMES):
        actor_id = 56 + slot_index
        address = read_u32(
            emulator, context + BATTLE_PARTY_ACTOR_TABLE_OFFSET + slot_index * 4
        )
        actors.append(
            capture_battle_actor(
                emulator, address, actor_id, "party", slot_index, name, name_hints
            )
        )
    for slot_index in range(8):
        actor_id = 60 + slot_index
        address = read_u32(
            emulator, context + BATTLE_ENEMY_ACTOR_TABLE_OFFSET + slot_index * 4
        )
        actors.append(
            capture_battle_actor(
                emulator,
                address,
                actor_id,
                "enemy",
                slot_index,
                f"Enemy slot {slot_index}",
                name_hints,
            )
        )

    result.update(
        {
            "background_id": read_u16(emulator, context + BATTLE_BACKGROUND_ID_OFFSET),
            "ai_control_mask": f"{read_u16(emulator, context + BATTLE_AI_CONTROL_MASK_OFFSET):#06x}",
            "runtime_flags": f"{read_u32(emulator, context + BATTLE_RUNTIME_FLAGS_OFFSET):#010x}",
            "actors": actors,
        }
    )
    return result


def decode_hook_arguments(emulator: DeSmuME, label: str) -> dict[str, Any] | None:
    registers = emulator.memory.register_arm9
    r0 = registers.r0 & 0xFFFFFFFF
    r1 = registers.r1 & 0xFFFFFFFF
    r2 = registers.r2 & 0xFFFFFFFF
    r3 = registers.r3 & 0xFFFFFFFF

    if label == "FieldGeometry_GetVectorLength":
        return {"vector_q12": [to_s32(r0), to_s32(r1), to_s32(r2)]}
    if label == "FieldGeometry_GetOrbitLength":
        return {"radii_q12": [to_s32(r0), to_s32(r1)]}

    if label.startswith(("FieldOrbit_", "FieldOrbit3D_")):
        result = {"entity": f"{r0:#010x}"}
        if label == "FieldOrbit_AdjustRadius":
            result.update(radius_q12=to_s32(r1), angle=r2 & 0xFFFF, scale_q12=to_s32(r3))
            return result
        if label == "FieldOrbit3D_CalculatePosition":
            stack = registers.sp & 0xFFFFFFFF
            pointer = read_u32(emulator, stack) if is_arm9_work_ram_pointer(stack, 4) else 0
        else:
            pointer = r3 if label == "FieldOrbit_CalculatePosition" else (r1 or r0 + 0x238)
        result["controller"] = f"{pointer:#010x}"
        if is_arm9_work_ram_pointer(pointer, 0x48):
            flags = read_u32(emulator, pointer)
            result.update(flags=f"{flags:#010x}", active=bool(flags & 1), paused=bool(flags & 2),
                          elapsed_frames=read_u32(emulator, pointer + 4),
                          speed_q12=read_s32(emulator, pointer + 8),
                          angle=read_s32(emulator, pointer + 0x20),
                          remaining_angle=read_s32(emulator, pointer + 0x24))
        return result

    if label in {"FieldEntity2D_InitPlacement", "FieldEntity2D_InitPlacementBase",
                 "FieldEntity3D_InitPlacement", "FieldEntity3D_InitPlacementBase"}:
        result = {"entity": f"{r0:#010x}", "entity_index": to_s32(r1), "spawn": f"{r2:#010x}"}
        if is_arm9_work_ram_pointer(r2, 28):
            result["cell"] = [read_s16(emulator, r2 + index * 2) for index in range(3)]
            result["pixel_offset"] = [read_s16(emulator, r2 + 6 + index * 2) for index in range(3)]
            result["animation_or_facing"] = f"{read_u16(emulator, r2 + 12):#06x}"
            result["appearance"] = f"{read_u16(emulator, r2 + 14):#06x}"
            result["spawn_flags"] = f"{read_u32(emulator, r2 + 24):#010x}"
        return result

    if label.startswith(("FieldEntity2D_", "FieldEntity3D_")):
        return {"entity": f"{r0:#010x}"}

    if label == "FieldEntity_SetBehaviorMode":
        return {"entity": f"{r0:#010x}", "requested_mode": to_s32(r1)}

    if label == "FieldEntity_SetInteractionBounds":
        result = {"entity": f"{r0:#010x}", "minimum_x": to_s32(r1),
                  "maximum_y": to_s32(r2), "width": r3 & 0xFFFF}
        stack = registers.sp & 0xFFFFFFFF
        if is_arm9_work_ram_pointer(stack, 8):
            result.update(height=read_u16(emulator, stack), vertical_extent=read_u16(emulator, stack + 4))
        return result

    if label == "FieldEntity_ResetLocomotionParameters" and is_main_ram_pointer(r0, 0x168):
        return {"entity": f"{r0:#010x}", "initial_locomotion_q12":
                [read_s32(emulator, r0 + 0x150 + index * 4) for index in range(6)]}

    if label in {"FieldEntity_SetLocomotionParameters", "FieldEntity_SetVerticalParameters"}:
        return {"entity": f"{r0:#010x}", "arguments_1_3": [to_s32(value) for value in (r1, r2, r3)]}

    if label.startswith("FieldBlink_") and is_main_ram_pointer(r0, 0x2B0):
        flags = read_u32(emulator, r0 + 0x184)
        frames = (flags >> 17) & 0xFF
        result = {"entity": f"{r0:#010x}", "mode": (flags >> 13) & 3,
                  "paused": bool(flags & 0x8000), "phase": (flags >> 16) & 1,
                  "frames": frames if frames < 128 else frames - 256,
                  "offset": flags >> 25,
                  "durations": f"{read_u32(emulator, r0 + 0x188):#010x}",
                  "first_phase": f"{read_u32(emulator, r0 + 0x18C):#010x}",
                  "second_phase": f"{read_u32(emulator, r0 + 0x190):#010x}",
                  "renderer": f"{read_u32(emulator, r0 + 0x1E0):#010x}"}
        if label in {"FieldBlink_Start", "FieldBlink_StartRenderers", "FieldBlink_Stop"}:
            result["requested_mode"] = to_s32(r1)
        return result

    if label in {"FieldDeferredEntity_Init", "FieldDeferredEntity_InitEmpty"}:
        result = {"entity": f"{r0:#010x}"}
        if label == "FieldDeferredEntity_Init":
            result.update(entity_index=to_s32(r1), spawn_record=f"{r2:#010x}")
        return result

    if label.startswith("FieldDeferredEntity_") and is_main_ram_pointer(r0, 0x524):
        control = read_u32(emulator, r0 + 0x520)
        result = {"entity": f"{r0:#010x}", "phase": (control >> 4) & 3,
                  "pause_frames": (control >> 6) & 0xFFFF,
                  "blink_mode": (read_u32(emulator, r0 + 0x184) >> 13) & 3,
                  "property_flag_00": bool(read_u16(emulator, r0 + 10) & 1)}
        if label == "FieldDeferredEntity_CopyState":
            result["source"] = f"{r1:#010x}"
        elif label == "FieldDeferredEntity_StopScript":
            result["deferred"] = to_s32(r1)
        return result

    if label == "FieldRenderList_Clear":
        return {"screen": to_s32(r0)}

    if label in {"FieldAnimationRenderer_Init", "FieldAnimationRenderer_InitBase",
                 "FieldAnimationRenderer_DestroyBase", "FieldTimedRenderer_DestroyBase"}:
        return {"renderer": f"{r0:#010x}"}

    if label == "FieldAnimationRenderer_GetOverlapPriority" and is_main_ram_pointer(r0, 0x138) and is_arm9_work_ram_pointer(r1, 10):
        mode = read_u16(emulator, r1 + 8) >> 14
        return {"renderer": f"{r0:#010x}", "mode": mode,
                "priority": read_u8(emulator, r0 + 0x134 + mode)}

    if label == "FieldAnimationRenderer_RestoreController" and is_arm9_work_ram_pointer(r1, 0x60):
        return {"renderer": f"{r0:#010x}", "descriptor": f"{r1:#010x}",
                "controller": f"{r2:#010x}", "animation": to_s32(r3),
                "requested_overlap_priority": read_u32(emulator, r1 + 0x54) & 3}

    if label.startswith("FieldTimedRenderer_") and is_main_ram_pointer(r0, 0x13C):
        flags = read_u32(emulator, r0 + 0x7C)
        control = read_u16(emulator, r0 + 0x138)
        result = {
            "renderer": f"{r0:#010x}",
            "resource_animation": read_s16(emulator, r0 + 0x54),
            "animation": read_s16(emulator, r0 + 0x56),
            "speed": read_s16(emulator, r0 + 0x5A),
            "animation_active": bool(flags & 0x100),
            "animation_suppressed": bool(flags & 0x200),
            "behavior_state": (flags >> 12) & 15,
            "finished": bool(control & 16),
            "loops_remaining": read_s16(emulator, r0 + 0x138) >> 5,
        }
        if label == "FieldTimedRenderer_Update" and is_main_ram_pointer(r0, 0x140):
            result["expired"] = bool(read_u32(emulator, r0 + 0x13C) & 0x8000)
            result["remaining"] = read_u16(emulator, r0 + 0x13E)
        if label == "FieldTimedRenderer_SetLoopCount":
            result["requested_loop_count"] = to_s32(r1)
        return result

    if label.startswith("FieldTimer_") and is_main_ram_pointer(r0, 40):
        flags = read_u8(emulator, r0)
        result = {
            "timer": f"{r0:#010x}",
            "screen": flags & 1,
            "allocated": bool(flags & 2),
            "paused": bool(flags & 4),
            "clock": {
                name: int(emulator.memory.signed.read_byte(r0 + offset))
                for offset, name in enumerate(
                    ("step", "minutes", "seconds", "frames", "hundredths"), start=1
                )
            },
            "position": [read_s16(emulator, r0 + 6), read_s16(emulator, r0 + 8)],
            "image": f"{read_u32(emulator, r0 + 12):#010x}",
            "allocation_offset": read_u32(emulator, r0 + 16),
        }
        if label == "FieldTimer_SetValue":
            result["requested_clock"] = [to_s32(value) for value in (r1, r2, r3)]
        elif label == "FieldTimer_SetVisible":
            result["requested_visible"] = r1 & 0xFF
        elif label == "FieldTimer_SetPosition":
            result["requested_position"] = [to_s32(value) for value in (r1, r2, r3)]
        elif label == "FieldTimer_SetImage":
            result["requested_image"] = f"{r1:#010x}"
        return result

    if label == "GameGraphics_SetOrthographicProjection":
        return {"near_plane_q12": to_s32(r0), "far_plane_q12": to_s32(r1)}

    if label == "GameSpriteWindow_OpenWithProperties" and is_main_ram_pointer(r1, 28):
        shape = read_u32(emulator, r1)
        return {
            "manager": f"{r0:#010x}",
            "properties": f"{r1:#010x}",
            "screen": shape & 1,
            "skin": (shape >> 1) & 15,
            "position_mode": (shape >> 17) & 7,
            "requested_index": to_s32(r2),
        }

    if label in {"FieldBackground_ActivateTilemapPatch", "func_020131b0"} and is_main_ram_pointer(r0, 0x790):
        patches = read_u32(emulator, r0 + 0xAC)
        patch = patches + to_s32(r1) * 20
        if is_main_ram_pointer(patch, 20):
            save = read_u32(emulator, 0x02059FE8)
            flags = [read_u16(emulator, patch + offset) for offset in (16, 18)]
            flag_values = [
                bool(read_u32(emulator, save + 0x50 + (flag >> 5) * 4) & (1 << (flag & 31)))
                if flag != 0x7FFF and is_main_ram_pointer(save, 0x50 + (flag >> 5) * 4 + 4)
                else None for flag in flags
            ]
            return {
                "background": f"{r0:#010x}",
                "index": to_s32(r1),
                "tiles": f"{read_u32(emulator, patch):#010x}",
                "unknown_04": f"{read_u32(emulator, patch + 4):#010x}",
                "rectangle": [read_u16(emulator, patch + offset) for offset in (8, 10, 12, 14)],
                "set_flag": flags[0],
                "clear_flag": flags[1],
                "save_flag_values": flag_values,
            }

    if label == "FieldBackground_SetOrigin" and is_main_ram_pointer(r0, 0x754):
        configuration = read_u32(emulator, r0 + 0x634)
        return {
            "background": f"{r0:#010x}",
            "requested_origin": [to_s32(r1), to_s32(r2)],
            "previous_origin": [to_s32(read_u32(emulator, r0 + offset))
                                for offset in (0x74C, 0x750)],
            "relative_scroll_layers": (read_u32(emulator, configuration + 8) >> 5) & 7
            if is_main_ram_pointer(configuration, 12) else None,
            "scroll_x_q8": [to_s32(read_u32(emulator, r0 + 0x734 + i * 4)) for i in range(3)],
            "scroll_y_q8": [to_s32(read_u32(emulator, r0 + 0x740 + i * 4)) for i in range(3)],
        }

    if label in {
        "BattleRenderModel_UpdateTexture", "BattleRenderModel_SetTextureDirty",
    } and is_main_ram_pointer(r0, 0x1B8):
        offset = read_u32(emulator, r0 + 0x130)
        state = read_u32(emulator, r0 + 0x144)
        return {
            "model": f"{r0:#010x}",
            "texture_offset": f"{offset:#010x}",
            "texture_bank": offset >> 17,
            "offset_within_bank": offset & 0x1FFFF,
            "texture_dirty": bool(state & 4),
            "texture_state": f"{state:#010x}",
            "resource": f"{read_u32(emulator, r0 + 0x38):#010x}",
            "pixels": f"{read_u32(emulator, r0 + 0x30):#010x}",
            "palette_source": f"{read_u32(emulator, r0 + 0x158):#010x}",
            "palette_bytes": read_u16(emulator, r0 + 0x15C),
            "palette_state": f"{read_u16(emulator, r0 + 0x15E):#06x}",
        }

    if label in {
        "BattleRenderModel_Init", "BattleModelController_Init",
        "BattleModelController_InitBase", "BattleModel_InitResourceState",
    }:
        # Entry hooks run before the constructor has initialized its storage.
        return {"model": f"{r0:#010x}", "initializing": True}

    if label in {
        "BattleModelController_Reset", "BattleModelController_Configure",
        "BattleModelController_Restore", "BattleModelController_DestroyBase",
        "BattleRenderModel_Delete", "BattleModel_ReleaseTexture",
        "BattleModel_CopyAnimationLayers", "BattleModelController_SetLayerAnimation",
        "BattleModelController_SetLayerSpeed", "BattleModelController_SetLayerMode",
        "BattleModelController_SetLayerState", "BattleModelController_SetPrimarySpeed",
        "BattleModelController_SetPrimaryMode", "BattleModelController_SetPrimaryState",
        "BattleModelController_SetPrimaryAnimation",
    } and is_main_ram_pointer(r0, 0x12C):
        return {
            "model": f"{r0:#010x}",
            "screen": read_u8(emulator, r0 + 0x10),
            "flags": f"{read_u32(emulator, r0 + 0x7C):#010x}",
            "controller": f"{read_u32(emulator, r0 + 0x84):#010x}",
            "primary_state": read_u8(emulator, r0 + 0xB8),
            "primary_mode": read_u8(emulator, r0 + 0xB9),
            "primary_track": f"{read_u32(emulator, r0 + 0xBC):#010x}",
            "layer_states": [read_u8(emulator, r0 + 0xF0 + i) for i in range(8)],
            "layer_modes": [read_u8(emulator, r0 + 0xF8 + i) for i in range(8)],
            "layer_tracks": [f"{read_u32(emulator, r0 + 0x100 + 4 * i):#010x}" for i in range(8)],
            "layer_animation_ids": [read_u8(emulator, r0 + 0x120 + i) for i in range(8)],
        }

    if label in {
        "FieldBackground_UpdateTransfers", "FieldBackground_InitTransfers",
        "FieldBackground_DestroyTransfers", "FieldBackground_DeleteTransfers",
    } and is_main_ram_pointer(r0, 0x100):
        initializing = label == "FieldBackground_InitTransfers"
        background = r3 if initializing else read_u32(emulator, r0 + 4)
        return {
            "transfers": f"{r0:#010x}",
            "background": f"{background:#010x}",
            "initializing": initializing,
            "priority_argument": r1 if initializing else None,
            "state": None if initializing else read_u32(emulator, r0 + 0x28),
            "queued_transfers": None if initializing else read_u8(emulator, r0 + 0xFC),
            "dirty_flags": None if initializing else read_u8(emulator, r0 + 0xFD),
            "waiting_vblank": bool(read_u16(emulator, 0x02060B40) & 0x100),
            "background_load_status": read_u8(emulator, background + 0x28)
            if is_main_ram_pointer(background, 0x29) else None,
        }

    if label in {
        "FieldBackground_GetTileBytes", "FieldBackground_AllocatePalettes",
        "FieldBackground_UploadAllPalettes", "FieldBackground_RestorePalettes",
        "FieldBackground_InitializePaletteAnimation", "FieldBackground_UpdatePaletteAnimations",
        "FieldBackground_HasPaletteAnimation", "FieldBackground_GetPaletteAnimation",
        "FieldBackground_UpdateBlendAnimation", "FieldBackground_SetPaletteEffects",
        "FieldBackground_ConfigurePaletteEffect", "FieldBackground_ResetPaletteEffect",
        "FieldBackground_RestoreAndUploadPalette", "FieldBackground_UploadBasePalette",
        "FieldBackground_IsReady",
    } and is_main_ram_pointer(r0, 0x78D):
        screen = read_u8(emulator, r0 + 0x782)
        track_count = read_u8(emulator, r0 + 0x78A) + 1
        states = read_u32(emulator, r0 + 0x75C)
        times = read_u32(emulator, r0 + 0x758)
        return {
            "background": f"{r0:#010x}",
            "load_status": read_u8(emulator, r0 + 0x28),
            "screen": screen,
            "color256_layers": read_u8(emulator, r0 + 0x783),
            "palette_state": read_u8(emulator, r0 + 0x78C),
            "palette_animation": {
                "table": f"{read_u32(emulator, r0 + 0x644):#010x}",
                "track_count_including_default": track_count,
                "states": [read_u8(emulator, states + i) for i in range(track_count)]
                if is_main_ram_pointer(states, track_count) else None,
                "times_q8": [to_s32(read_u32(emulator, times + 4 * i)) for i in range(track_count)]
                if is_main_ram_pointer(times, 4 * track_count) else None,
                "speed_q8": to_s32(read_u32(emulator, r0 + 0x760)),
                "components": [f"{read_u32(emulator, r0 + 0x9C + 4 * i):#010x}" for i in range(3)],
            },
            "palette_effect_count": read_u8(emulator, r0 + 0x78B),
            "blend_animation": {
                "resource": f"{read_u32(emulator, r0 + 0x728):#010x}",
                "remaining_q8": to_s32(read_u32(emulator, r0 + 0x72C)),
                "frame": read_u16(emulator, r0 + 0x730),
            },
            "bg_control_words": [
                f"{read_u16(emulator, 0x0400000A + screen * 0x1000 + layer * 2):#06x}"
                for layer in range(3)
            ] if screen in (0, 1) else None,
            "palette_sources": [
                {
                    "bytes": read_u32(emulator, r0 + 0x618 + 8 * layer),
                    "colors": f"{read_u32(emulator, r0 + 0x61C + 8 * layer):#010x}",
                    "workspace": f"{read_u32(emulator, r0 + 0x80 + 4 * layer):#010x}",
                }
                for layer in range(3)
            ],
        }

    if label == "VM_WriteVariable":
        return {
            "variable": f"{r0 & 0xFFFF:#06x}",
            "value": to_s32(r1),
            "vm": f"{r2:#010x}",
            "state": f"{r3:#010x}",
            "state_script": f"{read_u32(emulator, r3):#010x}"
            if is_main_ram_pointer(r3, 4)
            else None,
        }
    if label == "BattleScript_SetProperty":
        return {"actor_id": r0 & 0xFFFF, "property": to_s32(r1), "value": to_s32(r2)}
    if label == "BattleAI_StartReactionScript":
        return {"actor_id": r0 & 0xFFFF}
    if label == "BattleActor_ApplyDamage":
        return {
            "scene_object": f"{r0:#010x}",
            "actor_id": read_u16(emulator, r0 + 0xEC)
            if is_main_ram_pointer(r0, 0xEE)
            else None,
            "damage": to_s32(r1),
        }
    if label == "BattleDamage_ApplyToEnemy":
        return {
            "scene_object": f"{r0:#010x}",
            "actor_id": read_u16(emulator, r0 + 0xEC)
            if is_main_ram_pointer(r0, 0xEE)
            else None,
            "popup_offset_x": to_s32(r1),
            "popup_offset_y": to_s32(r2),
            "damage": to_s32(r3),
        }
    return None


def read_registers(emulator: DeSmuME, processor: str) -> dict[str, str]:
    registers = getattr(emulator.memory, f"register_{processor}")
    result = {f"r{index}": f"{getattr(registers, f'r{index}') & 0xFFFFFFFF:#010x}" for index in range(16)}
    result.update(
        {
            "sp": f"{registers.sp & 0xFFFFFFFF:#010x}",
            "lr": f"{registers.lr & 0xFFFFFFFF:#010x}",
            "pc": f"{registers.pc & 0xFFFFFFFF:#010x}",
            "cpsr": f"{registers.cpsr & 0xFFFFFFFF:#010x}",
            "spsr": f"{registers.spsr & 0xFFFFFFFF:#010x}",
        }
    )
    return result


def load_function_symbols(symbol_root: Path) -> dict[str, list[dict[str, Any]]]:
    symbols: dict[str, list[dict[str, Any]]] = {}
    for path in sorted(symbol_root.rglob("symbols.txt")):
        module = path.parent.name
        for line in path.read_text(encoding="utf-8").splitlines():
            match = SYMBOL_PATTERN.match(line)
            if match is None:
                continue
            name, address_text = match.groups()
            symbols.setdefault(name, []).append(
                {
                    "address": int(address_text, 16),
                    "module": module,
                    "source": str(path.relative_to(PROJECT_ROOT)).replace("\\", "/"),
                }
            )
    return symbols


def resolve_function(value: str, symbols: dict[str, list[dict[str, Any]]]) -> tuple[str, int]:
    try:
        return value, parse_int(value)
    except ValueError:
        pass
    matches = symbols.get(value, [])
    if not matches:
        raise ValueError(f"unknown function symbol: {value}")
    addresses = {entry["address"] for entry in matches}
    if len(addresses) != 1:
        locations = ", ".join(f"{entry['module']}:{entry['address']:#010x}" for entry in matches)
        raise ValueError(f"ambiguous function symbol {value}: {locations}")
    return value, matches[0]["address"]


def compare_overlays(
    emulator: DeSmuME, overlay_config: Path, overlay_directory: Path
) -> list[dict[str, Any]]:
    document = yaml.safe_load(overlay_config.read_text(encoding="utf-8"))
    overlays = document["overlays"]
    groups: dict[int, list[dict[str, Any]]] = {}
    for overlay in overlays:
        if overlay["code_size"] >= 64:
            groups.setdefault(overlay["base_address"], []).append(overlay)

    results = []
    for base_address, candidates in sorted(groups.items()):
        maximum_size = max(candidate["code_size"] for candidate in candidates)
        runtime = read_bytes(emulator, base_address, base_address + maximum_size)
        scored = []
        for candidate in candidates:
            expected = (overlay_directory / candidate["file_name"]).read_bytes()
            size = min(candidate["code_size"], len(expected))
            actual = runtime[:size]
            matching = sum(left == right for left, right in zip(actual, expected[:size]))
            prefix = 0
            for left, right in zip(actual, expected[:size]):
                if left != right:
                    break
                prefix += 1
            scored.append(
                {
                    "id": candidate["id"],
                    "file": candidate["file_name"],
                    "base_address": f"{base_address:#010x}",
                    "code_size": size,
                    "matching_bytes": matching,
                    "match_percent": round(100.0 * matching / size, 4),
                    "matching_prefix_bytes": prefix,
                }
            )
        scored.sort(key=lambda item: (item["match_percent"], item["matching_prefix_bytes"]), reverse=True)
        best = scored[0]
        active = best["match_percent"] >= 90.0
        results.append(
            {
                "base_address": f"{base_address:#010x}",
                "active": active,
                "active_overlay_id": best["id"] if active else None,
                "candidates": scored,
            }
        )
    return results


def is_overlay_active(matches: list[dict[str, Any]], overlay_id: int) -> bool:
    return any(
        group["active"] and group["active_overlay_id"] == overlay_id
        for group in matches
    )


def capture_model_render_lists(emulator: DeSmuME) -> list[dict[str, Any]]:
    """Read the two resident lists and check the recovered link fields."""
    lists = []
    for screen in range(2):
        head = read_u32(emulator, 0x0205A89C + 4 * screen)
        tail = read_u32(emulator, 0x0205A8A4 + 4 * screen)
        address = head
        previous = 0
        visited: set[int] = set()
        models = []
        errors = []
        while address:
            if not is_main_ram_pointer(address, 0x80) or address in visited:
                errors.append(f"invalid or cyclic node {address:#010x}")
                break
            visited.add(address)
            node_previous = read_u32(emulator, address + 4)
            next_address = read_u32(emulator, address + 8)
            node_screen = read_u8(emulator, address + 0x10)
            flags = read_u32(emulator, address + 0x7C)
            if node_previous != previous or node_screen != screen or not flags & 0x10:
                errors.append(f"link, screen or membership mismatch at {address:#010x}")
            models.append({
                "address": f"{address:#010x}",
                "vtable": f"{read_u32(emulator, address):#010x}",
                "previous": f"{node_previous:#010x}",
                "next": f"{next_address:#010x}",
                "screen": node_screen,
                "flags": f"{flags:#010x}",
            })
            previous, address = address, next_address
        if previous != tail:
            errors.append("last visited node does not equal the tail")
        lists.append({
            "screen": screen, "head": f"{head:#010x}", "tail": f"{tail:#010x}",
            "models": models, "errors": errors,
        })
    return lists


def capture_domain_state(
    emulator: DeSmuME,
    overlay_matches: list[dict[str, Any]],
    enemy_stats_path: Path,
) -> dict[str, Any]:
    render_lists = capture_model_render_lists(emulator)
    if not is_overlay_active(overlay_matches, 2):
        return {
            "model_render_lists": render_lists,
            "battle": {
                "available": False,
                "reason": "European battle overlay 2 is not active",
            }
        }
    battle = capture_battle_state(emulator, enemy_stats_path)
    battle["available"] = battle["valid_context"]
    if not battle["valid_context"]:
        battle["reason"] = "overlay 2 is active but gBattleContext is not valid"
    return {"battle": battle, "model_render_lists": render_lists}


def changed_runs(before: bytes, after: bytes, base_address: int) -> list[dict[str, Any]]:
    runs = []
    cursor = 0
    while cursor < len(before):
        if before[cursor] == after[cursor]:
            cursor += 1
            continue
        start = cursor
        while cursor < len(before) and before[cursor] != after[cursor]:
            cursor += 1
        end = cursor
        runs.append(
            {
                "start": f"{base_address + start:#010x}",
                "end": f"{base_address + end:#010x}",
                "size": end - start,
                "before": before[start : min(end, start + 32)].hex(),
                "after": after[start : min(end, start + 32)].hex(),
                "sample_truncated": end - start > 32,
            }
        )
    return runs


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, required=True, help="legally obtained PiT ROM or rebuilt ROM")
    parser.add_argument("--battery-save", type=Path, help="raw .sav battery save to import before execution")
    parser.add_argument("--state", type=Path, help="DeSmuME .dst savestate to load")
    parser.add_argument("--save-state", type=Path, help="optional compatible state after the captured actions")
    parser.add_argument("--output", type=Path, required=True, help="ignored output directory for evidence")
    parser.add_argument("--frames", type=int, default=0, help="frames to execute after loading the state")
    parser.add_argument(
        "--action",
        type=parse_action,
        action="append",
        default=[],
        metavar="KEY[:FRAMES]",
        help="ordered DS button, chord (select+a), or wait (repeatable; cannot be combined with --frames)",
    )
    parser.add_argument(
        "--allow-state-advance",
        action="store_true",
        help="confirm that a loaded state is compatible before advancing it",
    )
    parser.add_argument("--exec", dest="exec_hooks", action="append", default=[], help="function symbol or address")
    parser.add_argument(
        "--watch-write", type=parse_watch, action="append", default=[], metavar="ADDRESS[:SIZE]"
    )
    parser.add_argument(
        "--diff-range", type=parse_memory_range, action="append", default=[], metavar="[NAME=]START:END"
    )
    parser.add_argument("--overlay-config", type=Path, default=DEFAULT_OVERLAY_CONFIG)
    parser.add_argument("--overlay-directory", type=Path, default=DEFAULT_OVERLAY_DIRECTORY)
    parser.add_argument("--symbol-root", type=Path, default=DEFAULT_SYMBOL_ROOT)
    parser.add_argument("--enemy-stats", type=Path, default=DEFAULT_ENEMY_STATS)
    parser.add_argument("--no-screenshot", action="store_true")
    return parser


def main() -> int:
    args = build_argument_parser().parse_args()
    if args.frames < 0:
        raise SystemExit("--frames must not be negative")
    if args.frames and args.action:
        raise SystemExit("--frames and --action cannot be combined; use --action wait:FRAMES")
    if len({region.name for region in args.diff_range}) != len(args.diff_range):
        raise SystemExit("--diff-range labels must be unique")
    if (args.frames or args.action) and args.state is not None and not args.allow_state_advance:
        raise SystemExit(
            "refusing to advance a loaded savestate without --allow-state-advance; "
            "cross-version DeSmuME states can hang inside one frame"
        )
    for required in [
        args.rom,
        args.overlay_config,
        args.overlay_directory,
        args.symbol_root,
        args.enemy_stats,
    ]:
        if not required.exists():
            raise SystemExit(f"missing required path: {required}")
    if args.battery_save is not None and not args.battery_save.exists():
        raise SystemExit(f"missing battery save: {args.battery_save}")
    if args.state is not None and not args.state.exists():
        raise SystemExit(f"missing savestate: {args.state}")
    input_state = None if args.state is None else {
        "path": str(args.state.resolve()), "sha1": sha1_file(args.state)
    }

    args.output.mkdir(parents=True, exist_ok=True)
    emulator = DeSmuME()
    events: list[dict[str, Any]] = []
    before_ranges: dict[str, bytes] = {}

    try:
        emulator.open(str(args.rom.resolve()), auto_resume=False)
        if args.battery_save is not None:
            if not emulator.backup.import_file(
                str(args.battery_save.resolve()), force_size=args.battery_save.stat().st_size
            ):
                raise RuntimeError(f"failed to import battery save: {args.battery_save}")
        if args.state is not None:
            emulator.savestate.load_file(str(args.state.resolve()))
        emulator.pause()

        symbols = load_function_symbols(args.symbol_root)

        def make_callback(kind: str, label: str) -> Callable[[int, int], None]:
            def callback(address: int, size: int) -> None:
                event = {
                    "frame": current_frame[0],
                    "kind": kind,
                    "label": label,
                    "address": f"{address:#010x}",
                    "size": size,
                    "arm9": read_registers(emulator, "arm9"),
                }
                decoded_arguments = decode_hook_arguments(emulator, label)
                if decoded_arguments is not None:
                    event["decoded_arguments"] = decoded_arguments
                events.append(event)

            return callback

        for value in args.exec_hooks:
            label, address = resolve_function(value, symbols)
            emulator.memory.register_exec(address, make_callback("exec", label))
        for address, size in args.watch_write:
            emulator.memory.register_write(
                address, make_callback("write", f"{address:#010x}:{size}"), size=size
            )

        for memory_range in args.diff_range:
            before = read_bytes(emulator, memory_range.start, memory_range.end)
            before_ranges[memory_range.name] = before
            (args.output / f"{memory_range.name}.before.bin").write_bytes(before)

        initial_registers = {
            "arm9": read_registers(emulator, "arm9"),
            "arm7": read_registers(emulator, "arm7"),
            "arm9_next_instruction": f"{emulator.memory.get_next_instruction() & 0xFFFFFFFF:#010x}",
        }
        initial_overlay_matches = compare_overlays(
            emulator, args.overlay_config, args.overlay_directory
        )
        initial_domain_state = capture_domain_state(
            emulator, initial_overlay_matches, args.enemy_stats
        )

        current_frame = [0]
        executed_frames = 0
        if args.frames or args.action:
            emulator.resume()
            if args.action:
                for name, frames in args.action:
                    emulator.input.keypad_update(action_mask(name))
                    for _ in range(frames):
                        current_frame[0] = executed_frames
                        emulator.cycle(with_joystick=False)
                        executed_frames += 1
                    emulator.input.keypad_update(0)
                    current_frame[0] = executed_frames
                    emulator.cycle(with_joystick=False)
                    executed_frames += 1
            else:
                for frame in range(args.frames):
                    current_frame[0] = frame
                    emulator.cycle(with_joystick=False)
                    executed_frames += 1
            emulator.pause()

        range_diffs = []
        for memory_range in args.diff_range:
            after = read_bytes(emulator, memory_range.start, memory_range.end)
            (args.output / f"{memory_range.name}.after.bin").write_bytes(after)
            changes = changed_runs(before_ranges[memory_range.name], after, memory_range.start)
            range_diffs.append(
                {
                    "name": memory_range.name,
                    "start": f"{memory_range.start:#010x}",
                    "end": f"{memory_range.end:#010x}",
                    "changed_bytes": sum(run["size"] for run in changes),
                    "changed_runs": changes,
                }
            )

        if not args.no_screenshot:
            emulator.screenshot().save(args.output / "screens.png")

        saved_state = None
        if args.save_state is not None:
            args.save_state.parent.mkdir(parents=True, exist_ok=True)
            emulator.savestate.save_file(str(args.save_state.resolve()))
            saved_state = {"path": str(args.save_state.resolve()), "sha1": sha1_file(args.save_state)}

        final_overlay_matches = compare_overlays(
            emulator, args.overlay_config, args.overlay_directory
        )
        final_domain_state = capture_domain_state(
            emulator, final_overlay_matches, args.enemy_stats
        )
        evidence = {
            "format_version": 2,
            "captured_at_utc": datetime.now(timezone.utc).isoformat(),
            "toolchain": {
                "py_desmume": package_version("py-desmume"),
                "embedded_desmume": "0.9.12",
            },
            "rom": {"path": str(args.rom.resolve()), "sha1": sha1_file(args.rom)},
            "battery_save": None
            if args.battery_save is None
            else {
                "path": str(args.battery_save.resolve()),
                "sha1": sha1_file(args.battery_save),
            },
            "savestate": input_state,
            "frames_executed": executed_frames,
            "saved_state": saved_state,
            "memory_view": "ARM9 CPU address space; VRAM uses the current bank mapping",
            "input_actions": [
                {"input": name, "frames": frames} for name, frames in args.action
            ],
            "initial_registers": initial_registers,
            "final_registers": {
                "arm9": read_registers(emulator, "arm9"),
                "arm7": read_registers(emulator, "arm7"),
                "arm9_next_instruction": f"{emulator.memory.get_next_instruction() & 0xFFFFFFFF:#010x}",
            },
            "gpu_layers": {
                "main": [bool(emulator.gpu_get_layer_main_enable_state(index)) for index in range(5)],
                "sub": [bool(emulator.gpu_get_layer_sub_enable_state(index)) for index in range(5)],
            },
            "initial_overlay_matches": initial_overlay_matches,
            "final_overlay_matches": final_overlay_matches,
            "initial_domain_state": initial_domain_state,
            "final_domain_state": final_domain_state,
            "events": events,
            "range_diffs": range_diffs,
        }
        (args.output / "evidence.json").write_text(
            json.dumps(evidence, indent=2) + "\n", encoding="utf-8"
        )
        print(f"wrote {args.output / 'evidence.json'}")
        print(f"captured {len(events)} hook events across {executed_frames} frames")
        for group in evidence["final_overlay_matches"]:
            best = group["candidates"][0]
            if group["active"]:
                print(
                    f"overlay slot {group['base_address']}: active ov{best['id']:03d} "
                    f"({best['match_percent']:.2f}% bytes)"
                )
            else:
                print(
                    f"overlay slot {group['base_address']}: no matching overlay "
                    f"(best candidate ov{best['id']:03d}, {best['match_percent']:.2f}%)"
                )
        battle = evidence["final_domain_state"]["battle"]
        if battle["available"]:
            for actor in battle["actors"]:
                if actor.get("active"):
                    print(
                        f"actor {actor['actor_id']} {actor['label']}: "
                        f"HP {actor['current_hp']}/{actor['max_hp']}, "
                        f"POW {actor['power']}, DEF {actor['defense']}, "
                        f"SPEED {actor['speed']}"
                    )
    finally:
        emulator.destroy()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
