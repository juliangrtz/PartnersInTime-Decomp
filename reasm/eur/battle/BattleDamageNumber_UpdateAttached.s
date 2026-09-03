/* Advance an attached number into its final actor-state-dependent effect. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamageNumber_UpdateAttached, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_CONTEXT_FLAGS, 0xD3A0
.equ ACTOR_FLAG_NUMBER_PENDING, 0x400
.equ NUMBER_EFFECT_NORMAL, 0x36C
.equ NUMBER_EFFECT_ALTERNATE, 0x36D
.equ EFFECT_CALLBACK, 0x04
.equ EFFECT_X, 0x14
.equ EFFECT_Y, 0x16
.equ EFFECT_VALUE, 0x1C
.equ TASK_CALLBACK, 0x04
.equ TASK_DATA, 0x0C
.equ TASK_DATA_ACTOR_ID, 0x04

.extern BattleActor_GetById
.extern BattleDamageNumber_CleanupAttached
.extern func_ov002_020acb44
.extern gBattleContext

.global BattleDamageNumber_UpdateAttached
.type BattleDamageNumber_UpdateAttached, %function
BattleDamageNumber_UpdateAttached:
    push {r4, r5, lr}
    sub sp, sp, #0x0C
    mov r5, r0
    add r4, r5, #TASK_DATA
    ldrh r0, [r4, #TASK_DATA_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r2, [r0, #BATTLE_ACTOR_FLAGS]
    ldr r1, [r5, #TASK_DATA]
    mov r3, r2, lsl #21
    movs r3, r3, lsr #31
    bicne r1, r2, #ACTOR_FLAG_NUMBER_PENDING
    strhne r1, [r0, #BATTLE_ACTOR_FLAGS]
    addne sp, sp, #0x0C
    popne {r4, r5, lr}
    bxne lr

    mov r0, r2, lsl #20
    movs r0, r0, lsr #31
    bne .L_alternate
    mov r2, #0
    ldr r0, .L_cleanup_callback
    str r2, [r1, #EFFECT_CALLBACK]
    str r0, [r5, #TASK_CALLBACK]
    ldrsh r3, [r1, #EFFECT_Y]
    ldr r5, [r1, #EFFECT_VALUE]
    mov r0, #0x10
    str r3, [sp]
    str r0, [sp, #4]
    mov r0, #0x100
    str r0, [sp, #8]
    ldrsh r3, [r1, #EFFECT_X]
    mov r0, r4
    mov r1, #NUMBER_EFFECT_NORMAL
    bl func_ov002_020acb44
    ldr r0, [r4]
    add sp, sp, #0x0C
    str r5, [r0, #EFFECT_VALUE]
    pop {r4, r5, lr}
    bx lr

.L_alternate:
    ldr r0, .L_battle_context
    ldr r0, [r0]
    add r0, r0, #0xD000
    ldr r0, [r0, #0x3A0]
    mov r0, r0, lsl #18
    movs r0, r0, lsr #31
    addne sp, sp, #0x0C
    popne {r4, r5, lr}
    bxne lr
    mov r2, #0
    ldr r0, .L_cleanup_callback
    str r2, [r1, #EFFECT_CALLBACK]
    str r0, [r5, #TASK_CALLBACK]
    ldrsh r3, [r1, #EFFECT_Y]
    ldr r5, [r1, #EFFECT_VALUE]
    mov r0, #0x10
    str r3, [sp]
    str r0, [sp, #4]
    mov r0, #0x100
    str r0, [sp, #8]
    ldrsh r3, [r1, #EFFECT_X]
    ldr r1, .L_alternate_effect
    mov r0, r4
    bl func_ov002_020acb44
    ldr r0, [r4]
    str r5, [r0, #EFFECT_VALUE]
    add sp, sp, #0x0C
    pop {r4, r5, lr}
    bx lr

.L_cleanup_callback:
    .word BattleDamageNumber_CleanupAttached
.L_battle_context:
    .word gBattleContext
.L_alternate_effect:
    .word NUMBER_EFFECT_ALTERNATE
.size BattleDamageNumber_UpdateAttached, . - BattleDamageNumber_UpdateAttached
