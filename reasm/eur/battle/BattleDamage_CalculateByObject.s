/* Resolve scene-object IDs and calculate their battle damage with mode/equipment rules. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_CalculateByObject, "ax", %progbits
.balign 4

.equ PARTY_ACTOR_ID_FIRST, 0x38
.equ PARTY_ACTOR_ID_END, 0x3C
.equ ENEMY_ACTOR_ID_END, 0x44
.equ BATTLE_ACTOR_DAMAGE_SCALE, 0x22
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_ACTOR_RESOURCE_SLOT, 0x6C
.equ BATTLE_ACTOR_PARTY_MEMBER, 0x7E
.equ SCENE_OBJECT_LINKED_ACTOR_ID, 0xEE
.equ BATTLE_CONTEXT_ACTIVE_ACTOR_ID, 0x20
.equ BATTLE_CONTEXT_DAMAGE_MODE, 0xCB78
.equ SAVE_EQUIPMENT_RECORD_SIZE, 0x24
.equ SAVE_EQUIPMENT_EFFECT_OFFSET, 0x418
.equ EQUIPMENT_EFFECT_DAMAGE_BOOST, 0x301B
.equ DAMAGE_MAX, 999

.extern BattleActor_GetById
.extern BattleDamage_ApplyEquipmentModifiers
.extern BattleDamage_CalculateBase
.extern BattleSceneObject_GetById
.extern gBattleContext
.extern gSaveData

.global BattleDamage_CalculateByObject
.type BattleDamage_CalculateByObject, %function
BattleDamage_CalculateByObject:
    push {r4, r5, r6, lr}
    mov r5, r1
    bl BattleSceneObject_GetById
    ldrh r6, [r0, #SCENE_OBJECT_LINKED_ACTOR_ID]
    mov r0, r5
    bl BattleSceneObject_GetById
    cmp r6, #PARTY_ACTOR_ID_FIRST
    ldrh r5, [r0, #SCENE_OBJECT_LINKED_ACTOR_ID]
    blo .L_attacker_not_party
    cmp r6, #PARTY_ACTOR_ID_END
    blo .L_attacker_valid
.L_attacker_not_party:
    cmp r6, #PARTY_ACTOR_ID_END
    blo .L_invalid
    cmp r6, #ENEMY_ACTOR_ID_END
    bhs .L_invalid
.L_attacker_valid:
    mov r0, r6
    bl BattleActor_GetById
    mov r4, r0
    b .L_validate_defender
.L_invalid:
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_validate_defender:
    cmp r5, #PARTY_ACTOR_ID_FIRST
    blo .L_defender_not_party
    cmp r5, #PARTY_ACTOR_ID_END
    blo .L_defender_valid
.L_defender_not_party:
    cmp r5, #PARTY_ACTOR_ID_END
    blo .L_invalid_defender
    cmp r5, #ENEMY_ACTOR_ID_END
    bhs .L_invalid_defender
.L_defender_valid:
    mov r0, r5
    bl BattleActor_GetById
    b .L_check_defender_flag
.L_invalid_defender:
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_check_defender_flag:
    ldrh r0, [r0, #BATTLE_ACTOR_FLAGS]
    mov r0, r0, lsl #17
    movs r0, r0, lsr #31
    movne r0, #1
    popne {r4, r5, r6, lr}
    bxne lr

    ldr r0, .L_battle_context
    mov r2, #0
    ldr r0, [r0]
    mov r3, r2
    ldrh r1, [r0, #BATTLE_CONTEXT_ACTIVE_ACTOR_ID]
    cmp r1, #PARTY_ACTOR_ID_FIRST
    blo .L_active_not_party
    cmp r1, #PARTY_ACTOR_ID_END
    blo .L_finish
.L_active_not_party:
    cmp r1, #PARTY_ACTOR_ID_END
    blo .L_active_other
    cmp r1, #ENEMY_ACTOR_ID_END
    bhs .L_active_other

    cmp r6, #PARTY_ACTOR_ID_END
    blo .L_check_party_attacker
    cmp r6, #ENEMY_ACTOR_ID_END
    bhs .L_check_party_attacker
    ldrsh r2, [r4, #BATTLE_ACTOR_DAMAGE_SCALE]
    mov r3, #0
    cmp r2, #0
    movle r2, #0x6400

.L_check_party_attacker:
    cmp r6, #PARTY_ACTOR_ID_FIRST
    blo .L_apply_full_formula
    cmp r6, #PARTY_ACTOR_ID_END
    bhs .L_apply_full_formula
    add r0, r0, #0xCB00
    ldrsh r0, [r0, #0x78]
    mov r3, #1
    cmp r0, #0
    beq .L_mode_zero
    cmp r0, #1
    beq .L_mode_one
    b .L_apply_full_formula

.L_mode_zero:
    ldrh r0, [r4, #BATTLE_ACTOR_PARTY_MEMBER]
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_apply_full_formula
    b .L_mode_zero_0
    b .L_mode_zero_1
    b .L_mode_zero_2
    b .L_mode_zero_3
    b .L_mode_zero_4
    b .L_mode_zero_5
.L_mode_zero_0:
    ldr r2, .L_mode_zero_scale
    b .L_apply_full_formula
.L_mode_zero_1:
    ldr r2, .L_mode_zero_scale
    b .L_apply_full_formula
.L_mode_zero_2:
    ldr r2, .L_mode_zero_scale
    b .L_apply_full_formula
.L_mode_zero_3:
    ldr r2, .L_mode_zero_scale
    b .L_apply_full_formula
.L_mode_zero_4:
    ldr r2, .L_mode_zero_scale
    b .L_apply_full_formula
.L_mode_zero_5:
    ldr r2, .L_mode_zero_scale
    b .L_apply_full_formula

.L_mode_one:
    ldrh r0, [r4, #BATTLE_ACTOR_PARTY_MEMBER]
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_apply_full_formula
    b .L_mode_one_0
    b .L_mode_one_1
    b .L_mode_one_2
    b .L_mode_one_3
    b .L_mode_one_4
    b .L_mode_one_5
.L_mode_one_0:
    mov r2, #0x10C
    b .L_apply_full_formula
.L_mode_one_1:
    mov r2, #0x10C
    b .L_apply_full_formula
.L_mode_one_2:
    mov r2, #0x10C
    b .L_apply_full_formula
.L_mode_one_3:
    mov r2, #0x10C
    b .L_apply_full_formula
.L_mode_one_4:
    mov r2, #0x10C
    b .L_apply_full_formula
.L_mode_one_5:
    mov r2, #0x10C

.L_apply_full_formula:
    mov r0, r6
    mov r1, r5
    bl BattleDamage_ApplyEquipmentModifiers
    mov r2, r0
    b .L_finish

.L_active_other:
    cmp r1, #0
    bne .L_finish
    ldrh r0, [r4, #BATTLE_ACTOR_PARTY_MEMBER]
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_base_formula
    b .L_base_0
    b .L_base_1
    b .L_base_2
    b .L_base_3
    b .L_base_4
    b .L_base_5
.L_base_0:
    mov r2, #0x100
    b .L_base_formula
.L_base_1:
    mov r2, #0x100
    b .L_base_formula
.L_base_2:
    mov r2, #0x100
    b .L_base_formula
.L_base_3:
    mov r2, #0x100
    b .L_base_formula
.L_base_4:
    mov r2, #0x100
    b .L_base_formula
.L_base_5:
    mov r2, #0x100

.L_base_formula:
    mov r0, r6
    mov r1, r5
    bl BattleDamage_CalculateBase
    ldr r2, [r4, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r1, .L_save_data
    ldrh r2, [r2]
    ldr r3, [r1]
    mov r1, #SAVE_EQUIPMENT_RECORD_SIZE
    and r2, r2, #0xFF
    mla r1, r2, r1, r3
    ldrb r3, [r1, #SAVE_EQUIPMENT_EFFECT_OFFSET]
    ldr r1, .L_damage_boost
    mov r2, r0
    orr r0, r3, #0x3000
    cmp r0, r1
    bne .L_finish
    mov r0, #140
    mul r0, r2, r0
    add r1, r0, #50
    ldr r3, .L_divide_by_100_magic
    mov r0, r1, lsr #31
    smull r1, r2, r3, r1
    mov r2, r2, asr #5
    add r2, r0, r2

.L_finish:
    ldr r0, .L_damage_max
    cmp r2, r0
    movgt r2, r0
    mov r0, r2
    pop {r4, r5, r6, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_mode_zero_scale:
    .word 0x126
.L_save_data:
    .word gSaveData
.L_damage_boost:
    .word EQUIPMENT_EFFECT_DAMAGE_BOOST
.L_divide_by_100_magic:
    .word 0x51EB851F
.L_damage_max:
    .word DAMAGE_MAX
.size BattleDamage_CalculateByObject, . - BattleDamage_CalculateByObject
