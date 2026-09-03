/* Start Mario's off-screen retreat after his rebound animation completes. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_UpdateMarioReloadStartRetreat, "ax", %progbits
.balign 4

.extern BattleParty_UpdateMarioReloadWaitRetreat
.extern BattleSceneObject_GetById
.extern BattleSceneObject_MoveTo
.extern BattleSceneObject_SetAnimation
.extern BattleSound_Play
.extern gBattleContext

.global BattleParty_UpdateMarioReloadStartRetreat
.type BattleParty_UpdateMarioReloadStartRetreat, %function
BattleParty_UpdateMarioReloadStartRetreat:
    push {r4, r5, r6, lr}
    sub sp, sp, #8
    ldr r1, .L_battle_context
    mov r5, r0
    ldr r1, [r1]
    mov r0, #0x38
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
    mov r0, #0x38
    bl BattleSceneObject_GetById
    mov r1, #2
    mvn r2, #0
    bl BattleSceneObject_SetAnimation
    ldrsh r1, [r4, #8]
    ldr ip, .L_divide_by_three
    mov r0, r4
    str r1, [sp]
    ldrsh r3, [r4, #4]
    mov r1, #3
    mvn r2, #0x1f
    add lr, r3, #0x20
    smull r3, r6, ip, lr
    add r6, r6, lr, lsr #31
    str r6, [sp, #4]
    ldrsh r3, [r4, #6]
    bl BattleSceneObject_MoveTo
    mov r1, #0
    mov r0, #0x3d
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
    .word BattleParty_UpdateMarioReloadWaitRetreat
.size BattleParty_UpdateMarioReloadStartRetreat, . - BattleParty_UpdateMarioReloadStartRetreat
