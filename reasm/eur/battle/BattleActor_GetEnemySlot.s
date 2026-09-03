/* Resolve one enemy actor ID (60-67) through the battle context. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleActor_GetEnemySlot, "ax", %progbits
.balign 4

.equ ENEMY_ACTOR_ID_FIRST, 60
.equ BATTLE_CONTEXT_ENEMY_SLOTS, 0x6508

.extern gBattleContext

.global BattleActor_GetEnemySlot
.type BattleActor_GetEnemySlot, %function
BattleActor_GetEnemySlot:
    ldr r1, .L_battle_context
    sub r0, r0, #ENEMY_ACTOR_ID_FIRST
    ldr r1, [r1]
    add r0, r1, r0, lsl #2
    add r0, r0, #0x6000
    ldr r0, [r0, #0x508]
    bx lr
.L_battle_context:
    .word gBattleContext
.size BattleActor_GetEnemySlot, . - BattleActor_GetEnemySlot
