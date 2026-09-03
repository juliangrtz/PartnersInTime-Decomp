/* Resolve a battle object ID to its fixed 192-byte script-state record. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleScriptState_GetByObjectId, "ax", %progbits
.balign 4

.extern gBattleContext

.global BattleScriptState_GetByObjectId
.type BattleScriptState_GetByObjectId, %function
BattleScriptState_GetByObjectId:
    cmp r0, #0x28
    sublo r0, r0, #0x1c
    lsllo r0, r0, #0x10
    lsrlo r0, r0, #0x10
    blo .L_resolve
    cmp r0, #0x38
    sublo r0, r0, #0x1c
    lsllo r0, r0, #0x10
    lsrlo r0, r0, #0x10
    blo .L_resolve
    cmp r0, #0x3c
    sublo r0, r0, #0x1c
    lsllo r0, r0, #0x10
    lsrlo r0, r0, #0x10
    blo .L_resolve
    cmp r0, #0x44
    sublo r0, r0, #0x1c
    lsllo r0, r0, #0x10
    lsrlo r0, r0, #0x10
.L_resolve:
    ldr r2, .L_battle_context
    ldr r1, .L_states_offset
    ldr r3, [r2]
    mov r2, #0xc0
    add r1, r3, r1
    mla r0, r2, r0, r1
    bx lr
.L_battle_context:
    .word gBattleContext
.L_states_offset:
    .word 0x6d44
.size BattleScriptState_GetByObjectId, . - BattleScriptState_GetByObjectId
