"""Every source path named in the maintained documentation must still exist.

Reorganizing translation units is routine here, and a stale path in the overlay
map or the reconstruction notes sends the next reader to a file that is no
longer there.  The historical milestone log is excluded on purpose: its entries
record the tree as it was when the work landed.
"""

import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

MAINTAINED = [
    "AGENTS.md",
    "README.md",
    "docs/DECOMPILATION_STYLE.md",
    "docs/DECOMPILATION_ROADMAP.md",
    "docs/PROGRESS.md",
    "docs/PC_PORT.md",
    "docs/REASSEMBLY_PLAN.md",
    "docs/DATA_MODDING.md",
    "docs/research/OVERLAY_MAP.md",
    "docs/research/BATTLE_MAP.md",
    "docs/research/RECONSTRUCTION_NOTES.md",
    "docs/research/SCRIPT_VM_SEMANTICS.md",
    "docs/research/SCENE_VM_MATCHING.md",
    "docs/research/SCENE_PRESENTATION_MATCHING.md",
    "docs/research/RUNTIME_ANALYSIS.md",
]

# Anchored so a build output (build/eur/src/...) or a link into another project's
# tree is not mistaken for a path in this repository.
PATH_RE = re.compile(
    r"(?<![\w/.-])(?:\.\./)*((?:src|include|tools|config|tests)/[\w./-]+\.\w+)"
)
PLACEHOLDER = re.compile(r"(?:ovNNN|NNN|<|\*)")


class DocumentationPathTests(unittest.TestCase):
    def test_referenced_paths_exist(self):
        missing = []
        for name in MAINTAINED:
            document = ROOT / name
            if not document.is_file():
                continue
            for match in PATH_RE.finditer(document.read_text(encoding="utf-8")):
                target = match.group(1)
                if PLACEHOLDER.search(target):
                    continue
                if not (ROOT / target).exists():
                    missing.append(f"{name}: {target}")
        self.assertEqual(missing, [], "documentation points at paths that no longer exist")


if __name__ == "__main__":
    unittest.main()
