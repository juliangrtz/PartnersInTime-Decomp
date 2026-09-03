/* Atomically exchange a word and return its previous value. */

.syntax unified
.arch armv4t
.arm
.section .pit_MI_SwapWord, "ax", %progbits
.balign 4

.global MI_SwapWord
.type MI_SwapWord, %function
MI_SwapWord:
    swp r0, r0, [r1]
    bx lr
.size MI_SwapWord, . - MI_SwapWord
