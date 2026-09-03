/* Start the fourth fixed party VM state from its battle-context script block. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartPartyVmSlot4, "ax", %progbits
.balign 4

.extern BattleAI_InitStateFromScriptBlock
.extern gBattleContext

.global BattleAI_StartPartyVmSlot4
.type BattleAI_StartPartyVmSlot4, %function
BattleAI_StartPartyVmSlot4:
    ldr r1, .L_battle_context
    ldr r0, .L_state_offset
    ldr r2, [r1]
    ldr r1, .L_script_offset
    ldr ip, .L_initializer
    add r0, r2, r0
    add r1, r2, r1
    mov r2, #4
    bx ip
.L_battle_context:
    .word gBattleContext
.L_state_offset:
    .word 0x6c8c
.L_script_offset:
    .word 0x35608
.L_initializer:
    .word BattleAI_InitStateFromScriptBlock
.size BattleAI_StartPartyVmSlot4, . - BattleAI_StartPartyVmSlot4
