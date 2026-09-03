/* Initialize the ARM7 IRQ dispatch flag and shared VBlank counter. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_InitIrqTable, "ax", %progbits
.balign 4

.global OS_InitIrqTable
.type OS_InitIrqTable, %function
.extern HW_VBLANK_COUNT_BUF
.extern OS_IRQCheckFlag
OS_InitIrqTable:
    mov r1, #0
    ldr r0, .L_irq_check_flag
    strh r1, [r0]
    ldr r0, .L_vblank_count
    str r1, [r0]
    bx lr
.L_irq_check_flag:
    .word OS_IRQCheckFlag
.L_vblank_count:
    .word HW_VBLANK_COUNT_BUF

.size OS_InitIrqTable, . - OS_InitIrqTable
