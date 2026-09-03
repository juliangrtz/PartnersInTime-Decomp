/* Count the set bits in a 32-bit word with parallel bit summation. */

.syntax unified
.arch armv4t
.arm
.section .pit_MATH_CountPopulation, "ax", %progbits
.balign 4

.global MATH_CountPopulation
.type MATH_CountPopulation, %function
MATH_CountPopulation:
    ldr r1, .L_mask_1
    and r1, r1, r0, lsr #1
    sub r2, r0, r1
    ldr r0, .L_mask_2
    and r1, r2, r0
    and r0, r0, r2, lsr #2
    add r0, r1, r0
    add r1, r0, r0, lsr #4
    ldr r0, .L_mask_4
    and r0, r1, r0
    add r0, r0, r0, lsr #8
    add r0, r0, r0, lsr #16
    and r0, r0, #0xff
    bx lr
.L_mask_1:
    .word 0x55555555
.L_mask_2:
    .word 0x33333333
.L_mask_4:
    .word 0x0f0f0f0f
.size MATH_CountPopulation, . - MATH_CountPopulation
