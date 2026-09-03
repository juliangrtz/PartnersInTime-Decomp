/* Apply one encoded healing, revival, cure, or stat item to a battle actor. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleItemEffect_Apply, "ax", %progbits
.balign 4

.equ ITEM_ID_NONE, 0xFFFF
.equ ITEM_TAG, 0x2000
.equ ITEM_INDEX_MASK, 0x0FFF
.equ ITEM_RECORD_SIZE, 0x14
.equ ITEM_RECORD_EFFECT_FLAGS, 0x0C
.equ ITEM_RECORD_MAGNITUDE, 0x10
.equ BATTLE_ACTOR_MAX_HP, 0x04
.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ BATTLE_ACTOR_POWER_CHANGE_ACTIVE, 0x50
.equ BATTLE_ACTOR_DEFENSE_CHANGE_ACTIVE, 0x5C
.equ BATTLE_ACTOR_SPEED_CHANGE_ACTIVE, 0x68
.equ BATTLE_ACTOR_PARTY_MEMBER, 0x7E
.equ BATTLE_CONTEXT_MODE, 0x558
.equ BATTLE_CONTEXT_ITEM_STATUS_DURATION, 0x12C
.equ PARTY_ACTOR_MARIO, 0x38
.equ PARTY_ACTOR_LUIGI, 0x39

.extern BattleActor_GetById
.extern BattleActor_GetPartySlot
.extern BattleItemEffect_ApplyBadgeBoost
.extern BattleStatus_ClearAll
.extern BattleStatus_ClearEffect
.extern BattleStatus_TryApply
.extern ItemEffect_CalculateValue
.extern gBattleContext
.extern gItemRecords
.extern gSaveData

.global BattleItemEffect_Apply
.type BattleItemEffect_Apply, %function
BattleItemEffect_Apply:
    push {r4, r5, lr}
    sub sp, sp, #4
    ldr r2, .L_item_id_none
    mov r5, r0
    cmp r5, r2
    addeq sp, sp, #4
    popeq {r4, r5, lr}
    bxeq lr

    mov r0, r1
    bl BattleActor_GetById
    and r3, r5, #ITEM_TAG
    cmp r3, #ITEM_TAG
    mov r4, r0
    movne r0, #0
    ldreq r0, .L_item_index_mask
    ldreq r2, .L_item_records
    andeq r1, r5, r0
    moveq r0, #ITEM_RECORD_SIZE
    mlaeq r0, r1, r0, r2
    ldr r0, [r0, #ITEM_RECORD_EFFECT_FLAGS]
    mov r0, r0, lsl #1
    mov r0, r0, lsr #25
    cmp r0, #6
    addls pc, pc, r0, lsl #2
    b .L_return
    b .L_heal_fixed
    b .L_heal_fixed_second_kind
    b .L_revive
    b .L_cure
    b .L_stat_item
    b .L_stat_item
    b .L_stat_item

.L_heal_fixed:
    ldrsh r1, [r4, #BATTLE_ACTOR_CURRENT_HP]
    ldrsh r2, [r4, #BATTLE_ACTOR_MAX_HP]
    mov r0, r5
    mov r3, #0
    bl ItemEffect_CalculateValue
    ldrsh r2, [r4, #BATTLE_ACTOR_CURRENT_HP]
    mov r1, r0
    cmp r1, r2
    addeq sp, sp, #4
    popeq {r4, r5, lr}
    bxeq lr
    mov r0, r4
    bl BattleItemEffect_ApplyBadgeBoost
    add sp, sp, #4
    strh r0, [r4, #BATTLE_ACTOR_CURRENT_HP]
    pop {r4, r5, lr}
    bx lr

.L_heal_fixed_second_kind:
    ldrsh r1, [r4, #BATTLE_ACTOR_CURRENT_HP]
    ldrsh r2, [r4, #BATTLE_ACTOR_MAX_HP]
    mov r0, r5
    mov r3, #0
    bl ItemEffect_CalculateValue
    ldrsh r2, [r4, #BATTLE_ACTOR_CURRENT_HP]
    mov r1, r0
    cmp r1, r2
    addeq sp, sp, #4
    popeq {r4, r5, lr}
    bxeq lr
    mov r0, r4
    bl BattleItemEffect_ApplyBadgeBoost
    add sp, sp, #4
    strh r0, [r4, #BATTLE_ACTOR_CURRENT_HP]
    pop {r4, r5, lr}
    bx lr

.L_revive:
    ldrsh r2, [r4, #BATTLE_ACTOR_MAX_HP]
    mov r1, #0
    mov r0, r5
    mov r3, r1
    bl ItemEffect_CalculateValue
    mov r1, r0
    mov r0, r4
    bl BattleItemEffect_ApplyBadgeBoost
    ldr r1, .L_save_data
    strh r0, [r4, #BATTLE_ACTOR_CURRENT_HP]
    ldr r0, [r1]
    add r0, r0, #0x500
    ldrsh r0, [r0, #0x58]
    cmp r0, #2
    addne sp, sp, #4
    popne {r4, r5, lr}
    bxne lr

    ldrh r0, [r4, #BATTLE_ACTOR_PARTY_MEMBER]
    cmp r0, #4
    bne .L_check_luigi_link
    mov r0, #PARTY_ACTOR_MARIO
    bl BattleActor_GetPartySlot
    bl BattleStatus_ClearAll
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_check_luigi_link:
    cmp r0, #5
    addne sp, sp, #4
    popne {r4, r5, lr}
    bxne lr
    mov r0, #PARTY_ACTOR_LUIGI
    bl BattleActor_GetPartySlot
    bl BattleStatus_ClearAll
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_cure:
    mov r0, r4
    mov r1, #2
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #3
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #4
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #5
    bl BattleStatus_ClearEffect

    ldrsb r0, [r4, #BATTLE_ACTOR_POWER_CHANGE_ACTIVE]
    cmp r0, #0
    bge .L_check_defense_debuff
    mov r0, r4
    mov r1, #6
    bl BattleStatus_ClearEffect

.L_check_defense_debuff:
    ldrsb r0, [r4, #BATTLE_ACTOR_DEFENSE_CHANGE_ACTIVE]
    cmp r0, #0
    bge .L_check_speed_debuff
    mov r0, r4
    mov r1, #7
    bl BattleStatus_ClearEffect

.L_check_speed_debuff:
    ldrsb r0, [r4, #BATTLE_ACTOR_SPEED_CHANGE_ACTIVE]
    cmp r0, #0
    addge sp, sp, #4
    popge {r4, r5, lr}
    bxge lr
    mov r0, r4
    mov r1, #8
    bl BattleStatus_ClearEffect
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_stat_item:
    cmp r3, #ITEM_TAG
    movne r2, #0
    ldreq r0, .L_item_index_mask
    ldreq r2, .L_item_records
    andeq r1, r5, r0
    moveq r0, #ITEM_RECORD_SIZE
    mlaeq r2, r1, r0, r2
    ldr r0, [r2, #ITEM_RECORD_EFFECT_FLAGS]
    mov r0, r0, lsl #1
    mov r0, r0, lsr #25
    cmp r0, #4
    beq .L_apply_power_change
    cmp r0, #5
    beq .L_apply_defense_change
    cmp r0, #6
    beq .L_apply_speed_change
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_apply_power_change:
    mov r0, #100
    str r0, [sp]
    ldr r0, .L_battle_context
    ldrh r3, [r2, #ITEM_RECORD_MAGNITUDE]
    ldr r1, [r0]
    mov r0, r4
    add r1, r1, #0x100
    ldrsh r2, [r1, #0x2C]
    mov r1, #6
    bl BattleStatus_TryApply
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_apply_defense_change:
    mov r0, #100
    str r0, [sp]
    ldr r0, .L_battle_context
    ldrh r3, [r2, #ITEM_RECORD_MAGNITUDE]
    ldr r1, [r0]
    mov r0, r4
    add r1, r1, #0x100
    ldrsh r2, [r1, #0x2C]
    mov r1, #7
    bl BattleStatus_TryApply
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_apply_speed_change:
    mov r0, #100
    str r0, [sp]
    ldr r0, .L_battle_context
    ldrh r3, [r2, #ITEM_RECORD_MAGNITUDE]
    ldr r1, [r0]
    mov r0, r4
    add r1, r1, #0x100
    ldrsh r2, [r1, #0x2C]
    mov r1, #8
    bl BattleStatus_TryApply

.L_return:
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_item_id_none:
    .word ITEM_ID_NONE
.L_item_index_mask:
    .word ITEM_INDEX_MASK
.L_item_records:
    .word gItemRecords
.L_save_data:
    .word gSaveData
.L_battle_context:
    .word gBattleContext
.size BattleItemEffect_Apply, . - BattleItemEffect_Apply
