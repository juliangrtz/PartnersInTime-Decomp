#!/usr/bin/env python3
"""Extract or verify the three PiT script-VM descriptor ABIs."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import struct
from typing import Any


ROOT = Path(__file__).resolve().parents[1]
SCHEMA = "pit-script-vm-descriptors-v1"
DESCRIPTOR_ENCODING = (
    "bits 0-4: argument count; bit 5: result variable; "
    "bit 6: argument-mode mask"
)
GENERIC_OPCODE_COUNT = 0x33

INSTANCES = {
    "field": {
        "overlay_id": 0,
        "role": "field/world events and actors",
        "load_address": 0x02065D40,
        "descriptor_table_address": 0x020C00D0,
        "descriptor_count": 0x155,
        "command_handler_address": 0x020823F8,
        "vm_run_call_sites": [0x02088030],
    },
    "battle": {
        "overlay_id": 2,
        "role": "battle scenarios and enemy AI",
        "load_address": 0x02065D40,
        "descriptor_table_address": 0x020BF2C8,
        "descriptor_count": 0x104,
        "command_handler_address": 0x02079950,
        "vm_run_call_sites": [
            0x0207E894,
            0x0207E8F8,
            0x0207E988,
            0x0207EA20,
            0x0207EF50,
            0x0207EF98,
            0x0207EFC4,
        ],
    },
    "scene": {
        "overlay_id": 7,
        "role": "scene/object scripts",
        "load_address": 0x0206AB80,
        "descriptor_table_address": 0x0208DCA4,
        "descriptor_count": 0x0D2,
        "command_handler_address": 0x02081730,
        "vm_run_call_sites": [0x02083FFC, 0x02084508, 0x02084530],
    },
}


class DescriptorError(RuntimeError):
    pass


def _read_descriptors(overlays_root: Path, instance: dict[str, Any]) -> list[int]:
    overlay_id = instance["overlay_id"]
    path = overlays_root / f"ov{overlay_id:03d}.bin"
    data = path.read_bytes()
    offset = instance["descriptor_table_address"] - instance["load_address"]
    count = instance["descriptor_count"]
    end = offset + count * 4
    if offset < 0 or end > len(data):
        raise DescriptorError(
            f"{path}: descriptor table 0x{offset:X}..0x{end:X} is outside "
            f"the 0x{len(data):X}-byte overlay"
        )
    descriptors = list(struct.unpack_from(f"<{count}I", data, offset))
    invalid = [(index, value) for index, value in enumerate(descriptors) if value > 0x7F]
    if invalid:
        index, value = invalid[0]
        raise DescriptorError(
            f"{path}: opcode 0x{index:X} has invalid descriptor 0x{value:X}"
        )
    return descriptors


def _format_address(value: int) -> str:
    return f"0x{value:08X}"


def _document(
    version: str,
    name: str,
    instance: dict[str, Any],
    descriptors: list[int],
    generic_names: dict[str, str],
) -> dict[str, Any]:
    return {
        "schema": SCHEMA,
        "version": version,
        "instance": name,
        "role": instance["role"],
        "overlay_id": instance["overlay_id"],
        "descriptor_table_address": _format_address(
            instance["descriptor_table_address"]
        ),
        "command_handler_address": _format_address(
            instance["command_handler_address"]
        ),
        "vm_run_call_sites": [
            _format_address(address) for address in instance["vm_run_call_sites"]
        ],
        "descriptor_encoding": DESCRIPTOR_ENCODING,
        "generic_opcode_count": GENERIC_OPCODE_COUNT,
        "descriptors": [f"0x{value:02X}" for value in descriptors],
        "known_names": generic_names,
    }


def _load_battle_config(version: str) -> dict[str, Any]:
    path = ROOT / "config" / version / "battle_ai_vm.json"
    return json.loads(path.read_text(encoding="utf-8"))


def build_documents(version: str, overlays_root: Path) -> dict[str, dict[str, Any]]:
    extracted = {
        name: _read_descriptors(overlays_root, instance)
        for name, instance in INSTANCES.items()
    }
    for opcode in range(GENERIC_OPCODE_COUNT):
        values = {descriptors[opcode] for descriptors in extracted.values()}
        if len(values) != 1:
            raise DescriptorError(
                f"generic opcode 0x{opcode:X} has inconsistent descriptors: "
                + ", ".join(f"0x{value:02X}" for value in sorted(values))
            )

    battle = _load_battle_config(version)
    battle_descriptors = [int(value, 0) for value in battle["descriptors"]]
    if battle_descriptors != extracted["battle"]:
        raise DescriptorError(
            f"config/{version}/battle_ai_vm.json does not match overlay 2"
        )
    generic_names = {
        key: value
        for key, value in battle["known_names"].items()
        if int(key, 0) < GENERIC_OPCODE_COUNT
    }
    return {
        name: _document(
            version, name, INSTANCES[name], extracted[name], generic_names
        )
        for name in ("field", "scene")
    }


def _write_document(path: Path, document: dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(document, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
        newline="\n",
    )


def _preserve_instance_names(path: Path, document: dict[str, Any]) -> None:
    """Keep reviewed, instance-specific names across descriptor regeneration."""
    if not path.is_file():
        return
    existing = json.loads(path.read_text(encoding="utf-8"))
    for key, value in existing.get("known_names", {}).items():
        if int(key, 0) >= GENERIC_OPCODE_COUNT:
            document["known_names"][key] = value


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", default="eur")
    parser.add_argument(
        "--overlays-root",
        type=Path,
        default=ROOT / "build" / "rom_extract_eur" / "arm9_overlays",
    )
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument("--write", action="store_true")
    action.add_argument("--check", action="store_true")
    args = parser.parse_args()

    documents = build_documents(args.version, args.overlays_root)
    for name, document in documents.items():
        path = ROOT / "config" / args.version / f"{name}_vm.json"
        _preserve_instance_names(path, document)
        if args.write:
            _write_document(path, document)
            print(f"Wrote {path}")
            continue
        if not path.is_file():
            raise DescriptorError(f"missing generated descriptor config {path}")
        checked_in = json.loads(path.read_text(encoding="utf-8"))
        if checked_in != document:
            raise DescriptorError(f"{path} is stale; rerun with --write")
        print(f"Verified {name}: {len(document['descriptors'])} descriptors")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
