"""A module comment that states an address range must state the right one.

Source files open with a comment naming the native range the unit owns, which
is the first thing a reader checks against a disassembly. Merging or splitting
a unit changes that range, so the comment is verified against the component's
delinks entry rather than trusted.
"""

import re
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

from tools.reorganize_sources import component_delinks

RANGE_RE = re.compile(r"0x([0-9A-Fa-f]{8})\s*-\s*0x([0-9A-Fa-f]{8})")


def leading_comment(text: str) -> str:
    """The first block comment, skipping an extern "C" opener."""
    body = text.lstrip()
    if body.startswith('extern "C" {'):
        body = body[len('extern "C" {'):].lstrip()
    if not body.startswith("/*"):
        return ""
    return body[: body.find("*/") + 2] if "*/" in body else ""


class ModuleCommentTests(unittest.TestCase):
    def test_documented_ranges_match_the_delinks_entry(self):
        owners = {}
        for component, table in component_delinks("eur").items():
            for block in table.blocks:
                if block.name and ".text" in block.sections:
                    owners[block.name] = (component, block.sections[".text"])

        wrong = []
        for path in sorted(list(ROOT.glob("src/**/*.c")) + list(ROOT.glob("src/**/*.cpp"))):
            relative = str(path.relative_to(ROOT)).replace("\\", "/")
            comment = leading_comment(path.read_text(encoding="utf-8", errors="replace"))
            match = RANGE_RE.search(comment)
            if not match:
                continue
            if relative not in owners:
                wrong.append(f"{relative}: documents a range but has no delinks entry")
                continue
            start, end = owners[relative][1]
            documented = (int(match.group(1), 16), int(match.group(2), 16))
            if documented != (start, end):
                wrong.append(
                    f"{relative}: comment says {documented[0]:#010x}-{documented[1]:#010x}, "
                    f"delinks says {start:#010x}-{end:#010x}"
                )
        self.assertEqual(wrong, [], "module comments name the wrong native range")


if __name__ == "__main__":
    unittest.main()
