/* Release an actor's attached-number task after its final effect disappears. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamageNumber_CleanupAttached, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_NUMBER_TASK, 0x2C
.equ TASK_CALLBACK, 0x04
.equ TASK_DATA, 0x0C
.equ TASK_DATA_ACTOR_ID, 0x04

.extern BattleActor_GetById

.global BattleDamageNumber_CleanupAttached
.type BattleDamageNumber_CleanupAttached, %function
BattleDamageNumber_CleanupAttached:
    push {r4, lr}
    mov r4, r0
    add r0, r4, #TASK_DATA
    ldrh r0, [r0, #TASK_DATA_ACTOR_ID]
    bl BattleActor_GetById
    ldr r1, [r4, #TASK_DATA]
    cmp r1, #0
    moveq r1, #0
    streq r1, [r4, #TASK_CALLBACK]
    streq r1, [r0, #BATTLE_ACTOR_NUMBER_TASK]
    pop {r4, lr}
    bx lr
.size BattleDamageNumber_CleanupAttached, . - BattleDamageNumber_CleanupAttached
