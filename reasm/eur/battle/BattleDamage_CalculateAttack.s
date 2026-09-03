/* Calculate scaled attack damage from live battle actors and modifier tables. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_CalculateAttack, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_POWER, 0x0A
.equ BATTLE_ACTOR_DEFENSE, 0x0C
.equ BATTLE_ACTOR_TARGET_ID, 0x20
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_ACTOR_LEVEL_MASK, 0x7F
.equ BATTLE_CONTEXT_DAMAGE_SCALE, 0x13C
.equ BATTLE_DAMAGE_RAW_MIN, 0x100
.equ BATTLE_DAMAGE_RAW_SPECIAL_MIN, 0x200
.equ BATTLE_DAMAGE_RAW_MAX, 0x270F00
.equ BATTLE_DAMAGE_MAX, 9999

.extern BattleActor_GetById
.extern _s32_div_f
.extern gBattleContext
.extern gBattleDamageMultipliersQ8

.global BattleDamage_CalculateAttack
.type BattleDamage_CalculateAttack, %function
BattleDamage_CalculateAttack:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    movs r4, r3
    mov r7, r0
    mov r6, r1
    mov r5, r2
    bne .L_have_defender
    ldrh r0, [r7, #BATTLE_ACTOR_TARGET_ID]
    bl BattleActor_GetById
    mov r4, r0

.L_have_defender:
    ldrsh r0, [r7, #BATTLE_ACTOR_POWER]
    ldrh r3, [r7, #BATTLE_ACTOR_FLAGS]
    ldrsh r1, [r4, #BATTLE_ACTOR_DEFENSE]
    mul r2, r0, r6
    mov r0, r3, lsl #25
    mov r3, r0, lsr #25
    mul r0, r3, r2
    bl _s32_div_f

    add r1, r5, #1
    cmp r1, #7
    addls pc, pc, r1, lsl #2
    b .L_special_min_done
    b .L_special_min_done
    b .L_special_min_done
    b .L_apply_special_min
    b .L_special_min_done
    b .L_special_min_done
    b .L_special_min_done
    b .L_special_min_done
    b .L_apply_special_min

.L_apply_special_min:
    ldr r1, [sp, #0x18]
    cmp r1, #0
    bne .L_special_min_done
    cmp r0, #BATTLE_DAMAGE_RAW_SPECIAL_MIN
    movlt r0, #BATTLE_DAMAGE_RAW_SPECIAL_MIN

.L_special_min_done:
    add r1, r5, #1
    cmp r1, #7
    mov r3, #1
    addls pc, pc, r1, lsl #2
    b .L_category_ready
    b .L_category_zero
    b .L_category_two
    b .L_category_one
    b .L_category_ready
    b .L_category_ready
    b .L_category_ready
    b .L_category_ready
    b .L_category_four

.L_category_zero:
    mov r3, #0
.L_category_one:
    b .L_category_ready
.L_category_two:
    mov r3, #2
    b .L_category_ready
.L_category_four:
    mov r3, #4

.L_category_ready:
    ldr r1, .L_raw_damage_max
    cmp r0, #BATTLE_DAMAGE_RAW_MIN
    movlt r0, #BATTLE_DAMAGE_RAW_MIN
    cmp r0, r1
    movgt r0, r1

    ldr r2, [sp, #0x18]
    mov r1, #10
    mul r1, r2, r1
    ldr r2, .L_primary_multipliers
    mov r3, r3, lsl #1
    add r2, r2, r1
    ldrsh r3, [r3, r2]
    ldr r2, [sp, #0x1C]
    mul r3, r0, r3
    mov r0, r3, asr #7
    add r0, r3, r0, lsr #24
    cmp r2, #0
    mov r0, r0, asr #8
    beq .L_secondary_scale_done

    ldr r2, .L_secondary_multipliers
    ldrsh r1, [r2, r1]
    mul r1, r0, r1
    mov r0, r1, asr #7
    add r0, r1, r0, lsr #24
    mov r0, r0, asr #8

.L_secondary_scale_done:
    add r1, r0, #0x80
    mov r0, r1, asr #7
    add r0, r1, r0, lsr #24
    mov r0, r0, asr #8
    cmp r0, #1
    ldr r1, .L_damage_max
    movlt r0, #1
    cmp r0, r1
    movgt r0, r1

    ldr r1, .L_battle_context
    ldrh r3, [r4, #BATTLE_ACTOR_FLAGS]
    ldr r1, [r1]
    ldr r2, .L_divide_by_100_magic
    add r1, r1, #0x100
    ldrsh r4, [r1, #0x3C]
    mov r1, r3, lsl #17
    movs r1, r1, lsr #31
    mul r1, r0, r4
    add r3, r1, #50
    smull r1, r0, r2, r3
    mov r0, r0, asr #5
    mov r1, r3, lsr #31
    add r0, r1, r0
    movne r0, #1

    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr

.L_raw_damage_max:
    .word BATTLE_DAMAGE_RAW_MAX
.L_primary_multipliers:
    .word gBattleDamageMultipliersQ8
.L_secondary_multipliers:
    .word gBattleDamageMultipliersQ8 + 6
.L_damage_max:
    .word BATTLE_DAMAGE_MAX
.L_battle_context:
    .word gBattleContext
.L_divide_by_100_magic:
    .word 0x51EB851F
.size BattleDamage_CalculateAttack, . - BattleDamage_CalculateAttack
