/* Start Luigi's off-screen retreat after his rebound animation completes. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_UpdateLuigiReloadStartRetreat, "ax", %progbits
.balign 4

.extern BattleParty_UpdateLuigiReloadWaitRetreat
.extern BattleSceneObject_AdjustPosition
.extern BattleSceneObject_GetById
.extern BattleSceneObject_MoveTo
.extern BattleSceneObject_SetAnimation
.extern BattleSound_Play
.extern gBattleContext

.global BattleParty_UpdateLuigiReloadStartRetreat
.type BattleParty_UpdateLuigiReloadStartRetreat, %function
BattleParty_UpdateLuigiReloadStartRetreat:
    push {r4, r5, r6, lr}
    sub sp, sp, #8
    ldr r1, .L_battle_context
    mov r5, r0
    ldr r1, [r1]
    mov r0, #0x39
    add r1, r1, #0xd000
    ldr r2, [r1, #0x3a0]
    orr r2, r2, #4
    str r2, [r1, #0x3a0]
    bl BattleSceneObject_GetById
    mov r4, r0
    ldr r0, [r4, #0xc0]
    ldr r0, [r0, #0x7c]
    lsl r0, r0, #0x1d
    lsrs r0, r0, #0x1f
    addeq sp, sp, #8
    popeq {r4, r5, r6, lr}
    bxeq lr
    mov r0, #0x39
    bl BattleSceneObject_GetById
    mov r1, #2
    mvn r2, #0
    bl BattleSceneObject_SetAnimation
    ldrsh r0, [r4, #4]
    mov r2, #0
    ldr r1, .L_divide_by_three
    add ip, r0, #0x24
    smull r0, r6, r1, ip
    mov r0, r4
    mov r3, r2
    add r6, r6, ip, lsr #31
    mvn r1, #6
    bl BattleSceneObject_AdjustPosition
    ldrsh r2, [r4, #8]
    mov r0, r4
    mov r1, #3
    str r2, [sp]
    str r6, [sp, #4]
    ldrsh r3, [r4, #6]
    mvn r2, #0x23
    bl BattleSceneObject_MoveTo
    mov r0, #0x46
    mov r1, #0
    mov r2, r1
    mov r3, r1
    bl BattleSound_Play
    ldr r0, .L_next_callback
    str r0, [r5, #4]
    add sp, sp, #8
    pop {r4, r5, r6, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_divide_by_three:
    .word 0x55555556
.L_next_callback:
    .word BattleParty_UpdateLuigiReloadWaitRetreat
.size BattleParty_UpdateLuigiReloadStartRetreat, . - BattleParty_UpdateLuigiReloadStartRetreat
