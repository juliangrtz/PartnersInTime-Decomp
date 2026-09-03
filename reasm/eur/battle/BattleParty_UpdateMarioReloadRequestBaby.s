/* Wait for the transition state, then request Baby Mario's object resource. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_UpdateMarioReloadRequestBaby, "ax", %progbits
.balign 4

.extern BattleObjectData_EnsureLoaded
.extern BattleParty_UpdateMarioReloadFinish
.extern gBattleContext

.global BattleParty_UpdateMarioReloadRequestBaby
.type BattleParty_UpdateMarioReloadRequestBaby, %function
BattleParty_UpdateMarioReloadRequestBaby:
    push {r4, lr}
    ldr r2, .L_battle_context
    ldr r1, .L_required_state
    ldr r2, [r2]
    mov r4, r0
    ldr r0, [r2, #0x18]
    cmp r0, r1
    popne {r4, lr}
    bxne lr
    add r2, r2, #0xd000
    ldr r1, [r2, #0x3a0]
    mov r0, #0
    orr r3, r1, #4
    mov r1, #0xc0000002
    str r3, [r2, #0x3a0]
    bl BattleObjectData_EnsureLoaded
    ldr r0, .L_next_callback
    str r0, [r4, #4]
    pop {r4, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_required_state:
    .word 0x200A
.L_next_callback:
    .word BattleParty_UpdateMarioReloadFinish
.size BattleParty_UpdateMarioReloadRequestBaby, . - BattleParty_UpdateMarioReloadRequestBaby
