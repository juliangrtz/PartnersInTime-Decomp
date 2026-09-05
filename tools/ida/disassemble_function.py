"""Print one named function's address-annotated IDA disassembly."""

from __future__ import annotations

import ida_funcs
import ida_idaapi
import ida_kernwin
import ida_lines
import ida_name
import ida_pro
import idc


def main() -> None:
    function_names = idc.ARGV[1:] or ["Entry"]
    for function_name in function_names:
        address = ida_name.get_name_ea(ida_idaapi.BADADDR, function_name)
        if address == ida_idaapi.BADADDR:
            raise RuntimeError(f"unknown IDA function: {function_name}")
        function = ida_funcs.get_func(address)
        if function is None:
            raise RuntimeError(f"IDA has no function at {function_name}")

        ida_kernwin.msg(f"\n===== {function_name} =====\n")
        cursor = function.start_ea
        while cursor < function.end_ea:
            instruction = ida_lines.tag_remove(idc.generate_disasm_line(cursor, 0))
            ida_kernwin.msg(f"{cursor:08X}  {instruction}\n")
            cursor = idc.next_head(cursor, function.end_ea)
    ida_pro.qexit(0)


if __name__ == "__main__":
    main()
