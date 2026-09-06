"""Prove the scene opcode 0x4E alignment block equivalent to its reference.

Optional analysis dependencies: capstone, pyelftools, z3-solver.
Requires the reference and compiled objects produced by the local EUR build.
This checks machine-code semantics, not byte-identical matching or the full VM.
"""

import argparse
from pathlib import Path

import capstone
from capstone import arm
from elftools.elf.elffile import ELFFile
import z3


ROOT = Path(__file__).resolve().parents[1]
SYMBOL = "SceneVm_DispatchCommand"
BEGIN, END = 0x648, 0x688


def block(path, negative_control=False):
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = elf.get_section_by_name(".symtab").get_symbol_by_name(SYMBOL)
        if not symbols or len(symbols) != 1:
            raise ValueError(f"Expected one {SYMBOL} in {path}")
        symbol = symbols[0]
        section = elf.get_section(symbol["st_shndx"])
        start = symbol["st_value"] - section["sh_addr"]
        data = section.data()[start + BEGIN:start + END]
    if len(data) != END - BEGIN:
        raise ValueError(f"Incomplete alignment block in {path}")
    if negative_control:
        # Alter only the in-memory copy: the first narrowing shift at +0x668.
        offset = 0x668 - BEGIN
        word = int.from_bytes(data[offset:offset + 4], "little")
        if (word & 0x0FE00070) != 0x01A00000 or ((word >> 7) & 31) != 16:
            raise ValueError("Negative control expects MOV/LSL #16 at +0x668")
        word = (word & ~(31 << 7)) | (15 << 7)
        data = data[:offset] + word.to_bytes(4, "little") + data[offset + 4:]
    decoder = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    decoder.detail = True
    instructions = list(decoder.disasm(data, BEGIN))
    if len(instructions) != 16 or any(i.size != 4 for i in instructions):
        raise ValueError(f"Unexpected alignment block layout in {path}")
    return instructions


