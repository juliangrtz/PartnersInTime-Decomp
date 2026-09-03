/* Return one of the battle context's fixed-size object-data load states. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleObjectData_GetLoadState, "ax", %progbits
.balign 4

.extern gBattleContext

.global BattleObjectData_GetLoadState
.type BattleObjectData_GetLoadState, %function
BattleObjectData_GetLoadState:
    ldr r2, .L_battle_context
    ldr r1, .L_load_state_offset
    ldr r3, [r2]
    mov r2, #0x30
    add r1, r3, r1
    mla r0, r2, r0, r1
    bx lr
.L_battle_context:
    .word gBattleContext
.L_load_state_offset:
    .word 0xD3C8
.size BattleObjectData_GetLoadState, . - BattleObjectData_GetLoadState
