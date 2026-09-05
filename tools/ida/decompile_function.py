"""Print one named function's Hex-Rays pseudocode from an existing database."""

from __future__ import annotations

import ida_hexrays
import ida_idaapi
import ida_kernwin
import ida_name
import ida_pro
import idc


def main() -> None:
    if not ida_hexrays.init_hexrays_plugin():
        raise RuntimeError("the ARM Hex-Rays decompiler is unavailable")

    function_names = idc.ARGV[1:] or ["BattleAI_HandleVmResult"]
    for function_name in function_names:
        address = ida_name.get_name_ea(ida_idaapi.BADADDR, function_name)
        if address == ida_idaapi.BADADDR:
            raise RuntimeError(f"unknown IDA function: {function_name}")
        pseudocode = ida_hexrays.decompile(address)
        if pseudocode is None:
            raise RuntimeError(f"Hex-Rays could not decompile {function_name}")
        ida_kernwin.msg(f"\n===== {function_name} =====\n{pseudocode}\n")
    ida_pro.qexit(0)


if __name__ == "__main__":
    main()
