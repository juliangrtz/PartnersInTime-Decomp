#!/usr/bin/env python3
"""Inventory code still missing from linked C/C++; no ROM or compiler required.

This is a planning aid, not a matching check or a difficulty prediction. Symbolic
assembly, unlinked source drafts and code without function boundaries remain in
the inventory. Private experiments and runtime coverage are not inferred.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path

try:
    from . import generate_progress as progress
    from .reorganize_sources import component_delinks
except ImportError:
    import generate_progress as progress
    from reorganize_sources import component_delinks


ROOT = Path(__file__).resolve().parents[1]


def candidates(component: progress.Component, drafts: list[dict]) -> list[dict]:
    """Keep real function boundaries, partial matches and unmapped code separate."""
    pending = [unit for unit in component.units if unit.status != progress.STATUS_C]
    claimed = set()
    result = []

    def record(name, native, pieces, kind):
        return {
            "name": name,
            "kind": kind,
            "start": f"0x{native.start:08X}",
            "end": f"0x{native.end:08X}",
            "native_bytes": native.size,
            "missing_c_bytes": sum(piece.size for piece in pieces),
            "partly_linked": any(
                progress.intersect(native, owned) for owned in component.c_ranges
            ),
            "missing_ranges": [
                {"start": f"0x{piece.start:08X}", "end": f"0x{piece.end:08X}",
                 "bytes": piece.size, "status": piece.status}
                for piece in pieces
            ],
            "adjacent_linked_sources": sorted({
                owned.source for owned in component.c_ranges
                if owned.end == native.start or owned.start == native.end
            }),
            "unlinked_source_drafts": sorted({
                draft["source"] for draft in drafts
                if progress.intersect(native, progress.Range(draft["start"], draft["end"]))
            }),
        }

    for name, native in sorted(component.functions, key=lambda item: item[1].start):
        indices = [i for i, unit in enumerate(pending)
                   if native.start <= unit.start and unit.end <= native.end]
        if not indices:
            continue
        if claimed.intersection(indices):
            raise ValueError(f"overlapping function ownership in {component.key}: {name}")
        claimed.update(indices)
        result.append(record(name, native, [pending[i] for i in indices], "function"))
    for index, unit in enumerate(pending):
        if index not in claimed:
            result.append(record("Unmapped code", unit, [unit], "unmapped_code"))
    result.sort(key=lambda entry: int(entry["start"], 16))
    expected = component.total - component.status_bytes(progress.STATUS_C)
    if sum(entry["missing_c_bytes"] for entry in result) != expected:
        raise ValueError(f"backlog does not reconcile with progress for {component.key}")
    return result


def build_inventory(version="eur") -> dict:
    components, arm7_bytes = progress.collect_progress(version)
    linked = progress.parse_linked_sources(version)
    tables = component_delinks(version)
    owners = {}
    for key, table in tables.items():
        for block in table.blocks:
            if block.name:
                owners.setdefault(block.name, []).append((key, block.sections))
    sources = sorted(path.relative_to(ROOT).as_posix()
                     for directory in ("src", "libs")
                     for path in (ROOT / directory).rglob("*")
                     if path.suffix in {".c", ".cpp"})
    unlinked = [{"source": source, "has_delinks_entry": source in owners}
                for source in sources if source not in linked]
    rows = []
    for component in components:
        key = component.key.removeprefix("arm9_")
        drafts = []
        for draft in unlinked:
            for owner, sections in owners.get(draft["source"], []):
                if owner == key:
                    for start, end in sections.values():
                        if any(progress.intersect(progress.Range(start, end), code)
                               for code in component.code_ranges):
                            drafts.append({"source": draft["source"], "start": start, "end": end})
        entries = candidates(component, drafts)
        bands = []
        for label, low, high in (("up_to_128", 0, 128), ("129_to_512", 129, 512),
                                 ("513_to_2048", 513, 2048), ("over_2048", 2049, None)):
            selected = [entry for entry in entries if entry["kind"] == "function"
                        and low <= entry["native_bytes"]
                        and (high is None or entry["native_bytes"] <= high)]
            bands.append({"band": label, "functions": len(selected),
                          "missing_c_bytes": sum(entry["missing_c_bytes"] for entry in selected)})
        rows.append({
            "component": component.key,
            "missing_c_bytes": sum(entry["missing_c_bytes"] for entry in entries),
            "symbolic_asm_bytes": component.status_bytes(progress.STATUS_ASM),
            "function_candidates": sum(entry["kind"] == "function" for entry in entries),
            "unmapped_code_bytes": sum(entry["missing_c_bytes"] for entry in entries
                                       if entry["kind"] == "unmapped_code"),
            "size_bands": bands,
            "candidates": entries,
        })
    totals = progress.totals(components)
    return {
        "schema_version": 1,
        "version": version,
        "scope": "ARM9 code ranges in the progress metric; ARM7 is separate.",
        "limits": "Metadata inventory only. Size and adjacency do not prove ease, exactness, "
                  "original translation units or runtime coverage. Check private drafts before retrying.",
        "totals": {"mapped_code_bytes": totals["code"],
                   "matching_c_bytes": totals[progress.STATUS_C],
                   "missing_c_bytes": totals["code"] - totals[progress.STATUS_C],
                   "symbolic_asm_bytes": totals[progress.STATUS_ASM],
                   "arm7_symbolic_bytes_separate": arm7_bytes,
                   "source_units": len(sources), "linked_units": len(linked)},
        "unlinked_sources": unlinked,
        "components": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", default="eur")
    parser.add_argument("--output", type=Path,
                        default=ROOT / "build/analysis/decompilation_backlog.json")
    args = parser.parse_args()
    inventory = build_inventory(args.version)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(inventory, indent=2) + "\n", encoding="utf-8")
    print(f"Missing C/C++: {inventory['totals']['missing_c_bytes']:,} bytes")
    for row in sorted(inventory["components"], key=lambda item: -item["missing_c_bytes"]):
        print(f"{row['component']:12} {row['missing_c_bytes']:7,d} bytes; "
              f"{row['function_candidates']:4} functions; "
              f"{row['unmapped_code_bytes']} bytes without function boundaries")
    print(f"Inventory: {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
