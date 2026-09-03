/* Merge a completed attached number with an actor's pending number chain. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamageNumber_MergeAttached, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_ACTOR_NUMBER_TASK, 0x2C
.equ BATTLE_CONTEXT_FLAGS, 0xD3A0
.equ ACTOR_FLAG_NUMBER_PENDING, 0x400
.equ NUMBER_EFFECT_FIRST, 0x36A
.equ NUMBER_EFFECT_MERGED, 0x36B
.equ EFFECT_CALLBACK, 0x04
.equ EFFECT_X, 0x14
.equ EFFECT_Y, 0x16
.equ EFFECT_FLAGS, 0x1A
.equ EFFECT_VALUE, 0x1C
.equ TASK_CALLBACK, 0x04
.equ TASK_DATA, 0x0C
.equ TASK_DATA_ACTOR_ID, 0x04

.extern BattleActor_GetById
.extern BattleDamageNumber_UpdateAttached
.extern BattleModelEffect_Spawn
.extern gBattleContext

.global BattleDamageNumber_MergeAttached
.type BattleDamageNumber_MergeAttached, %function
BattleDamageNumber_MergeAttached:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #0x0C
    ldr r1, .L_battle_context
    mov r7, r0
    ldr r0, [r1]
    add r6, r7, #TASK_DATA
    add r0, r0, #0xD000
    ldr r1, [r0, #0x3A0]
    orr r1, r1, #0x2000
    str r1, [r0, #0x3A0]
    ldrh r0, [r6, #TASK_DATA_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    ldr r3, [r7, #TASK_DATA]
    orr r1, r1, #ACTOR_FLAG_NUMBER_PENDING
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    ldrh r1, [r3, #EFFECT_FLAGS]
    mov r1, r1, lsl #27
    movs r1, r1, lsr #31
    addeq sp, sp, #0x0C
    popeq {r4, r5, r6, r7, lr}
    bxeq lr

    mov r1, #0
    str r1, [r3, #EFFECT_CALLBACK]
    str r1, [r7, #TASK_CALLBACK]
    ldr r4, [r0, #BATTLE_ACTOR_NUMBER_TASK]
    ldr r5, [r3, #EFFECT_VALUE]
    cmp r4, #0
    beq .L_first_number

    ldr r6, [r4, #TASK_DATA]
    mov r2, #0x10
    ldr ip, [r6, #EFFECT_VALUE]
    mov r0, #0x100
    str r1, [r6, #EFFECT_CALLBACK]
    str r2, [sp]
    str r0, [sp, #4]
    ldrsh r2, [r3, #EFFECT_X]
    ldrsh r3, [r3, #EFFECT_Y]
    ldr r0, .L_merged_effect
    mov r7, r4
    add r6, r4, #TASK_DATA
    add r5, r5, ip
    bl BattleModelEffect_Spawn
    str r0, [r4, #TASK_DATA]
    b .L_finish

.L_first_number:
    str r7, [r0, #BATTLE_ACTOR_NUMBER_TASK]
    mov r0, #0x10
    str r0, [sp]
    mov r0, #0x100
    str r0, [sp, #4]
    ldrsh r2, [r3, #EFFECT_X]
    ldrsh r3, [r3, #EFFECT_Y]
    ldr r0, .L_first_effect
    bl BattleModelEffect_Spawn
    str r0, [r6]

.L_finish:
    ldr r2, [r6]
    ldr r0, .L_update_callback
    ldrh r1, [r2, #EFFECT_FLAGS]
    orr r1, r1, #8
    strh r1, [r2, #EFFECT_FLAGS]
    ldr r1, [r6]
    str r5, [r1, #EFFECT_VALUE]
    str r0, [r7, #TASK_CALLBACK]
    add sp, sp, #0x0C
    pop {r4, r5, r6, r7, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.L_merged_effect:
    .word NUMBER_EFFECT_MERGED
.L_first_effect:
    .word NUMBER_EFFECT_FIRST
.L_update_callback:
    .word BattleDamageNumber_UpdateAttached
.size BattleDamageNumber_MergeAttached, . - BattleDamageNumber_MergeAttached
