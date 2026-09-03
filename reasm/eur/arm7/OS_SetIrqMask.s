/* Atomically replace the hardware interrupt-enable mask. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_SetIrqMask, "ax", %progbits
.balign 4

.global OS_SetIrqMask
.type OS_SetIrqMask, %function
.extern REG_IE
.extern REG_IME
OS_SetIrqMask:
    ldr r12, .L_ime
    ldrh r3, [r12]
    mov r1, #0
    strh r1, [r12]
    ldr r2, .L_ie
    ldr r1, [r2]
    str r0, [r2]
    ldrh r0, [r12]
    strh r3, [r12]
    mov r0, r1
    bx lr

.L_ime:
    .word REG_IME
.L_ie:
    .word REG_IE

.size OS_SetIrqMask, . - OS_SetIrqMask
