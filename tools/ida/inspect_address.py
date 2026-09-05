"""Print the containing function and code references for one or more addresses."""

from __future__ import annotations

import ida_funcs
import ida_idaapi
import ida_name
import ida_pro
import idautils
import idc


def format_function(address: int) -> str:
    function = ida_funcs.get_func(address)
    if function is None:
        return f"{address:08X}: no function"

    name = ida_name.get_name(function.start_ea) or f"sub_{function.start_ea:08X}"
    return (
        f"{address:08X}: {name} "
        f"[{function.start_ea:08X}, {function.end_ea:08X})"
    )


def main() -> None:
    if len(idc.ARGV) < 2:
        raise RuntimeError("usage: inspect_address.py ADDRESS [ADDRESS ...]")

    for argument in idc.ARGV[1:]:
        address = int(argument, 0)
        print(f"\n===== {address:08X} =====")
        print(format_function(address))

        print("code references from:")
        references = list(idautils.CodeRefsTo(address, False))
        if not references:
            print("  none")
        for reference in references:
            print(f"  {format_function(reference)}")

        print("code references to:")
        references = list(idautils.CodeRefsFrom(address, False))
        if not references:
            print("  none")
        for reference in references:
            print(f"  {format_function(reference)}")

        print("data references from:")
        references = list(idautils.DataRefsTo(address))
        if not references:
            print("  none")
        for reference in references:
            print(f"  {format_function(reference)}")

        print("data references to:")
        references = list(idautils.DataRefsFrom(address))
        if not references:
            print("  none")
        for reference in references:
            print(f"  {format_function(reference)}")

    ida_pro.qexit(0)


if __name__ == "__main__":
    main()
