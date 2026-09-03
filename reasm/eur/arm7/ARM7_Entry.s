/* Resident ARM7 startup from the NDS ARM7 entry point. */

.syntax unified
.arch armv4t
.arm
.section .pit_ARM7_Entry, "ax", %progbits
.balign 4

.global ARM7_Entry
.type ARM7_Entry, %function
.extern ARM7_AutoloadDataStart
.extern ARM7_LoadAutoloads
.extern ARM7_Main
.extern ARM7_MemoryAliasTest
.extern ARM7_ModuleParams
.extern HW_ARM7BiosIrqHandler
.extern HW_ARM7ClearEnd
.extern HW_ARM7IrqStack
.extern HW_ARM7IrqVector
.extern HW_ARM7SystemStack
.extern OS_IrqHandler
ARM7_Entry:
    mov r12, #0x04000000
    str r12, [r12, #0x208]
    ldr r1, .L_autoload_data_start
    mov r0, #0x03800000
    cmp r0, r1
    movpl r1, r0
    ldr r2, .L_clear_end
    mov r0, #0
.Lclear:
    cmp r1, r2
    stmialt r1!, {r0}
    blt .Lclear

    mov r0, #0x13
    msr cpsr_c, r0
    ldr sp, .L_irq_stack
    mov r0, #0x12
    msr cpsr_c, r0
    ldr r0, .L_system_stack
    mov sp, r0
    ldr r1, .L_stack_size
    sub r1, r0, r1
    mov r0, #0x1F
    msr cpsr_fsxc, r0
    sub sp, r1, #4

    bl ARM7_LoadAutoloads
    ldr r0, .L_module_params
    ldr r1, [r0, #0xC]
    ldr r2, [r0, #0x10]
    mov r0, #0
.Lclear_bss:
    cmp r1, r2
    strlo r0, [r1], #4
    blo .Lclear_bss

    bl ARM7_MemoryAliasTest
    ldr r1, .L_irq_vector
    ldr r0, .L_irq_handler
    str r0, [r1]
    ldr r1, .L_main
    ldr lr, .L_bios_irq_handler
    bx r1

.L_autoload_data_start:
    .word ARM7_AutoloadDataStart
.L_clear_end:
    .word HW_ARM7ClearEnd
.L_irq_stack:
    .word HW_ARM7IrqStack
.L_system_stack:
    .word HW_ARM7SystemStack
.L_stack_size:
    .word 0x400
.L_module_params:
    .word ARM7_ModuleParams
.L_irq_vector:
    .word HW_ARM7IrqVector
.L_irq_handler:
    .word OS_IrqHandler
.L_main:
    .word ARM7_Main
.L_bios_irq_handler:
    .word HW_ARM7BiosIrqHandler

.size ARM7_Entry, . - ARM7_Entry
