/* Allocate or restart the per-actor party damage reaction task. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_StartPartyReaction, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_REACTION_TASK, 0x28
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ SCENE_OBJECT_ACTOR_ID, 0xEC
.equ BATTLE_TASK_POOL_OFFSET, 0x8B44

.extern BattleActor_GetById
.extern BattleDamage_UpdatePartyReaction
.extern BattleTask_BindOwnerSlot
.extern BattleTaskList_Insert
.extern gBattleContext

.global BattleDamage_StartPartyReaction
.type BattleDamage_StartPartyReaction, %function
BattleDamage_StartPartyReaction:
    push {r4, r5, r6, lr}
    mov r6, r0
    ldrh r0, [r6, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_GetById
    mov r4, r0
    ldr r5, [r4, #BATTLE_ACTOR_REACTION_TASK]
    cmp r5, #0
    bne .L_task_ready
    ldr r1, .L_battle_context
    ldr r0, .L_task_pool_offset
    ldr r2, [r1]
    mov r1, #0
    add r0, r2, r0
    bl BattleTaskList_Insert
    add r1, r4, #BATTLE_ACTOR_REACTION_TASK
    mov r5, r0
    bl BattleTask_BindOwnerSlot
.L_task_ready:
    ldrh r0, [r6, #SCENE_OBJECT_ACTOR_ID]
    strh r0, [r5, #0x0E]
    ldrh r0, [r6, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r2, [r0, #BATTLE_ACTOR_FLAGS]
    ldr r1, .L_update_callback
    orr r2, r2, #0x200
    strh r2, [r0, #BATTLE_ACTOR_FLAGS]
    str r1, [r5, #4]
    pop {r4, r5, r6, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_task_pool_offset:
    .word BATTLE_TASK_POOL_OFFSET
.L_update_callback:
    .word BattleDamage_UpdatePartyReaction
.size BattleDamage_StartPartyReaction, . - BattleDamage_StartPartyReaction
