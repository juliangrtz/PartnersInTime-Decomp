/* Complete party hit path: HP/status changes, reactions, popups, and effects. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_ApplyToParty, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ SCENE_OBJECT_X, 0x04
.equ SCENE_OBJECT_Y, 0x06
.equ SCENE_OBJECT_Z, 0x08
.equ SCENE_OBJECT_DEPTH, 0xEA
.equ SCENE_OBJECT_ACTOR_ID, 0xEC
.equ SCENE_OBJECT_FLAGS, 0xF4
.equ BATTLE_CONTEXT_STATUS_DURATION, 0x12C
.equ PARTY_ACTOR_MARIO, 0x38
.equ PARTY_ACTOR_LUIGI, 0x39
.equ DAMAGE_MAX, 999
.equ ARG_HIT_KIND, 0x30
.equ ARG_STATUS_RECORD, 0x34
.equ LOCAL_POSITION, 0x08

.extern BattleActor_ApplyDamage
.extern BattleActor_HandleHitEquipmentEffect
.extern BattleActor_GetById
.extern BattleActor_GetPartySlot
.extern BattleEntity_BindResource
.extern BattleSound_Play
.extern BattleStatus_ClearEffect
.extern BattleStatus_TryApply
.extern BattleDamage_StartPartyReaction
.extern func_ov002_02065ec4
.extern BattlePosition_StoreViewRelative
.extern func_ov002_02076bd4
.extern func_ov002_02091198
.extern BattleDamage_SpawnNumber
.extern BattleModelEffect_Spawn
.extern BattleSpriteEffect_Spawn
.extern gBattleContext

.global BattleDamage_ApplyToParty
.type BattleDamage_ApplyToParty, %function
BattleDamage_ApplyToParty:
    push {r4, r5, r6, r7, r8, sb, lr}
    sub sp, sp, #0x14
    mov r8, r0
    ldrh r0, [r8, #SCENE_OBJECT_ACTOR_ID]
    mov r5, r3
    mov r7, r1
    mov r6, r2
    ldr r4, [sp, #ARG_STATUS_RECORD]
    bl BattleActor_GetPartySlot
    mov sb, r0

    cmp r5, #1
    ldr r0, .L_damage_max
    movlt r5, #1
    cmp r5, r0
    movgt r5, r0
    ldrsh r0, [r8, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_HandleHitEquipmentEffect
    mov r0, sb
    mov r1, #1
    bl BattleStatus_ClearEffect

    ldr r0, [sp, #ARG_HIT_KIND]
    cmp r0, #2
    bne .L_apply_damage
    ldrsh r0, [sb, #BATTLE_ACTOR_CURRENT_HP]
    cmp r5, r0
    bge .L_damage_applied
.L_apply_damage:
    mov r0, r8
    mov r1, r5
    bl BattleActor_ApplyDamage
.L_damage_applied:
    ldr r0, [sp, #ARG_HIT_KIND]
    cmp r0, #2
    beq .L_position
    mov r0, r8
    bl BattleDamage_StartPartyReaction

.L_position:
    ldr r1, [r8, #SCENE_OBJECT_FLAGS]
    ldrsh r0, [r8, #SCENE_OBJECT_Y]
    mov r1, r1, lsl #14
    mov r1, r1, lsr #31
    str r1, [sp]
    ldr r1, [r8, #SCENE_OBJECT_FLAGS]
    rsb r2, r0, #0x100
    mov r1, r1, lsl #17
    mov r1, r1, lsr #31
    str r1, [sp, #4]
    ldrsh sb, [r8, #SCENE_OBJECT_Z]
    ldrsh r3, [r8, #SCENE_OBJECT_DEPTH]
    ldrsh r1, [r8, #SCENE_OBJECT_X]
    sub sb, r0, sb
    add r0, r3, r2, lsl #4
    mov r3, r0, lsl #16
    mov r2, sb, lsl #16
    add r0, sp, #LOCAL_POSITION
    mov r2, r2, asr #16
    mov r3, r3, asr #16
    bl BattlePosition_StoreViewRelative

    ldrh r0, [r8, #SCENE_OBJECT_ACTOR_ID]
    cmp r0, #PARTY_ACTOR_MARIO
    movne r1, #1
    moveq r1, #0
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    bl BattleEntity_BindResource

    ldr r0, [sp, #ARG_HIT_KIND]
    mov sb, #0
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_hit_done
    b .L_hit_kind_0
    b .L_hit_kind_1
    b .L_hit_kind_2
    b .L_hit_kind_3
    b .L_hit_kind_4
    b .L_hit_kind_5

.L_hit_kind_0:
    mov r1, sb
    mov r2, sb
    mov r3, sb
    mov r0, #0x24
    bl BattleSound_Play
    mov r0, #3
    mov r1, #6
    mov r2, sb
    bl func_ov002_02065ec4
    mov r1, sb
    str r1, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r2, [sp, #LOCAL_POSITION + 2]
    mov r0, r5
    mov r3, #4
    add r1, r1, r7
    add r2, r2, r6
    bl BattleDamage_SpawnNumber
    mov sb, r0
    mov r0, r8
    mov r1, #0x0A
    mvn r2, #0
    bl func_ov002_02091198
    b .L_hit_done

.L_hit_kind_1:
    mov r1, sb
    mov r2, sb
    mov r3, sb
    mov r0, #0x24
    bl BattleSound_Play
    mov r0, #3
    mov r1, #6
    mov r2, sb
    bl func_ov002_02065ec4
    mov r1, sb
    str r1, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r2, [sp, #LOCAL_POSITION + 2]
    mov r0, r5
    mov r3, #5
    add r1, r1, r7
    add r2, r2, r6
    bl BattleDamage_SpawnNumber
    mov sb, r0
    mov r0, r8
    mov r1, #0x0B
    mvn r2, #0
    bl func_ov002_02091198
    b .L_hit_done

.L_hit_kind_2:
    mov r1, sb
    mov r2, sb
    mov r3, sb
    mov r0, #0x25
    bl BattleSound_Play
    mov r0, #1
    mov r1, #0x3C
    mov r2, sb
    bl func_ov002_02065ec4
    mov r1, sb
    str r1, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r2, [sp, #LOCAL_POSITION + 2]
    mov r0, r5
    mov r3, #4
    add r1, r1, r7
    add r2, r2, r6
    bl BattleDamage_SpawnNumber
    mov sb, r0
    mov r0, r8
    mov r1, #0x0C
    mvn r2, #0
    bl func_ov002_02091198
    mov r0, #0
    str r0, [sp]
    mov r0, #0x100
    mov r2, r7, lsl #16
    mov r3, r6, lsl #16
    str r0, [sp, #4]
    mov r0, #0x1B
    mov r1, r8
    mov r2, r2, asr #16
    mov r3, r3, asr #16
    bl BattleModelEffect_Spawn
    mov r0, #0x100
    str r0, [sp]
    ldrsh r2, [sp, #LOCAL_POSITION]
    ldrsh r1, [sp, #LOCAL_POSITION + 2]
    ldrsh r3, [sp, #LOCAL_POSITION + 4]
    add r5, r2, r7
    add r2, r1, r6
    mov r1, r5, lsl #16
    mov r2, r2, lsl #16
    mov r0, #0x0C
    mov r1, r1, asr #16
    mov r2, r2, asr #16
    bl BattleSpriteEffect_Spawn
    b .L_hit_done

.L_hit_kind_3:
    mov r2, sb
    mov r0, #1
    mov r1, #0x28
    bl func_ov002_02065ec4
    mov r0, sb
    str r0, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r2, [sp, #LOCAL_POSITION + 2]
    mov r0, r5
    add r1, r1, r7
    add r2, r2, r6
    mov r3, #4
    bl BattleDamage_SpawnNumber
    mov sb, r0
    mov r0, r8
    mov r1, #0x1F
    mvn r2, #0
    bl func_ov002_02091198
    b .L_hit_done

.L_hit_kind_4:
    mov r2, sb
    mov r0, #1
    mov r1, #0x28
    bl func_ov002_02065ec4
    mov r0, sb
    str r0, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r2, [sp, #LOCAL_POSITION + 2]
    mov r0, r5
    add r1, r1, r7
    add r2, r2, r6
    mov r3, #4
    bl BattleDamage_SpawnNumber
    mov sb, r0
    mov r0, r8
    mov r1, #0x21
    mvn r2, #0
    bl func_ov002_02091198
    b .L_hit_done

.L_hit_kind_5:
    mov r2, sb
    mov r0, #1
    mov r1, #0x28
    bl func_ov002_02065ec4
    mov r0, sb
    str r0, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r2, [sp, #LOCAL_POSITION + 2]
    mov r0, r5
    add r1, r1, r7
    add r2, r2, r6
    mov r3, #4
    bl BattleDamage_SpawnNumber
    mov sb, r0
    mov r0, r8
    mov r1, #0x21
    mvn r2, #0
    bl func_ov002_02091198

.L_hit_done:
    cmp sb, #0
    beq .L_status_effect
    ldrh r0, [r8, #SCENE_OBJECT_ACTOR_ID]
    cmp r0, #PARTY_ACTOR_MARIO
    moveq r0, #0x0C
    streq r0, [sb, #0x24]
    moveq r0, #8
    strheq r0, [sb, #0x18]
    ldrh r0, [r8, #SCENE_OBJECT_ACTOR_ID]
    cmp r0, #PARTY_ACTOR_LUIGI
    moveq r0, #0x0D
    streq r0, [sb, #0x24]
    moveq r0, #0
    strheq r0, [sb, #0x18]

.L_status_effect:
    cmp r4, #0
    addeq sp, sp, #0x14
    popeq {r4, r5, r6, r7, r8, sb, lr}
    bxeq lr
    ldrh r0, [r8, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_GetById
    bl func_ov002_02076bd4
    cmp r0, #0
    addeq sp, sp, #0x14
    popeq {r4, r5, r6, r7, r8, sb, lr}
    bxeq lr
    ldrh r0, [r8, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_GetPartySlot
    ldrsb r2, [r4, #0x0D]
    ldr r1, .L_battle_context
    str r2, [sp]
    ldr r2, [r1]
    ldrsb r1, [r4, #0x0C]
    add r2, r2, #0x100
    ldrsh r2, [r2, #0x2C]
    ldrsb r3, [r4, #0x0E]
    bl BattleStatus_TryApply
    add sp, sp, #0x14
    pop {r4, r5, r6, r7, r8, sb, lr}
    bx lr

.L_damage_max:
    .word DAMAGE_MAX
.L_battle_context:
    .word gBattleContext
.size BattleDamage_ApplyToParty, . - BattleDamage_ApplyToParty
