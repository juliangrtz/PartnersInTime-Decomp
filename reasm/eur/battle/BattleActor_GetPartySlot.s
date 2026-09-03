/* First maintained symbolic function in the PiT EUR reassembly. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleActor_GetPartySlot, "ax", %progbits
.balign 4

.global BattleActor_GetPartySlot
.type BattleActor_GetPartySlot, %function
.extern gBattleContext
BattleActor_GetPartySlot:
    ldr r1, .L_gBattleContext
    sub r0, r0, #56
    ldr r1, [r1]
    add r0, r1, r0, lsl #2
    add r0, r0, #0x4000
    ldr r0, [r0, #0xFF8]
    bx lr

.L_gBattleContext:
    .word gBattleContext

.size BattleActor_GetPartySlot, . - BattleActor_GetPartySlot
