/* Atomically clear selected bits in the hardware interrupt-enable mask. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_DisableIrqMask, "ax", %progbits
.balign 4

.global OS_DisableIrqMask
.type OS_DisableIrqMask, %function
.extern REG_IE
.extern REG_IME
OS_DisableIrqMask:
    ldr r12, .L_ime
    ldrh r3, [r12]
    mov r1, #0
    strh r1, [r12]
    ldr r2, .L_ie
    ldr r1, [r2]
    mvn r0, r0
    and r0, r1, r0
    str r0, [r2]
    ldrh r0, [r12]
    strh r3, [r12]
    mov r0, r1
    bx lr

.L_ime:
    .word REG_IME
.L_ie:
    .word REG_IE

.size OS_DisableIrqMask, . - OS_DisableIrqMask
