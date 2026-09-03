/* Apply the equipped healing badge multiplier to a proposed HP value. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleItemEffect_ApplyBadgeBoost, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_MAX_HP, 0x04
.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ BATTLE_ACTOR_LOADOUT, 0x6C
.equ BATTLE_CONTEXT_ACTIVE_ACTOR_ID, 0x20
.equ ITEM_RECORD_SIZE, 0x24
.equ ITEM_EFFECT_OFFSET, 0x418
.equ BADGE_HEAL_BOOST_50, 0x3008
.equ BADGE_HEAL_BOOST_100, 0x3010

.extern BattleActor_GetPartySlot
.extern gBattleContext
.extern gSaveData

.global BattleItemEffect_ApplyBadgeBoost
.type BattleItemEffect_ApplyBadgeBoost, %function
BattleItemEffect_ApplyBadgeBoost:
    push {r4, r5, lr}
    sub sp, sp, #4
    ldr r2, .L_battle_context
    mov r5, r0
    ldr r0, [r2]
    mov r4, r1
    ldrh r0, [r0, #BATTLE_CONTEXT_ACTIVE_ACTOR_ID]
    bl BattleActor_GetPartySlot

    ldr r1, [r0, #BATTLE_ACTOR_LOADOUT]
    ldr r0, .L_save_data
    ldrh r1, [r1]
    ldr r2, [r0]
    mov r0, #ITEM_RECORD_SIZE
    and r1, r1, #0xFF
    mla r0, r1, r0, r2
    ldrb r1, [r0, #ITEM_EFFECT_OFFSET]
    ldr r0, .L_badge_heal_boost_50
    orr r1, r1, #0x3000
    cmp r1, r0
    beq .L_apply_50_percent_boost

    ldr r0, .L_badge_heal_boost_100
    cmp r1, r0
    bne .L_done

    ldrsh ip, [r5, #BATTLE_ACTOR_CURRENT_HP]
    mov r0, #200
    ldr r1, .L_divide_by_100_magic
    sub r2, r4, ip
    mul r0, r2, r0
    add r2, r0, #50
    smull r0, r3, r1, r2
    mov r3, r3, asr #5
    mov r0, r2, lsr #31
    add r3, r0, r3
    ldrsh r0, [r5, #BATTLE_ACTOR_MAX_HP]
    add r4, ip, r3
    cmp r0, r4
    movlt r4, r0
    b .L_done

.L_apply_50_percent_boost:
    ldrsh ip, [r5, #BATTLE_ACTOR_CURRENT_HP]
    mov r0, #150
    ldr r1, .L_divide_by_100_magic
    sub r2, r4, ip
    mul r0, r2, r0
    add r2, r0, #50
    smull r0, r3, r1, r2
    mov r3, r3, asr #5
    mov r0, r2, lsr #31
    add r3, r0, r3
    ldrsh r0, [r5, #BATTLE_ACTOR_MAX_HP]
    add r4, ip, r3
    cmp r0, r4
    movlt r4, r0

.L_done:
    mov r0, r4
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.L_save_data:
    .word gSaveData
.L_badge_heal_boost_50:
    .word BADGE_HEAL_BOOST_50
.L_badge_heal_boost_100:
    .word BADGE_HEAL_BOOST_100
.L_divide_by_100_magic:
    .word 0x51EB851F
.size BattleItemEffect_ApplyBadgeBoost, . - BattleItemEffect_ApplyBadgeBoost
