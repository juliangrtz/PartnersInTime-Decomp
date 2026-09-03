/* Allocate when needed and prepend a task to an active battle-task list. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleTaskList_Insert, "ax", %progbits
.balign 4

.extern BattleTaskPool_Allocate

.global BattleTaskList_Insert
.type BattleTaskList_Insert, %function
BattleTaskList_Insert:
    push {r4, lr}
    mov r4, r0
    cmp r1, #0
    bne .L_have_task
    bl BattleTaskPool_Allocate
    mov r1, r0
.L_have_task:
    cmp r1, #0
    ldrne r2, [r4]
    moveq r0, #0
    strne r2, [r1]
    movne r0, r1
    strne r1, [r4]
    pop {r4, lr}
    bx lr
.size BattleTaskList_Insert, . - BattleTaskList_Insert
