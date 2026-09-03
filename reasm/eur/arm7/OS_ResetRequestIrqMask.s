/* Atomically replace the hardware interrupt-request flags. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_ResetRequestIrqMask, "ax", %progbits
.balign 4

.global OS_ResetRequestIrqMask
.type OS_ResetRequestIrqMask, %function
.extern REG_IF
.extern REG_IME
OS_ResetRequestIrqMask:
    ldr r12, .L_ime
    ldrh r3, [r12]
    mov r1, #0
    strh r1, [r12]
    ldr r2, .L_if
    ldr r1, [r2]
    str r0, [r2]
    ldrh r0, [r12]
    strh r3, [r12]
    mov r0, r1
    bx lr

.L_ime:
    .word REG_IME
.L_if:
    .word REG_IF

.size OS_ResetRequestIrqMask, . - OS_ResetRequestIrqMask
