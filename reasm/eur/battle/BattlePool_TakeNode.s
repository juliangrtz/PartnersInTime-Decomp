/* Remove and return the first raw node from a battle pool's free list. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattlePool_TakeNode, "ax", %progbits
.balign 4

.global BattlePool_TakeNode
.type BattlePool_TakeNode, %function
BattlePool_TakeNode:
    ldr r2, [r0, #4]
    ldr r1, [r2]
    str r1, [r0, #4]
    mov r0, r2
    bx lr
.size BattlePool_TakeNode, . - BattlePool_TakeNode
