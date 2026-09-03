/* Roll resistance/chance and install one ailment or temporary stat change. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleStatus_TryApply, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_SPEED, 0x08
.equ BATTLE_ACTOR_POWER, 0x0A
.equ BATTLE_ACTOR_DEFENSE, 0x0C
.equ BATTLE_ACTOR_BASE_SPEED, 0x10
.equ BATTLE_ACTOR_BASE_POWER, 0x12
.equ BATTLE_ACTOR_BASE_DEFENSE, 0x14
.equ BATTLE_ACTOR_AILMENT_1, 0x30
.equ BATTLE_ACTOR_AILMENT_2, 0x3C
.equ BATTLE_ACTOR_POWER_CHANGE, 0x48
.equ BATTLE_ACTOR_DEFENSE_CHANGE, 0x54
.equ BATTLE_ACTOR_SPEED_CHANGE, 0x60
.equ BATTLE_ACTOR_RESOURCE_SLOT, 0x6C
.equ BATTLE_ACTOR_SPECIAL_FLAGS, 0x74
.equ BATTLE_ACTOR_OBJECT_ID, 0xEC
.equ BATTLE_STATE_DURATION, 0x04
.equ BATTLE_STATE_COUNTER, 0x06
.equ BATTLE_STATE_KIND, 0x08
.equ BATTLE_STATE_PARAM_1, 0x09
.equ BATTLE_STATE_PARAM_2, 0x0A
.equ ENEMY_ACTOR_ID_FIRST, 0x3C
.equ ENEMY_ACTOR_ID_END, 0x44
.equ SAVE_EQUIPMENT_RECORD_SIZE, 0x24
.equ SAVE_EQUIPMENT_EFFECT_OFFSET, 0x418
.equ EQUIPMENT_EFFECT_STATUS_GUARD, 0x3003
.equ EQUIPMENT_EFFECT_POWER_SPECIAL, 0x3024
.equ BATTLE_STAT_MAX, 999

.extern BattleSound_Play
.extern BattleStatus_SpawnEffect
.extern Random_NextModulo
.extern gSaveData

.global BattleStatus_TryApply
.type BattleStatus_TryApply, %function
BattleStatus_TryApply:
    push {r4, r5, r6, r7, r8, sb, sl, lr}
    mov sb, r0
    ldr r0, [sb]
    mov r8, r1
    ldrh r4, [r0, #BATTLE_ACTOR_OBJECT_ID]
    mov sl, r2
    mov r7, r3
    cmp r4, #ENEMY_ACTOR_ID_FIRST
    ldr r6, [sp, #0x20]
    blo .L_not_enemy
    cmp r4, #ENEMY_ACTOR_ID_END
    movlo r5, #1
    blo .L_enemy_flag_ready
.L_not_enemy:
    mov r5, #0

.L_enemy_flag_ready:
    cmp r5, #0
    beq .L_roll_chance
    ldr r0, [sb, #BATTLE_ACTOR_RESOURCE_SLOT]
    cmp r0, #0
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, lr}
    bxeq lr
    ldr r1, [r0]
    cmp r8, #8
    mov r0, #0
    addls pc, pc, r8, lsl #2
    b .L_resistance_dispatch
    b .L_resistance_dispatch
    b .L_enemy_status_one_immune
    b .L_enemy_resistance_bits_10
    b .L_enemy_resistance_bits_8
    b .L_enemy_resistance_bits_12
    b .L_enemy_resistance_bits_12
    b .L_enemy_resistance_bits_14
    b .L_enemy_resistance_bits_14
    b .L_enemy_resistance_bits_14

.L_enemy_status_one_immune:
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr
.L_enemy_resistance_bits_10:
    ldrh r0, [r1, #0x0E]
    mov r0, r0, lsl #20
    mov r0, r0, lsr #30
    b .L_resistance_dispatch
.L_enemy_resistance_bits_8:
    ldrh r0, [r1, #0x0E]
    mov r0, r0, lsl #22
    mov r0, r0, lsr #30
    b .L_resistance_dispatch
.L_enemy_resistance_bits_12:
    ldrh r0, [r1, #0x0E]
    mov r0, r0, lsl #18
    mov r0, r0, lsr #30
    b .L_resistance_dispatch
.L_enemy_resistance_bits_14:
    ldrh r0, [r1, #0x0E]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #30

.L_resistance_dispatch:
    cmp r0, #3
    addls pc, pc, r0, lsl #2
    b .L_roll_chance
    b .L_roll_chance
    b .L_double_chance
    b .L_half_chance
    b .L_resistance_immune
.L_double_chance:
    mov r0, #200
    mul r1, r6, r0
    ldr r2, .L_divide_by_100_magic
    mov r0, r1, lsr #31
    smull r1, r6, r2, r1
    mov r6, r6, asr #5
    add r6, r0, r6
    b .L_roll_chance
.L_half_chance:
    mov r0, #50
    mul r1, r6, r0
    ldr r2, .L_divide_by_100_magic
    mov r0, r1, lsr #31
    smull r1, r6, r2, r1
    mov r6, r6, asr #5
    add r6, r0, r6
    b .L_roll_chance
.L_resistance_immune:
    mov r0, #0
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr

.L_roll_chance:
    mov r0, #100
    bl Random_NextModulo
    cmp r6, r0
    movle r0, #0
    pople {r4, r5, r6, r7, r8, sb, sl, lr}
    bxle lr
    cmp r5, #0
    mov r0, #1
    bne .L_apply_dispatch

    ldr r2, [sb, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r1, .L_save_data
    ldrh r2, [r2]
    ldr r3, [r1]
    mov r1, #SAVE_EQUIPMENT_RECORD_SIZE
    and r2, r2, #0xFF
    mla r1, r2, r1, r3
    ldrb r2, [r1, #SAVE_EQUIPMENT_EFFECT_OFFSET]
    ldr r1, .L_equipment_status_guard
    orr r2, r2, #0x3000
    cmp r2, r1
    bne .L_check_power_special
    cmp r8, #8
    addls pc, pc, r8, lsl #2
    b .L_guard_reject
    b .L_guard_reject
    b .L_guard_reject_ailment
    b .L_guard_reject_ailment
    b .L_guard_reject_ailment
    b .L_guard_reject_ailment
    b .L_guard_reject_ailment
    b .L_guard_power
    b .L_guard_defense
    b .L_guard_speed
.L_guard_reject_ailment:
    mov r0, #0
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr
.L_guard_power:
    ldrsb r1, [sb, #0x50]
    cmp r7, r1
    bge .L_apply_dispatch
    mov r0, #0
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr
.L_guard_defense:
    ldrsb r1, [sb, #0x5C]
    cmp r7, r1
    bge .L_apply_dispatch
    mov r0, #0
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr
.L_guard_speed:
    ldrsb r1, [sb, #0x68]
    cmp r7, r1
    bge .L_apply_dispatch
    mov r0, #0
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr
.L_guard_reject:
    mov r0, #0
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr

.L_check_power_special:
    ldr r1, .L_equipment_power_special
    cmp r2, r1
    bne .L_apply_dispatch
    cmp r8, #6
    bne .L_apply_dispatch
    ldrh r1, [sb, #BATTLE_ACTOR_SPECIAL_FLAGS]
    mov r1, r1, lsl #27
    movs r1, r1, lsr #31
    bne .L_apply_dispatch
    cmp r6, #101
    movne r0, #0
    popne {r4, r5, r6, r7, r8, sb, sl, lr}
    bxne lr
    mov r0, #0

.L_apply_dispatch:
    cmp r8, #8
    addls pc, pc, r8, lsl #2
    b .L_apply_invalid
    b .L_apply_invalid
    b .L_apply_status_1
    b .L_apply_status_2
    b .L_apply_status_3
    b .L_apply_status_4
    b .L_apply_status_5
    b .L_apply_power
    b .L_apply_defense
    b .L_apply_speed

.L_apply_status_1:
    add r5, sb, #BATTLE_ACTOR_AILMENT_1
    ldrsb r0, [r5, #BATTLE_STATE_KIND]
    cmp r0, #1
    movge r0, #0
    popge {r4, r5, r6, r7, r8, sb, sl, lr}
    bxge lr
    mov r6, #1
    strb r6, [r5, #BATTLE_STATE_KIND]
    ldrh r1, [r5, #BATTLE_STATE_PARAM_2]
    mov r0, #0
    bic r1, r1, #0x7F00
    strh r1, [r5, #BATTLE_STATE_PARAM_2]
    strb r0, [r5, #BATTLE_STATE_PARAM_2]
    strb r0, [r5, #BATTLE_STATE_PARAM_1]
    b .L_finalize

.L_apply_status_2:
    add r5, sb, #BATTLE_ACTOR_AILMENT_1
    ldrsb r0, [r5, #BATTLE_STATE_KIND]
    cmp r0, #2
    movge r0, #0
    popge {r4, r5, r6, r7, r8, sb, sl, lr}
    bxge lr
    mov r6, #2
    strb r6, [r5, #BATTLE_STATE_KIND]
    ldrh r2, [r5, #BATTLE_STATE_PARAM_2]
    mov r1, #0
    mov r0, #20
    bic r2, r2, #0x7F00
    orr r2, r2, #0x3C00
    strh r2, [r5, #BATTLE_STATE_PARAM_2]
    strb r0, [r5, #BATTLE_STATE_PARAM_2]
    mov r2, r1
    mov r3, r1
    mov r0, #0x8C
    strb r6, [r5, #BATTLE_STATE_PARAM_1]
    mov r6, #1
    bl BattleSound_Play
    b .L_finalize

.L_apply_status_3:
    add r5, sb, #BATTLE_ACTOR_AILMENT_1
    ldrsb r0, [r5, #BATTLE_STATE_KIND]
    cmp r0, #3
    movge r0, #0
    popge {r4, r5, r6, r7, r8, sb, sl, lr}
    bxge lr
    mov r0, #3
    strb r0, [r5, #BATTLE_STATE_KIND]
    ldrh r2, [r5, #BATTLE_STATE_PARAM_2]
    mov r1, #0
    mov r0, #30
    bic r2, r2, #0x7F00
    orr r2, r2, #0x2800
    strh r2, [r5, #BATTLE_STATE_PARAM_2]
    strb r0, [r5, #BATTLE_STATE_PARAM_2]
    mov r0, #2
    mov r2, r1
    mov r3, r1
    strb r0, [r5, #BATTLE_STATE_PARAM_1]
    mov r6, #8
    mov r0, #0x78
    strh r6, [r5, #BATTLE_STATE_COUNTER]
    mov r6, #1
    bl BattleSound_Play
    b .L_finalize

.L_apply_status_4:
    add r5, sb, #BATTLE_ACTOR_AILMENT_2
    ldrsb r0, [r5, #BATTLE_STATE_KIND]
    cmp r0, #1
    movge r0, #0
    popge {r4, r5, r6, r7, r8, sb, sl, lr}
    bxge lr
    mov r6, #1
    strb r6, [r5, #BATTLE_STATE_KIND]
    ldrh r2, [r5, #BATTLE_STATE_PARAM_2]
    mov r1, #0
    mov r0, #10
    bic r2, r2, #0x7F00
    orr r2, r2, #0x1400
    strh r2, [r5, #BATTLE_STATE_PARAM_2]
    strb r0, [r5, #BATTLE_STATE_PARAM_2]
    mov r0, #4
    mov r2, r1
    mov r3, r1
    strb r0, [r5, #BATTLE_STATE_PARAM_1]
    mov r7, #5
    mov r0, #0x9E
    strh r7, [r5, #BATTLE_STATE_COUNTER]
    bl BattleSound_Play
    b .L_finalize

.L_apply_status_5:
    add r5, sb, #BATTLE_ACTOR_AILMENT_2
    ldrsb r0, [r5, #BATTLE_STATE_KIND]
    cmp r0, #2
    movge r0, #0
    popge {r4, r5, r6, r7, r8, sb, sl, lr}
    bxge lr
    mov r0, #2
    strb r0, [r5, #BATTLE_STATE_KIND]
    ldrh r2, [r5, #BATTLE_STATE_PARAM_2]
    mov r1, #0
    mov r0, #5
    bic r2, r2, #0x7F00
    orr r2, r2, #0x0A00
    strh r2, [r5, #BATTLE_STATE_PARAM_2]
    strb r0, [r5, #BATTLE_STATE_PARAM_2]
    mov r0, #4
    mov r2, r1
    mov r3, r1
    strb r0, [r5, #BATTLE_STATE_PARAM_1]
    mov r6, #10
    mov r0, #0x9F
    strh r6, [r5, #BATTLE_STATE_COUNTER]
    mov r6, #1
    bl BattleSound_Play
    b .L_finalize

.L_apply_power:
    add r5, sb, #BATTLE_ACTOR_POWER_CHANGE
    cmp r7, #0
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, lr}
    bxeq lr
    strb r7, [r5, #BATTLE_STATE_KIND]
    ldrh r3, [r5, #BATTLE_STATE_PARAM_2]
    mov r2, #10
    mov r1, #4
    bic r3, r3, #0x7F00
    orr r3, r3, #0x0A00
    strh r3, [r5, #BATTLE_STATE_PARAM_2]
    strb r2, [r5, #BATTLE_STATE_PARAM_2]
    strb r1, [r5, #BATTLE_STATE_PARAM_1]
    ldrsh r3, [sb, #BATTLE_ACTOR_BASE_POWER]
    add r1, r7, #100
    ldr r2, .L_divide_by_100_magic
    mul r1, r3, r1
    add r3, r1, #50
    smull r1, r6, r2, r3
    mov r6, r6, asr #5
    mov r1, r3, lsr #31
    add r6, r1, r6
    strh r6, [sb, #BATTLE_ACTOR_POWER]
    ldrsh r1, [sb, #BATTLE_ACTOR_POWER]
    mov r6, #1
    cmp r1, #0
    movlt r1, #0
    strhlt r1, [sb, #BATTLE_ACTOR_POWER]
    ldrsh r2, [sb, #BATTLE_ACTOR_POWER]
    ldr r1, .L_stat_max
    cmp r2, r1
    strhgt r1, [sb, #BATTLE_ACTOR_POWER]
    cmp r0, #0
    beq .L_finalize
    cmp r7, #0
    bge .L_power_positive_sound
    mov r1, #0
    mov r2, r1
    mov r3, r1
    mov r0, #0x70
    bl BattleSound_Play
.L_power_positive_sound:
    cmp r7, #0
    ble .L_finalize
    mov r1, #0
    mov r2, r1
    mov r3, r1
    mov r0, #0x6B
    bl BattleSound_Play
    b .L_finalize

.L_apply_defense:
    add r5, sb, #BATTLE_ACTOR_DEFENSE_CHANGE
    cmp r7, #0
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, lr}
    bxeq lr
    strb r7, [r5, #BATTLE_STATE_KIND]
    ldrh r2, [r5, #BATTLE_STATE_PARAM_2]
    mov r1, #10
    mov r0, #4
    bic r2, r2, #0x7F00
    orr r2, r2, #0x0A00
    strh r2, [r5, #BATTLE_STATE_PARAM_2]
    strb r1, [r5, #BATTLE_STATE_PARAM_2]
    strb r0, [r5, #BATTLE_STATE_PARAM_1]
    ldrsh r2, [sb, #BATTLE_ACTOR_BASE_DEFENSE]
    add r0, r7, #100
    ldr r1, .L_divide_by_100_magic
    mul r0, r2, r0
    add r2, r0, #50
    smull r0, r3, r1, r2
    mov r3, r3, asr #5
    mov r0, r2, lsr #31
    add r3, r0, r3
    strh r3, [sb, #BATTLE_ACTOR_DEFENSE]
    ldrsh r0, [sb, #BATTLE_ACTOR_DEFENSE]
    mov r6, #1
    cmp r0, #0
    movlt r0, #0
    strhlt r0, [sb, #BATTLE_ACTOR_DEFENSE]
    ldrsh r1, [sb, #BATTLE_ACTOR_DEFENSE]
    ldr r0, .L_stat_max
    cmp r1, r0
    strhgt r0, [sb, #BATTLE_ACTOR_DEFENSE]
    cmp r7, #0
    bge .L_defense_positive_sound
    mov r1, #0
    mov r2, r1
    mov r3, r1
    mov r0, #0x70
    bl BattleSound_Play
.L_defense_positive_sound:
    cmp r7, #0
    ble .L_finalize
    mov r1, #0
    mov r2, r1
    mov r3, r1
    mov r0, #0x6B
    bl BattleSound_Play
    b .L_finalize

.L_apply_speed:
    add r5, sb, #BATTLE_ACTOR_SPEED_CHANGE
    cmp r7, #0
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, lr}
    bxeq lr
    strb r7, [r5, #BATTLE_STATE_KIND]
    ldrh r2, [r5, #BATTLE_STATE_PARAM_2]
    mov r1, #10
    mov r0, #4
    bic r2, r2, #0x7F00
    orr r2, r2, #0x0A00
    strh r2, [r5, #BATTLE_STATE_PARAM_2]
    strb r1, [r5, #BATTLE_STATE_PARAM_2]
    strb r0, [r5, #BATTLE_STATE_PARAM_1]
    ldrsh r2, [sb, #BATTLE_ACTOR_BASE_SPEED]
    add r0, r7, #100
    ldr r1, .L_divide_by_100_magic
    mul r0, r2, r0
    add r2, r0, #50
    smull r0, r3, r1, r2
    mov r3, r3, asr #5
    mov r0, r2, lsr #31
    add r3, r0, r3
    strh r3, [sb, #BATTLE_ACTOR_SPEED]
    ldrsh r0, [sb, #BATTLE_ACTOR_SPEED]
    mov r6, #1
    cmp r0, #0
    movlt r0, #0
    strhlt r0, [sb, #BATTLE_ACTOR_SPEED]
    ldrsh r1, [sb, #BATTLE_ACTOR_SPEED]
    ldr r0, .L_stat_max
    cmp r1, r0
    strhgt r0, [sb, #BATTLE_ACTOR_SPEED]
    cmp r7, #0
    bge .L_speed_positive_sound
    mov r1, #0
    mov r2, r1
    mov r3, r1
    mov r0, #0x70
    bl BattleSound_Play
.L_speed_positive_sound:
    cmp r7, #0
    ble .L_finalize
    mov r1, #0
    mov r2, r1
    mov r3, r1
    mov r0, #0x6B
    bl BattleSound_Play
    b .L_finalize

.L_apply_invalid:
    mov r0, #0
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr
.L_finalize:
    strh sl, [r5, #BATTLE_STATE_DURATION]
    ldrh r0, [r5, #BATTLE_STATE_PARAM_2]
    cmp r6, #0
    orr r0, r0, #0x8000
    strh r0, [r5, #BATTLE_STATE_PARAM_2]
    beq .L_return_status
    mov r0, r4
    mov r1, r8
    bl BattleStatus_SpawnEffect
.L_return_status:
    mov r0, r8
    pop {r4, r5, r6, r7, r8, sb, sl, lr}
    bx lr

.L_divide_by_100_magic:
    .word 0x51EB851F
.L_save_data:
    .word gSaveData
.L_equipment_status_guard:
    .word EQUIPMENT_EFFECT_STATUS_GUARD
.L_equipment_power_special:
    .word EQUIPMENT_EFFECT_POWER_SPECIAL
.L_stat_max:
    .word BATTLE_STAT_MAX
.size BattleStatus_TryApply, . - BattleStatus_TryApply
