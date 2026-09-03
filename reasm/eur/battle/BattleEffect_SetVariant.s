/* Set the signed battle-effect variant selected by attacks and battle scripts. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleEffect_SetVariant, "ax", %progbits
.balign 4

.equ BATTLE_CONTEXT_EFFECT_VARIANT, 0xCB7A

.extern gBattleContext

.global BattleEffect_SetVariant
.type BattleEffect_SetVariant, %function
BattleEffect_SetVariant:
    ldr r1, .L_battle_context
    ldr r1, [r1]
    add r1, r1, #0xCB00
    strh r0, [r1, #0x7A]
    bx lr
.L_battle_context:
    .word gBattleContext
.size BattleEffect_SetVariant, . - BattleEffect_SetVariant
