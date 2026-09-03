from __future__ import annotations

import importlib.util
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "pit_reassembly", ROOT / "tools" / "reassembly.py"
)
assert SPEC is not None and SPEC.loader is not None
reassembly = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = reassembly
SPEC.loader.exec_module(reassembly)


def synthetic_nds() -> bytes:
    data = bytearray(0x240)
    data[0:12] = b"SYNTHETIC\0\0\0"
    data[0x0C:0x10] = b"TEST"

    # ARM9 and ARM7 header records.
    struct.pack_into("<4I", data, 0x20, 0x180, 0x02000000, 0x02000000, 8)
    struct.pack_into("<4I", data, 0x30, 0x188, 0x03800000, 0x03800000, 4)

    # One FAT record and one ARM9 overlay record.
    struct.pack_into("<2I", data, 0x48, 0x100, 8)
    struct.pack_into("<2I", data, 0x50, 0x108, 0x20)
    struct.pack_into("<2I", data, 0x58, 0, 0)
    struct.pack_into("<2I", data, 0x100, 0x190, 0x198)
    struct.pack_into(
        "<8I",
        data,
        0x108,
        7,
        0x02001000,
        8,
        4,
        0,
        0,
        0,
        0,
    )
    data[0x180:0x188] = b"ARM9TEST"
    data[0x188:0x18C] = b"ARM7"
    data[0x190:0x198] = b"OVERLAY7"
    return bytes(data)


class ParseModulesTests(unittest.TestCase):
    def test_reads_header_fat_and_overlay_table(self) -> None:
        modules = reassembly.parse_modules(synthetic_nds())
        self.assertEqual([module.name for module in modules], ["arm9", "arm7", "arm9_ov007"])
        self.assertEqual(modules[0].rom_offset, 0x180)
        self.assertEqual(modules[1].load_address, 0x03800000)
        self.assertEqual(modules[2].rom_offset, 0x190)
        self.assertEqual(modules[2].size, 8)
        self.assertEqual(modules[2].bss_size, 4)

    def test_rejects_bad_fat_size(self) -> None:
        data = bytearray(synthetic_nds())
        struct.pack_into("<I", data, 0x4C, 7)
        with self.assertRaises(reassembly.ReassemblyError):
            reassembly.parse_modules(bytes(data))


@unittest.skipUnless(
    shutil.which("llvm-mc") and shutil.which("llvm-objcopy"),
    "LLVM assembler tools are not on PATH",
)
class AssemblyRoundtripTests(unittest.TestCase):
    def test_raw_source_roundtrips_arbitrary_bytes(self) -> None:
        payload = bytes(range(1, 38))
        module = reassembly.Module("test", "arm9", 0x200, len(payload), 0x02001234)
        identity = {"sha1": "0" * 40}
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            source = directory / "test.s"
            obj = directory / "test.o"
            binary = directory / "test.bin"
            reassembly.emit_source(module, payload, source, identity, [("KnownSymbol", 4)])
            subprocess.run(
                [
                    shutil.which("llvm-mc"),
                    "-triple=armv5te-none-eabi",
                    "-filetype=obj",
                    str(source),
                    "-o",
                    str(obj),
                ],
                check=True,
            )
            subprocess.run(
                [
                    shutil.which("llvm-objcopy"),
                    "-O",
                    "binary",
                    "--only-section=.text",
                    str(obj),
                    str(binary),
                ],
                check=True,
            )
            self.assertEqual(binary.read_bytes(), payload)
            self.assertIn(".set KnownSymbol", source.read_text(encoding="utf-8"))


if __name__ == "__main__":
    unittest.main()
