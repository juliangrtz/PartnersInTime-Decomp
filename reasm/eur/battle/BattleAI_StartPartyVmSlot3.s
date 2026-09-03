/* Start the third fixed party VM state from its selected script block. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartPartyVmSlot3, "ax", %progbits
.balign 4

.extern BattleAI_InitStateFromScriptBlock
.extern gBattleContext

.global BattleAI_StartPartyVmSlot3
.type BattleAI_StartPartyVmSlot3, %function
BattleAI_StartPartyVmSlot3:
    ldr r1, .L_battle_context
    ldr r0, .L_state_offset
    ldr r3, [r1]
    ldr ip, .L_initializer
    add r1, r3, #0xd000
    mov r2, #3
    ldr r1, [r1, #0x3c4]
    add r0, r3, r0
    bx ip
.L_battle_context:
    .word gBattleContext
.L_state_offset:
    .word 0x6bd4
.L_initializer:
    .word BattleAI_InitStateFromScriptBlock
.size BattleAI_StartPartyVmSlot3, . - BattleAI_StartPartyVmSlot3
