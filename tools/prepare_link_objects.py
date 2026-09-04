#!/usr/bin/env python3

"""Replace DSD reference objects with the corresponding compiled objects."""

import argparse
import os
from pathlib import Path


def object_path(source_file: Path) -> Path:
    return source_file.with_suffix(".o")


def normalized(path: Path) -> str:
    return os.path.normcase(os.path.normpath(path))


def linked_sources(manifest: Path):
    for raw_line in manifest.read_text().splitlines():
        line = raw_line.split("#", maxsplit=1)[0].strip()
        if line:
            yield Path(line)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Prepare the object list used for the final ARM9 link"
    )
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--build-root", type=Path, required=True)
    parser.add_argument("--delinks-root", type=Path, required=True)
    parser.add_argument("--linked-sources", type=Path, required=True)
    args = parser.parse_args()

    replacements: dict[str, Path] = {}
    for source_file in linked_sources(args.linked_sources):
        if source_file.suffix not in {".c", ".cpp"}:
            raise ValueError(f"Unsupported linked source file: {source_file}")
        if not source_file.is_file():
            raise FileNotFoundError(f"Linked source file does not exist: {source_file}")
        compiled_object = args.build_root / object_path(source_file)
        reference_object = args.delinks_root / object_path(source_file)
        replacements[normalized(reference_object)] = compiled_object

    output_lines = []
    replaced = set()
    for line in args.input.read_text().splitlines():
        replacement = replacements.get(normalized(Path(line)))
        if replacement is None:
            output_lines.append(line)
            continue
        output_lines.append(str(replacement))
        replaced.add(normalized(Path(line)))

    missing = replacements.keys() - replaced
    if missing:
        missing_paths = "\n".join(f"  {path}" for path in sorted(missing))
        raise RuntimeError(
            "Compiled source objects are absent from DSD's link order:\n"
            f"{missing_paths}"
        )

    args.output.write_text("\n".join(output_lines) + "\n")


if __name__ == "__main__":
    main()
