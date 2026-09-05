"""Find functions whose Hex-Rays pseudocode contains one or more strings."""

from __future__ import annotations

import ida_funcs
import ida_hexrays
import ida_kernwin
import ida_pro
import idautils
import idc


def main() -> None:
    if not ida_hexrays.init_hexrays_plugin():
        raise RuntimeError("the ARM Hex-Rays decompiler is unavailable")
    needles = idc.ARGV[1:]
    if not needles:
        raise RuntimeError("pass at least one case-sensitive search string")

    checked = 0
    failed = 0
    for address in idautils.Functions():
        checked += 1
        try:
            pseudocode = ida_hexrays.decompile(address)
        except Exception:
            failed += 1
            continue
        if pseudocode is None:
            failed += 1
            continue
        rendered = str(pseudocode)
        matches = [needle for needle in needles if needle in rendered]
        if matches:
            name = ida_funcs.get_func_name(address)
            ida_kernwin.msg(
                f"MATCH 0x{address:08X} {name}: {', '.join(matches)}\n"
            )
    ida_kernwin.msg(f"Checked {checked} functions; {failed} failed to decompile.\n")
    ida_pro.qexit(0)


if __name__ == "__main__":
    main()