def execute(instructions, initial, memory):
    registers = dict(initial)
    writes = []

    def value(operand):
        if operand.type == arm.ARM_OP_REG:
            if operand.reg == arm.ARM_REG_PC:
                raise ValueError("PC-dependent arithmetic is outside this model")
            result = registers[operand.reg]
        elif operand.type == arm.ARM_OP_IMM:
            result = z3.BitVecVal(operand.imm, 32)
        else:
            raise ValueError("Unexpected arithmetic operand")
        shift = operand.shift
        if shift.type == arm.ARM_SFT_LSL:
            result = result << shift.value
        elif shift.type == arm.ARM_SFT_ASR:
            result = result >> shift.value
        elif shift.type != arm.ARM_SFT_INVALID:
            raise ValueError("Unexpected shift kind")
        return result

    def address(operand):
        if operand.type != arm.ARM_OP_MEM or operand.mem.index:
            raise ValueError("Unexpected memory addressing mode")
        if operand.mem.base == arm.ARM_REG_PC:
            raise ValueError("PC-relative memory access is outside this model")
        result = z3.simplify(registers[operand.mem.base] + operand.mem.disp)
        if not z3.is_bv_value(result):
            raise ValueError("Unexpected symbolic pointer")
        return result.as_long()

    for index, instruction in enumerate(instructions):
        if instruction.cc != arm.ARM_CC_AL or instruction.update_flags or instruction.writeback:
            raise ValueError("Unexpected conditional, flag-setting or writeback instruction")
        operands = instruction.operands
        opcode = instruction.id
        if opcode == arm.ARM_INS_LDRSH:
            result = z3.SignExt(16, memory[(address(operands[1]), 16)])
        elif opcode == arm.ARM_INS_LDR:
            result = memory[(address(operands[1]), 32)]
        elif opcode == arm.ARM_INS_MOV:
            result = value(operands[1])
        elif opcode in (arm.ARM_INS_LSL, arm.ARM_INS_ASR):
            # Capstone can expose MOV-with-shift through an instruction alias.
            result = value(operands[1])
            if len(operands) == 3:
                if operands[2].type != arm.ARM_OP_IMM:
                    raise ValueError("Register-controlled shifts are outside this model")
                amount = operands[2].imm
                result = result << amount if opcode == arm.ARM_INS_LSL else result >> amount
            elif len(operands) != 2 or operands[1].shift.type == arm.ARM_SFT_INVALID:
                raise ValueError("Unexpected shift alias operands")
        elif opcode == arm.ARM_INS_ADD:
            result = value(operands[1]) + value(operands[2])
        elif opcode == arm.ARM_INS_SUB:
            result = value(operands[1]) - value(operands[2])
        elif opcode == arm.ARM_INS_RSB:
            result = value(operands[2]) - value(operands[1])
        elif opcode == arm.ARM_INS_STRH:
            if index != len(instructions) - 1:
                raise ValueError("Expected the height write at the end of the block")
            writes.append((address(operands[1]), z3.Extract(15, 0, value(operands[0]))))
            continue
        else:
            raise ValueError(f"Unsupported instruction: {instruction.mnemonic} {instruction.op_str}")
        if operands[0].reg not in (arm.ARM_REG_R0, arm.ARM_REG_R1, arm.ARM_REG_R2, arm.ARM_REG_R3):
            raise ValueError("Unexpected destination register in the arithmetic block")
        registers[operands[0].reg] = result
    if len(writes) != 1 or writes[0][0] != 0x200D8:
        raise ValueError("Expected exactly one write to the object's render height")
    return registers, writes[0][1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, default=ROOT / "build/eur/delinks/src/overlay007/scene_vm_dispatch.o")
    parser.add_argument("--candidate", type=Path, default=ROOT / "build/eur/src/overlay007/scene_vm_dispatch.o")
    parser.add_argument("--negative-control", action="store_true",
                        help="also require rejection of a shift changed from 16 to 15 in memory")
    args = parser.parse_args()
    register_names = [f"R{i}" for i in range(13)] + ["SP", "LR", "PC"]
    registers = {getattr(arm, f"ARM_REG_{name}"): z3.BitVec(f"incoming_{name}", 32)
                 for name in register_names}
    registers.update({arm.ARM_REG_R0: z3.BitVecVal(0x10000, 32),
                      arm.ARM_REG_R4: z3.BitVecVal(0x30000, 32),
                      arm.ARM_REG_R5: z3.BitVecVal(0x20000, 32)})
    memory = {
        (0x10006, 16): z3.BitVec("reference_y", 16),
        (0x100D8, 16): z3.BitVec("reference_height", 16),
        (0x20006, 16): z3.BitVec("object_y", 16),
        (0x200D8, 16): z3.BitVec("object_height", 16),
        (0x30010, 32): z3.BitVec("alignment_offset", 32),
    }
    expected, expected_write = execute(block(args.reference), registers, memory)
    actual, actual_write = execute(block(args.candidate), registers, memory)
    solver = z3.Solver()
    solver.set(timeout=30000)
    solver.add(z3.Or(expected_write != actual_write,
                     *[expected[reg] != actual[reg] for reg in registers]))
    result = solver.check()
    if result != z3.unsat:
        raise SystemExit(f"Equivalence not proved: {result}")
    print("PASS: all final registers, the render-height write and unchanged flags agree")
    print("for all four 16-bit field values and every 32-bit alignment offset (SMT: unsat).")
    print("Scope: dispatcher offsets 0x648..0x684; this is not a byte-matching check.")
    if args.negative_control:
        altered, altered_write = execute(block(args.candidate, negative_control=True), registers, memory)
        solver = z3.Solver()
        solver.set(timeout=30000)
        solver.add(z3.Or(expected_write != altered_write,
                         *[expected[reg] != altered[reg] for reg in registers]))
        result = solver.check()
        if result != z3.sat:
            raise SystemExit(f"Negative control did not expose the changed shift: {result}")
        print("PASS: negative control detects the altered shift (SMT: sat); object files are untouched.")


if __name__ == "__main__":
    main()
