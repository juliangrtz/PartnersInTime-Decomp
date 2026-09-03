/* Resolve a battle actor pointer from the script-visible actor ID. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleActor_GetById, "ax", %progbits
.balign 4

.global BattleActor_GetById
.type BattleActor_GetById, %function
.extern gBattleContext
BattleActor_GetById:
    sub r1, r0, #56
    lsl r1, r1, #16
    lsr r1, r1, #16
    cmp r1, #4
    bhs .Lenemy

    ldr r0, .L_gBattleContext
    ldr r0, [r0]
    add r0, r0, r1, lsl #2
    add r0, r0, #0x4000
    ldr r0, [r0, #0xFF8]
    bx lr

.Lenemy:
    ldr r1, .L_gBattleContext
    sub r0, r0, #60
    ldr r1, [r1]
    lsl r0, r0, #16
    add r0, r1, r0, lsr #14
    add r0, r0, #0x6000
    ldr r0, [r0, #0x508]
    bx lr

.L_gBattleContext:
    .word gBattleContext

.size BattleActor_GetById, . - BattleActor_GetById
