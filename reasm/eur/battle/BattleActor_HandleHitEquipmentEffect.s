/* Consume the one-shot party hit flag and its power-effect equipment case. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleActor_HandleHitEquipmentEffect, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_RESOURCE_SLOT, 0x6C
.equ BATTLE_ACTOR_SPECIAL_FLAGS, 0x74
.equ SAVE_EQUIPMENT_RECORD_SIZE, 0x24
.equ SAVE_EQUIPMENT_EFFECT_OFFSET, 0x418
.equ EQUIPMENT_EFFECT_POWER_SPECIAL, 0x3024

.extern BattleActor_GetPartySlot
.extern BattleStatus_ClearEffect
.extern gSaveData

.global BattleActor_HandleHitEquipmentEffect
.type BattleActor_HandleHitEquipmentEffect, %function
BattleActor_HandleHitEquipmentEffect:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    bl BattleActor_GetPartySlot
    ldrh r1, [r0, #BATTLE_ACTOR_SPECIAL_FLAGS]
    mov r2, r1, lsl #27
    movs r2, r2, lsr #31
    addne sp, sp, #4
    ldmne sp!, {lr}
    bxne lr
    orr r1, r1, #0x10
    strh r1, [r0, #BATTLE_ACTOR_SPECIAL_FLAGS]
    ldr r2, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r1, .L_save_data
    ldrh r2, [r2]
    ldr r3, [r1]
    mov r1, #SAVE_EQUIPMENT_RECORD_SIZE
    and r2, r2, #0xFF
    mla r1, r2, r1, r3
    ldrb r2, [r1, #SAVE_EQUIPMENT_EFFECT_OFFSET]
    ldr r1, .L_power_special
    orr r2, r2, #0x3000
    cmp r2, r1
    addne sp, sp, #4
    ldmne sp!, {lr}
    bxne lr
    mov r1, #6
    bl BattleStatus_ClearEffect
    add sp, sp, #4
    ldm sp!, {lr}
    bx lr
.L_save_data:
    .word gSaveData
.L_power_special:
    .word EQUIPMENT_EFFECT_POWER_SPECIAL
.size BattleActor_HandleHitEquipmentEffect, . - BattleActor_HandleHitEquipmentEffect
