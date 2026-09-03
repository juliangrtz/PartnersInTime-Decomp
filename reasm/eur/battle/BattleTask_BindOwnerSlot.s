/* Bind a task to an owner pointer slot and return the displaced task. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleTask_BindOwnerSlot, "ax", %progbits
.balign 4

.global BattleTask_BindOwnerSlot
.type BattleTask_BindOwnerSlot, %function
BattleTask_BindOwnerSlot:
    str r1, [r0, #8]
    ldr r2, [r1]
    cmp r2, r0
    moveq r2, #0
    str r0, [r1]
    mov r0, r2
    bx lr
.size BattleTask_BindOwnerSlot, . - BattleTask_BindOwnerSlot
