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


PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_OVERLAY_CONFIG = PROJECT_ROOT / "extract/eur/arm9_overlays/overlays.yaml"
DEFAULT_OVERLAY_DIRECTORY = PROJECT_ROOT / "extract/eur/arm9_overlays"
DEFAULT_SYMBOL_ROOT = PROJECT_ROOT / "config/eur/arm9"
DEFAULT_ENEMY_STATS = PROJECT_ROOT / "data/eur/stats/enemies.json"
MAIN_RAM_START = 0x02000000
MAIN_RAM_END = 0x02400000
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
BATTLE_PARTY_NAMES = ("Mario", "Luigi", "Baby Mario", "Baby Luigi")


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
    if not name or start >= end:
        raise argparse.ArgumentTypeError("range name must be non-empty and START must be below END")
    if start < MAIN_RAM_START or end > MAIN_RAM_END:
        raise argparse.ArgumentTypeError("runtime dumps are limited to 0x02000000:0x02400000")
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


def is_main_ram_pointer(address: int, size: int = 1) -> bool:
    return MAIN_RAM_START <= address and address + size <= MAIN_RAM_END


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
        actor["formation_index"] = read_u16(emulator, address + 0x7E)
        actor["linked_object_id"] = read_u16(emulator, address + 0x80)
    else:
        actor["defeat_effect_kind"] = read_s16(emulator, address + 0x298)
        actor["enemy_state_flags"] = f"{read_u32(emulator, address + 0x29C):#010x}"
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
    for slot_index, name in enumerate(BATTLE_PARTY_NAMES):
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


def capture_domain_state(
    emulator: DeSmuME,
    overlay_matches: list[dict[str, Any]],
    enemy_stats_path: Path,
) -> dict[str, Any]:
    if not is_overlay_active(overlay_matches, 2):
        return {
            "battle": {
                "available": False,
                "reason": "European battle overlay 2 is not active",
            }
        }
    battle = capture_battle_state(emulator, enemy_stats_path)
    battle["available"] = battle["valid_context"]
    if not battle["valid_context"]:
        battle["reason"] = "overlay 2 is active but gBattleContext is not valid"
    return {"battle": battle}


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
    parser.add_argument("--output", type=Path, required=True, help="ignored output directory for evidence")
    parser.add_argument("--frames", type=int, default=0, help="frames to execute after loading the state")
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
    if args.frames and args.state is not None and not args.allow_state_advance:
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

    args.output.mkdir(parents=True, exist_ok=True)
    emulator = DeSmuME()
    events: list[dict[str, Any]] = []
    before_ranges: dict[str, bytes] = {}

    try:
        emulator.open(str(args.rom.resolve()), auto_resume=False)
        if args.battery_save is not None:
            if not emulator.backup.import_file(str(args.battery_save.resolve())):
                raise RuntimeError(f"failed to import battery save: {args.battery_save}")
        if args.state is not None:
            emulator.savestate.load_file(str(args.state.resolve()))
        emulator.pause()

        symbols = load_function_symbols(args.symbol_root)

        def make_callback(kind: str, label: str) -> Callable[[int, int], None]:
            def callback(address: int, size: int) -> None:
                events.append(
                    {
                        "frame": current_frame[0],
                        "kind": kind,
                        "label": label,
                        "address": f"{address:#010x}",
                        "size": size,
                        "arm9": read_registers(emulator, "arm9"),
                    }
                )

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
        if args.frames:
            emulator.resume()
            for frame in range(args.frames):
                current_frame[0] = frame
                emulator.cycle(with_joystick=False)
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
            "savestate": None
            if args.state is None
            else {"path": str(args.state.resolve()), "sha1": sha1_file(args.state)},
            "frames_executed": args.frames,
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
        print(f"captured {len(events)} hook events across {args.frames} frames")
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
