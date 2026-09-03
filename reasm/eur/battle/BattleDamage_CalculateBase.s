/* Calculate base damage from attacker level/POW, a Q8 scale, and defender DEF. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_CalculateBase, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_POWER, 0x0A
.equ BATTLE_ACTOR_DEFENSE, 0x0C
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_ACTOR_LEVEL_MASK, 0x7F

.extern BattleActor_GetById
.extern _s32_div_f

.global BattleDamage_CalculateBase
.type BattleDamage_CalculateBase, %function
BattleDamage_CalculateBase:
    push {r4, r5, r6, lr}
    mov r6, r1
    mov r5, r2
    bl BattleActor_GetById
    mov r4, r0
    mov r0, r6
    bl BattleActor_GetById

    ldrsh r1, [r4, #BATTLE_ACTOR_POWER]
    mov r3, r0
    ldrh r2, [r4, #BATTLE_ACTOR_FLAGS]
    mul r0, r1, r5
    mov r1, r2, lsl #25
    mov r1, r1, lsr #25
    mul r0, r1, r0
    ldrsh r1, [r3, #BATTLE_ACTOR_DEFENSE]
    bl _s32_div_f

    add r1, r0, #0x80
    mov r0, r1, asr #7
    add r0, r1, r0, lsr #24
    ldr r1, .L_max_damage
    mov r0, r0, asr #8
    cmp r0, r1
    movgt r0, r1
    pop {r4, r5, r6, lr}
    bx lr

.L_max_damage:
    .word 999
.size BattleDamage_CalculateBase, . - BattleDamage_CalculateBase
