/* Resolve field, party, enemy, and auxiliary battle scene-object IDs. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_GetById, "ax", %progbits
.balign 4

.extern gBattleContext

.global BattleSceneObject_GetById
.type BattleSceneObject_GetById, %function
BattleSceneObject_GetById:
    cmp r0, #0x38
    bhs .L_party_or_later
    ldr r1, .L_battle_context
    ldr r1, [r1]
    add r0, r1, r0, lsl #2
    add r0, r0, #0x4000
    ldr r0, [r0, #0xCC8]
    bx lr
.L_party_or_later:
    sub r1, r0, #0x38
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    cmp r1, #4
    bhs .L_enemy_or_later
    ldr r0, .L_battle_context
    ldr r0, [r0]
    add r0, r0, r1, lsl #2
    add r0, r0, #0x4000
    ldr r0, [r0, #0xFF8]
    ldr r0, [r0]
    bx lr
.L_enemy_or_later:
    sub r1, r0, #0x3C
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    cmp r1, #8
    bhs .L_auxiliary
    ldr r0, .L_battle_context
    ldr r0, [r0]
    add r0, r0, r1, lsl #2
    add r0, r0, #0x6000
    ldr r0, [r0, #0x508]
    ldr r0, [r0]
    bx lr
.L_auxiliary:
    ldr r1, .L_battle_context
    sub r0, r0, #0x44
    ldr r1, [r1]
    mov r0, r0, lsl #16
    add r0, r1, r0, lsr #14
    add r0, r0, #0x6000
    ldr r0, [r0, #0x528]
    bx lr
.L_battle_context:
    .word gBattleContext
.size BattleSceneObject_GetById, . - BattleSceneObject_GetById
