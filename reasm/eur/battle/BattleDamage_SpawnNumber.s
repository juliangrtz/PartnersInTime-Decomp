/* Spawn a damage-number effect, optionally attaching its lifecycle to an actor. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_SpawnNumber, "ax", %progbits
.balign 4

.equ BATTLE_CONTEXT_POPUP_X, 0xCB9C
.equ BATTLE_CONTEXT_POPUP_Y, 0xCB9E
.equ BATTLE_TASK_POOL_OFFSET, 0x8B44
.equ ATTACHED_NUMBER_EFFECT_ID, 0x369
.equ EFFECT_FLAGS, 0x1A
.equ EFFECT_VALUE, 0x1C
.equ TASK_DATA, 0x0C
.equ TASK_CALLBACK, 0x04

.extern BattleDamageNumber_MergeAttached
.extern BattleTaskList_Insert
.extern func_ov002_020acb88
.extern gBattleContext

.global BattleDamage_SpawnNumber
.type BattleDamage_SpawnNumber, %function
BattleDamage_SpawnNumber:
    push {r4, r5, lr}
    sub sp, sp, #0x0C
    ldr r4, .L_battle_context
    ldrh ip, [sp, #0x18]
    ldr r4, [r4]
    mov r5, r0
    add r0, r4, #0xCB00
    ldrsh r4, [r0, #0x9C]
    ldrsh r0, [r0, #0x9E]
    cmp ip, #0
    add r1, r1, r4
    add r0, r2, r0
    bne .L_attached

    mov r1, r1, lsl #16
    mov r2, #0x10
    mov r4, r0, lsl #16
    mov r0, r3
    str r2, [sp]
    mov ip, #0x100
    mov r2, r1, asr #16
    mov r3, r4, asr #16
    mov r1, #0
    str ip, [sp, #4]
    bl func_ov002_020acb88
    mov r4, r0
    str r5, [r4, #EFFECT_VALUE]
    b .L_return

.L_attached:
    mov r1, r1, lsl #16
    mov r3, r0, lsl #16
    mov r2, #0x10
    ldr r0, .L_attached_effect_id
    str r2, [sp]
    mov r4, #0x100
    mov r2, r1, asr #16
    mov r3, r3, asr #16
    mov r1, #0
    str r4, [sp, #4]
    bl func_ov002_020acb88
    mov r4, r0
    str r5, [r4, #EFFECT_VALUE]
    ldrh r2, [r4, #EFFECT_FLAGS]
    ldr r1, .L_battle_context
    ldr r0, .L_task_pool_offset
    orr r2, r2, #8
    strh r2, [r4, #EFFECT_FLAGS]
    ldr r2, [r1]
    mov r1, #0
    add r0, r2, r0
    bl BattleTaskList_Insert
    ldrh r2, [sp, #0x18]
    str r4, [r0, #TASK_DATA]
    add r3, r0, #TASK_DATA
    ldr r1, .L_update_callback
    strh r2, [r3, #4]
    str r1, [r0, #TASK_CALLBACK]

.L_return:
    mov r0, r4
    add sp, sp, #0x0C
    pop {r4, r5, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.L_attached_effect_id:
    .word ATTACHED_NUMBER_EFFECT_ID
.L_task_pool_offset:
    .word BATTLE_TASK_POOL_OFFSET
.L_update_callback:
    .word BattleDamageNumber_MergeAttached
.size BattleDamage_SpawnNumber, . - BattleDamage_SpawnNumber
