/* Append a callback and argument to the battle system's 32-entry task queue. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleTaskQueue_Enqueue, "ax", %progbits
.balign 4

.equ BATTLE_TASK_QUEUE_OFFSET, 0x468
.equ BATTLE_TASK_COUNT_PAGE, 0x500
.equ BATTLE_TASK_COUNT_IN_PAGE, 0xEA
.equ BATTLE_TASK_SIZE, 12
.equ BATTLE_TASK_CAPACITY_MINUS_ONE, 31

.extern gBattleSystem

.global BattleTaskQueue_Enqueue
.type BattleTaskQueue_Enqueue, %function
BattleTaskQueue_Enqueue:
    push {r4, lr}
    ldr r2, .L_battle_system
    mov r12, #BATTLE_TASK_SIZE
    ldr r4, [r2]
    ldr r3, .L_task_queue_offset
    add r2, r4, #BATTLE_TASK_COUNT_PAGE
    ldrh lr, [r2, #BATTLE_TASK_COUNT_IN_PAGE]
    add r3, r4, r3
    mov r4, #0
    mul r2, lr, r12
    str r0, [r3, r2]
    add r0, r3, r2
    strh r4, [r0, #4]
    strh r4, [r0, #6]
    str r1, [r0, #8]

    ldr r1, .L_battle_system
    cmp lr, #BATTLE_TASK_CAPACITY_MINUS_ONE
    ldr r1, [r1]
    addlt r4, lr, #1
    add r1, r1, #BATTLE_TASK_COUNT_PAGE
    strh r4, [r1, #BATTLE_TASK_COUNT_IN_PAGE]
    pop {r4, lr}
    bx lr

.L_battle_system:
    .word gBattleSystem
.L_task_queue_offset:
    .word BATTLE_TASK_QUEUE_OFFSET
.size BattleTaskQueue_Enqueue, . - BattleTaskQueue_Enqueue
