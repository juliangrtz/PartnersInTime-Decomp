/* Release a hit-lock after an attached reaction effect finishes or becomes invalid. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_UpdateEffectReaction, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_FLAGS, 0x24
.equ ACTOR_FLAG_HIT_LOCK, 0x200
.equ RESOURCE_FLAGS, 0x7C
.equ TASK_CALLBACK, 0x04
.equ TASK_DATA, 0x0C
.equ TASK_DATA_ACTOR_ID, 0x02
.equ TASK_DATA_EFFECT, 0x04

.extern BattleActor_GetById
.extern BattleSceneObject_GetById
.extern func_ov002_020a3370

.global BattleDamage_UpdateEffectReaction
.type BattleDamage_UpdateEffectReaction, %function
BattleDamage_UpdateEffectReaction:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    add r4, r5, #TASK_DATA
    ldrh r0, [r4, #TASK_DATA_ACTOR_ID]
    bl BattleSceneObject_GetById
    ldrh r0, [r4, #TASK_DATA_ACTOR_ID]
    bl BattleSceneObject_GetById
    bl func_ov002_020a3370
    cmp r0, #0
    beq .L_release
    ldr r0, [r0, #RESOURCE_FLAGS]
    mov r0, r0, lsl #23
    movs r0, r0, lsr #31
    beq .L_release
    ldr r0, [r4, #TASK_DATA_EFFECT]
    cmp r0, #0
    addne sp, sp, #4
    popne {r4, r5, lr}
    bxne lr
.L_release:
    mov r0, #0
    str r0, [r5, #TASK_CALLBACK]
    ldrh r0, [r4, #TASK_DATA_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #ACTOR_FLAG_HIT_LOCK
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size BattleDamage_UpdateEffectReaction, . - BattleDamage_UpdateEffectReaction
