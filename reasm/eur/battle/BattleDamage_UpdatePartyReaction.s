/* Finish a party hit reaction after its scene animation reports completion. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_UpdatePartyReaction, "ax", %progbits
.balign 4

.equ TASK_CALLBACK, 0x04
.equ TASK_STATE, 0x0C
.equ STATE_ACTOR_ID, 0x02
.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ SCENE_OBJECT_ACTOR_ID, 0xEC
.equ PARTY_ACTOR_MARIO, 0x38

.extern BattleActor_GetById
.extern BattleEntity_BindResource
.extern BattleSceneObject_GetById
.extern func_ov002_02091198
.extern BattleSceneObject_GetActiveModel
.extern func_ov002_020a9280

.global BattleDamage_UpdatePartyReaction
.type BattleDamage_UpdatePartyReaction, %function
BattleDamage_UpdatePartyReaction:
    push {r4, r5, r6, lr}
    mov r6, r0
    add r5, r6, #TASK_STATE
    ldrh r0, [r5, #STATE_ACTOR_ID]
    bl BattleActor_GetById
    ldr r4, [r0]
    ldrh r0, [r5, #STATE_ACTOR_ID]
    bl BattleSceneObject_GetById
    bl BattleSceneObject_GetActiveModel
    ldr r0, [r0, #0x7C]
    mov r0, r0, lsl #29
    movs r0, r0, lsr #31
    popeq {r4, r5, r6, lr}
    bxeq lr

    mov r1, #0
    str r1, [r6, #TASK_CALLBACK]
    ldrh r0, [r4, #SCENE_OBJECT_ACTOR_ID]
    cmp r0, #PARTY_ACTOR_MARIO
    movne r1, #1
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    bl BattleEntity_BindResource
    ldrh r0, [r4, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_GetById
    ldrsh r0, [r0, #BATTLE_ACTOR_CURRENT_HP]
    cmp r0, #0
    bgt .L_alive
    mov r0, r4
    bl func_ov002_020a9280
    pop {r4, r5, r6, lr}
    bx lr
.L_alive:
    mov r0, r4
    mov r1, #0
    mvn r2, #0
    bl func_ov002_02091198
    ldrh r0, [r5, #STATE_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x200
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    pop {r4, r5, r6, lr}
    bx lr
.size BattleDamage_UpdatePartyReaction, . - BattleDamage_UpdatePartyReaction
