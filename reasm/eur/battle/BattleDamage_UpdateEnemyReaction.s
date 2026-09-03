/* Finish an enemy hit reaction and release its actor hit lock. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_UpdateEnemyReaction, "ax", %progbits
.balign 4

.equ TASK_CALLBACK, 0x04
.equ TASK_STATE, 0x0C
.equ STATE_EFFECT, 0x04
.equ STATE_ACTOR_ID, 0x02
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_ACTOR_STATE_FLAGS, 0x29C
.equ BATTLE_ACTOR_IMPACT_X, 0x29D
.equ BATTLE_ACTOR_IMPACT_Y, 0x29E
.equ SCENE_OBJECT_RESOURCE, 0xBC
.equ SCENE_OBJECT_ANIMATION, 0xE6
.equ SCENE_OBJECT_ANIMATION_OFFSET, 0xE8
.equ SCENE_OBJECT_ACTOR_ID, 0xEC
.equ SCENE_OBJECT_IMPACT_X, 0x102
.equ SCENE_OBJECT_IMPACT_Y, 0x103
.equ ENEMY_ACTOR_ID_FIRST, 0x3C
.equ ENEMY_ACTOR_ID_END, 0x44
.equ SPECIAL_RESOURCE_1, 0xC1000082
.equ SPECIAL_RESOURCE_2, 0xC10000A4

.extern BattleActor_GetById
.extern BattleActor_GetEnemySlot
.extern BattleSceneObject_GetById
.extern BattleSceneObject_SetAnimation
.extern BattleSceneObject_GetActiveModel

.global BattleDamage_UpdateEnemyReaction
.type BattleDamage_UpdateEnemyReaction, %function
BattleDamage_UpdateEnemyReaction:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r0
    add r6, r7, #TASK_STATE
    ldrh r0, [r6, #STATE_ACTOR_ID]
    bl BattleActor_GetEnemySlot
    mov r5, r0
    ldrh r0, [r6, #STATE_ACTOR_ID]
    ldr r4, [r5]
    bl BattleSceneObject_GetById
    bl BattleSceneObject_GetActiveModel
    cmp r0, #0
    beq .L_finish_task
    ldr r1, [r0, #0x7C]
    mov r1, r1, lsl #23
    movs r1, r1, lsr #31
    beq .L_finish_task
    ldr r1, [r6, #STATE_EFFECT]
    cmp r1, #0
    addne sp, sp, #4
    popne {r4, r5, r6, r7, lr}
    bxne lr

.L_finish_task:
    mov r1, #0
    str r1, [r7, #TASK_CALLBACK]
    cmp r0, #0
    beq .L_unlock_actor
    ldr r0, [r0, #0x7C]
    mov r0, r0, lsl #23
    movs r0, r0, lsr #31
    beq .L_unlock_actor
    ldr r0, [r5, #BATTLE_ACTOR_STATE_FLAGS]
    mov r0, r0, lsl #27
    movs r0, r0, lsr #31
    bne .L_unlock_actor

    ldrsh r5, [r4, #SCENE_OBJECT_ANIMATION]
    cmp r5, #0
    bge .L_set_animation
    ldrsh r3, [r4, #SCENE_OBJECT_ANIMATION_OFFSET]
    mov r0, r4
    mvn r2, #0
    add r3, r3, r5
    strh r3, [r4, #SCENE_OBJECT_ANIMATION_OFFSET]
    bl BattleSceneObject_SetAnimation
    ldrsh r0, [r4, #SCENE_OBJECT_ANIMATION_OFFSET]
    sub r0, r0, r5
    strh r0, [r4, #SCENE_OBJECT_ANIMATION_OFFSET]
    b .L_restore_impact_offsets
.L_set_animation:
    mov r0, r4
    mov r1, r5
    mvn r2, #0
    bl BattleSceneObject_SetAnimation

.L_restore_impact_offsets:
    ldrh r0, [r4, #SCENE_OBJECT_ACTOR_ID]
    cmp r0, #ENEMY_ACTOR_ID_FIRST
    blo .L_unlock_actor
    cmp r0, #ENEMY_ACTOR_ID_END
    bhs .L_unlock_actor
    ldr r2, [r4, #SCENE_OBJECT_RESOURCE]
    ldr r1, .L_special_resource_1
    ldr r2, [r2, #0x24]
    cmp r2, r1
    beq .L_unlock_actor
    ldr r1, .L_special_resource_2
    cmp r2, r1
    beq .L_unlock_actor
    bl BattleActor_GetEnemySlot
    add r1, r4, #0x100
    add r2, r0, #0x200
    ldrsb r7, [r1, #0x02]
    ldrsb r5, [r2, #0x9D]
    mov r3, #0
    add r5, r7, r5
    strb r5, [r4, #SCENE_OBJECT_IMPACT_X]
    ldrsb r5, [r1, #0x03]
    ldrsb r1, [r2, #0x9E]
    add r1, r5, r1
    strb r1, [r4, #SCENE_OBJECT_IMPACT_Y]
    strb r3, [r0, #BATTLE_ACTOR_IMPACT_X]
    strb r3, [r0, #BATTLE_ACTOR_IMPACT_Y]

.L_unlock_actor:
    ldrh r0, [r6, #STATE_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x200
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_special_resource_1:
    .word SPECIAL_RESOURCE_1
.L_special_resource_2:
    .word SPECIAL_RESOURCE_2
.size BattleDamage_UpdateEnemyReaction, . - BattleDamage_UpdateEnemyReaction
