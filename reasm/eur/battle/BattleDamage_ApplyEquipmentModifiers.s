/* Apply defending and attacking party equipment modifiers to base damage. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_ApplyEquipmentModifiers, "ax", %progbits
.balign 4

.equ PARTY_ACTOR_ID_FIRST, 0x38
.equ PARTY_ACTOR_ID_END, 0x3C
.equ BATTLE_ACTOR_RESOURCE_SLOT, 0x6C
.equ SAVE_EQUIPMENT_RECORD_SIZE, 0x24
.equ SAVE_EQUIPMENT_EFFECT_OFFSET, 0x418
.equ SAVE_CONSUMABLE_COUNTS, 0x48E
.equ ITEM_TAG, 0x2000
.equ ITEM_INDEX_MASK, 0x0FFF
.equ ITEM_RECORD_SIZE, 0x14
.equ ITEM_RECORD_EFFECT_FLAGS, 0x0C
.equ EQUIP_DEFENSE_QUARTER, 0x3015
.equ EQUIP_DEFENSE_HALF, 0x301F
.equ EQUIP_DEFENSE_PENALTY, 0x3020
.equ EQUIP_LOW_HP_ATTACK, 0x3014
.equ EQUIP_FLAG_ATTACK, 0x300D
.equ EQUIP_ATTACK_BOOST, 0x3016
.equ EQUIP_INVENTORY_ATTACK, 0x301D
.equ EQUIP_ATTACK_HALF, 0x301F
.equ EQUIP_ATTACK_BOOST_ALT, 0x3020
.equ DAMAGE_MAX, 999

.extern BattleActor_GetPartySlot
.extern BattleActor_IsHpAtMostQuarter
.extern BattleDamage_CalculateBase
.extern gItemRecords
.extern gSaveData

.global BattleDamage_ApplyEquipmentModifiers
.type BattleDamage_ApplyEquipmentModifiers, %function
BattleDamage_ApplyEquipmentModifiers:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r1
    mov r4, r0
    mov r6, r3
    bl BattleDamage_CalculateBase
    mov r5, r0

    cmp r7, #PARTY_ACTOR_ID_FIRST
    blo .L_attacker_equipment
    cmp r7, #PARTY_ACTOR_ID_END
    bhs .L_attacker_equipment
    mov r0, r7
    bl BattleActor_GetPartySlot
    ldr r1, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r0, .L_save_data
    ldrh r1, [r1]
    ldr r2, [r0]
    mov r0, #SAVE_EQUIPMENT_RECORD_SIZE
    and r1, r1, #0xFF
    mla r0, r1, r0, r2
    ldrb r1, [r0, #SAVE_EQUIPMENT_EFFECT_OFFSET]
    ldr r0, .L_defense_quarter
    mov r2, #100
    orr r1, r1, #0x3000
    cmp r1, r0
    beq .L_defense_25
    ldr r0, .L_defense_half
    cmp r1, r0
    beq .L_defense_50
    ldr r0, .L_defense_penalty
    cmp r1, r0
    moveq r2, #150
    b .L_scale_defender
.L_defense_50:
    mov r2, #50
    b .L_scale_defender
.L_defense_25:
    mov r2, #25
.L_scale_defender:
    mul r0, r5, r2
    add r1, r0, #50
    ldr r2, .L_divide_by_100_magic
    mov r0, r1, lsr #31
    smull r1, r5, r2, r1
    mov r5, r5, asr #5
    add r5, r0, r5

.L_attacker_equipment:
    cmp r4, #PARTY_ACTOR_ID_FIRST
    blo .L_finish
    cmp r4, #PARTY_ACTOR_ID_END
    bhs .L_finish
    mov r0, r4
    bl BattleActor_GetPartySlot
    ldr r2, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r1, .L_save_data
    ldrh r3, [r2]
    ldr r2, [r1]
    mov r1, #SAVE_EQUIPMENT_RECORD_SIZE
    and r3, r3, #0xFF
    mla r1, r3, r1, r2
    ldrb r1, [r1, #SAVE_EQUIPMENT_EFFECT_OFFSET]
    ldr r3, .L_attack_boost
    mov r4, #100
    orr r7, r1, #0x3000
    cmp r7, r3
    bgt .L_attack_high_range
    ldr r1, .L_low_hp_attack
    cmp r7, r1
    blt .L_attack_low_range
    cmp r7, r1
    beq .L_low_hp_case
    cmp r7, r3
    moveq r4, #150
    b .L_scale_attacker

.L_attack_low_range:
    ldr r0, .L_flag_attack
    cmp r7, r0
    beq .L_flag_case
    b .L_scale_attacker

.L_attack_high_range:
    ldr r0, .L_inventory_attack
    cmp r7, r0
    bgt .L_attack_highest_range
    cmp r7, r0
    beq .L_inventory_case
    b .L_scale_attacker

.L_attack_highest_range:
    ldr r1, .L_attack_boost_alt
    cmp r7, r1
    bgt .L_scale_attacker
    ldr r0, .L_attack_half
    cmp r7, r0
    blt .L_scale_attacker
    cmp r7, r0
    beq .L_attack_50
    cmp r7, r1
    moveq r4, #150
    b .L_scale_attacker

.L_low_hp_case:
    bl BattleActor_IsHpAtMostQuarter
    cmp r0, #0
    movne r4, #250
    b .L_scale_attacker
.L_flag_case:
    cmp r6, #1
    moveq r4, #250
    b .L_scale_attacker

.L_inventory_case:
    mov ip, #0
    mov r3, ip
    ldr r0, .L_item_records
    mov r1, ip
    ldr r6, .L_item_index_mask
    mov lr, #ITEM_RECORD_SIZE
.L_inventory_loop:
    orr r4, r3, #ITEM_TAG
    mov r4, r4, lsl #16
    mov r7, r4, lsr #16
    and r4, r7, #ITEM_TAG
    cmp r4, #ITEM_TAG
    movne r7, r1
    andeq r4, r7, r6
    mlaeq r7, r4, lr, r0
    ldr r4, [r7, #ITEM_RECORD_EFFECT_FLAGS]
    add r3, r3, #1
    mov r4, r4, lsl #1
    mov r4, r4, lsr #25
    cmp r4, #2
    addls r4, r2, #0x400
    ldrsbls r4, [r4, #0x8E]
    add r2, r2, #1
    addls ip, ip, r4
    cmp r3, #14
    blt .L_inventory_loop
    mov r0, ip, lsl #3
    rsb r4, r0, #300
    cmp r4, #100
    movlt r4, #100
    b .L_scale_attacker

.L_attack_50:
    mov r4, #50
.L_scale_attacker:
    mul r0, r5, r4
    add r1, r0, #50
    ldr r2, .L_divide_by_100_magic
    mov r0, r1, lsr #31
    smull r1, r5, r2, r1
    mov r5, r5, asr #5
    add r5, r0, r5

.L_finish:
    ldr r0, .L_damage_max
    cmp r5, r0
    movgt r5, r0
    mov r0, r5
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_save_data:
    .word gSaveData
.L_defense_quarter:
    .word EQUIP_DEFENSE_QUARTER
.L_defense_half:
.L_attack_half:
    .word EQUIP_DEFENSE_HALF
.L_defense_penalty:
.L_attack_boost_alt:
    .word EQUIP_DEFENSE_PENALTY
.L_divide_by_100_magic:
    .word 0x51EB851F
.L_attack_boost:
    .word EQUIP_ATTACK_BOOST
.L_low_hp_attack:
    .word EQUIP_LOW_HP_ATTACK
.L_flag_attack:
    .word EQUIP_FLAG_ATTACK
.L_inventory_attack:
    .word EQUIP_INVENTORY_ATTACK
.L_item_records:
    .word gItemRecords
.L_item_index_mask:
    .word ITEM_INDEX_MASK
.L_damage_max:
    .word DAMAGE_MAX
.size BattleDamage_ApplyEquipmentModifiers, . - BattleDamage_ApplyEquipmentModifiers
