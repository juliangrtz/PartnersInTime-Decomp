/* Allocate or restart an actor hit-lock task around one attached effect. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_StartEffectReaction, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_REACTION_TASK, 0x28
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ ACTOR_FLAG_HIT_LOCK, 0x200
.equ SCENE_OBJECT_ACTOR_ID, 0xEC
.equ BATTLE_TASK_POOL_OFFSET, 0x8B44
.equ TASK_DATA, 0x0C
.equ TASK_DATA_ACTOR_ID, 0x02
.equ TASK_DATA_EFFECT, 0x04

.extern BattleActor_GetById
.extern BattleDamage_UpdateEffectReaction
.extern BattleTask_BindOwnerSlot
.extern BattleTaskList_Insert
.extern BattleModelEffect_SpawnAttached
.extern gBattleContext

.global BattleDamage_StartEffectReaction
.type BattleDamage_StartEffectReaction, %function
BattleDamage_StartEffectReaction:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #0x0C
    mov r7, r0
    ldrh r0, [r7, #SCENE_OBJECT_ACTOR_ID]
    mov r6, r1
    bl BattleActor_GetById
    mov r4, r0
    ldr r5, [r4, #BATTLE_ACTOR_REACTION_TASK]
    cmp r5, #0
    bne .L_reuse_task
    ldr r1, .L_battle_context
    ldr r0, .L_task_pool_offset
    ldr r2, [r1]
    mov r1, #0
    add r0, r2, r0
    bl BattleTaskList_Insert
    mov r5, r0
    add r1, r4, #BATTLE_ACTOR_REACTION_TASK
    bl BattleTask_BindOwnerSlot
    add r4, r5, #TASK_DATA
    b .L_task_ready
.L_reuse_task:
    add r4, r5, #TASK_DATA
    ldr r1, [r4, #TASK_DATA_EFFECT]
    cmp r1, #0
    movne r0, #0
    strne r0, [r1, #0x34]
.L_task_ready:
    mov r3, #0
    str r3, [sp]
    str r3, [sp, #4]
    mov r0, #0x100
    mov r1, r6
    mov r2, r7
    str r0, [sp, #8]
    add r0, r4, #TASK_DATA_EFFECT
    bl BattleModelEffect_SpawnAttached
    ldrh r0, [r7, #SCENE_OBJECT_ACTOR_ID]
    strh r0, [r4, #TASK_DATA_ACTOR_ID]
    ldrh r0, [r7, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r2, [r0, #BATTLE_ACTOR_FLAGS]
    ldr r1, .L_update_callback
    orr r2, r2, #ACTOR_FLAG_HIT_LOCK
    strh r2, [r0, #BATTLE_ACTOR_FLAGS]
    str r1, [r5, #4]
    add sp, sp, #0x0C
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_task_pool_offset:
    .word BATTLE_TASK_POOL_OFFSET
.L_update_callback:
    .word BattleDamage_UpdateEffectReaction
.size BattleDamage_StartEffectReaction, . - BattleDamage_StartEffectReaction
