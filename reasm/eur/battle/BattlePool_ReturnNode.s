/* Return a raw node to a battle pool's free list. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattlePool_ReturnNode, "ax", %progbits
.balign 4

.global BattlePool_ReturnNode
.type BattlePool_ReturnNode, %function
BattlePool_ReturnNode:
    ldr r2, [r0, #4]
    str r2, [r1]
    str r1, [r0, #4]
    bx lr
.size BattlePool_ReturnNode, . - BattlePool_ReturnNode
