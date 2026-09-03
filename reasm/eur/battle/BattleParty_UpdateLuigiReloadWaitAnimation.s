/* Wait for Luigi's rebound resource to animate, then advance the KO task. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_UpdateLuigiReloadWaitAnimation, "ax", %progbits
.balign 4

.extern BattleSceneObject_GetById
.extern BattleSceneObject_SetAnimation
.extern func_ov002_020a8aa4
.extern gBattleContext

.global BattleParty_UpdateLuigiReloadWaitAnimation
.type BattleParty_UpdateLuigiReloadWaitAnimation, %function
BattleParty_UpdateLuigiReloadWaitAnimation:
    push {r4, lr}
    ldr r1, .L_battle_context
    mov r4, r0
    ldr r1, [r1]
    mov r0, #0x39
    add r1, r1, #0xd000
    ldr r2, [r1, #0x3a0]
    orr r2, r2, #4
    str r2, [r1, #0x3a0]
    bl BattleSceneObject_GetById
    ldr r0, [r0, #0xc0]
    ldr r0, [r0, #0x7c]
    lsl r0, r0, #0x1d
    lsrs r0, r0, #0x1f
    popeq {r4, lr}
    bxeq lr
    mov r0, #0x39
    bl BattleSceneObject_GetById
    mov r1, #1
    mvn r2, #0
    bl BattleSceneObject_SetAnimation
    ldr r0, .L_next_callback
    str r0, [r4, #4]
    pop {r4, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_next_callback:
    .word func_ov002_020a8aa4
.size BattleParty_UpdateLuigiReloadWaitAnimation, . - BattleParty_UpdateLuigiReloadWaitAnimation
