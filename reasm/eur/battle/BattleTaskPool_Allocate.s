/* Take a task from the free list and initialize its linkage fields. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleTaskPool_Allocate, "ax", %progbits
.balign 4

.global BattleTaskPool_Allocate
.type BattleTaskPool_Allocate, %function
BattleTaskPool_Allocate:
    ldr r3, [r0, #4]
    mov r1, #0
    ldr r2, [r3]
    str r2, [r0, #4]
    str r1, [r3, #8]
    str r1, [r3, #4]
    str r0, [r3]
    mov r0, r3
    bx lr
.size BattleTaskPool_Allocate, . - BattleTaskPool_Allocate
