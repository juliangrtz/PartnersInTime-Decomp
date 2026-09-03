/* Start the three-stage launch/impact reaction for a party battle actor. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_StartPartyLaunchReaction, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_SCENE_OBJECT, 0x00
.equ BATTLE_ACTOR_REACTION_TASK, 0x28
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ ACTOR_FLAG_HIT_LOCK, 0x200
.equ SCENE_OBJECT_ACTOR_ID, 0xEC
.equ BATTLE_TASK_POOL_OFFSET, 0x8B44
.equ TASK_DATA, 0x0C

.extern BattleDamage_UpdatePartyLaunchReaction
.extern func_ov002_020a519c
.extern func_ov002_020a51f8
.extern gBattleContext

.global BattleDamage_StartPartyLaunchReaction
.type BattleDamage_StartPartyLaunchReaction, %function
BattleDamage_StartPartyLaunchReaction:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    ldr r4, [r5, #BATTLE_ACTOR_REACTION_TASK]
    cmp r4, #0
    bne .L_task_ready
    ldr r1, .L_battle_context
    ldr r0, .L_task_pool_offset
    ldr r2, [r1]
    mov r1, #0
    add r0, r2, r0
    bl func_ov002_020a51f8
    add r1, r5, #BATTLE_ACTOR_REACTION_TASK
    mov r4, r0
    bl func_ov002_020a519c
.L_task_ready:
    ldr r0, [r5, #BATTLE_ACTOR_SCENE_OBJECT]
    add r3, r4, #TASK_DATA
    ldrh r2, [r0, #SCENE_OBJECT_ACTOR_ID]
    mov r1, #3
    mov r0, #1
    strh r2, [r3, #2]
    strh r1, [r4, #TASK_DATA]
    strh r0, [r3, #8]
    ldrh r1, [r5, #BATTLE_ACTOR_FLAGS]
    ldr r0, .L_update_callback
    orr r1, r1, #ACTOR_FLAG_HIT_LOCK
    strh r1, [r5, #BATTLE_ACTOR_FLAGS]
    str r0, [r4, #4]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_task_pool_offset:
    .word BATTLE_TASK_POOL_OFFSET
.L_update_callback:
    .word BattleDamage_UpdatePartyLaunchReaction
.size BattleDamage_StartPartyLaunchReaction, . - BattleDamage_StartPartyLaunchReaction
