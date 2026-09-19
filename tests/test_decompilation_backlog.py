from pathlib import Path

from tools import generate_progress as gp
from tools.decompilation_backlog import candidates


def test_keeps_asm_partial_functions_and_init_gaps_in_the_backlog():
    code = [gp.Range(0x1000, 0x1070), gp.Range(0x2000, 0x2008)]
    functions = [("Linked", gp.Range(0x1000, 0x1020)),
                 ("Partial", gp.Range(0x1020, 0x1040)),
                 ("Asm", gp.Range(0x1040, 0x1060)),
                 ("Tiny", gp.Range(0x1060, 0x1064)),
                 ("Draft", gp.Range(0x1064, 0x1070)),
                 ("OutsideCode", gp.Range(0x3000, 0x3100))]
    linked = [gp.CoverageRange(0x1000, 0x1030, gp.STATUS_C, "linked", "src/linked.c")]
    assembly = [gp.CoverageRange(0x1040, 0x1060, gp.STATUS_ASM, "asm", "reasm/asm.s")]
    component = gp.Component("arm9", "ARM9", Path("config"), code, functions,
                             linked, assembly, gp.build_units(code, functions, linked, assembly))
    result = candidates(component, [{"source": "src/draft.c", "start": 0x1064, "end": 0x1070}])
    by_name = {row["name"]: row for row in result}
    assert sum(row["missing_c_bytes"] for row in result) == 72
    assert "Linked" not in by_name and "OutsideCode" not in by_name
    assert by_name["Partial"]["missing_c_bytes"] == 16
    assert by_name["Partial"]["partly_linked"]
    assert by_name["Asm"]["missing_ranges"][0]["status"] == gp.STATUS_ASM
    assert by_name["Tiny"]["native_bytes"] == 4
    assert by_name["Draft"]["unlinked_source_drafts"] == ["src/draft.c"]
    assert by_name["Unmapped code"]["missing_c_bytes"] == 8
    assert by_name["Unmapped code"]["kind"] == "unmapped_code"
