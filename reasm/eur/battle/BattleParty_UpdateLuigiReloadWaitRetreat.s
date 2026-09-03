/* Finish Luigi's retreat and hand control to the Baby Luigi resource stage. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_UpdateLuigiReloadWaitRetreat, "ax", %progbits
.balign 4

.extern BattleActor_GetPartySlot
.extern BattleParty_UpdateLuigiReloadRequestBaby
.extern BattleSceneObject_GetById
.extern BattleSceneObject_IsAnimationActiveById
.extern BattleSceneObject_SetAnimation
.extern gBattleContext

.global BattleParty_UpdateLuigiReloadWaitRetreat
.type BattleParty_UpdateLuigiReloadWaitRetreat, %function
BattleParty_UpdateLuigiReloadWaitRetreat:
    push {r4, lr}
    ldr r1, .L_battle_context
    mov r4, r0
    ldr r1, [r1]
    mov r0, #0x39
    add r2, r1, #0xd000
    ldr r3, [r2, #0x3a0]
    mov r1, #3
    orr r3, r3, #4
    str r3, [r2, #0x3a0]
    bl BattleSceneObject_IsAnimationActiveById
    cmp r0, #0
    popne {r4, lr}
    bxne lr
    ldr r0, .L_battle_context
    ldr r0, [r0]
    add r0, r0, #0xd000
    ldr r0, [r0, #0x3a0]
    lsl r0, r0, #0xf
    lsrs r0, r0, #0x1f
    beq .L_stop_animation
    mov r0, #0x38
    bl BattleSceneObject_GetById
    mov r1, #0
    strh r1, [r0, #0xea]
.L_stop_animation:
    mov r0, #0x39
    bl BattleSceneObject_GetById
    mvn r1, #0
    mov r2, r1
    bl BattleSceneObject_SetAnimation
    mov r0, #0x39
    bl BattleActor_GetPartySlot
    ldrh r1, [r0, #0x24]
    bic r1, r1, #0x200
    strh r1, [r0, #0x24]
    mov r0, #0x3b
    bl BattleActor_GetPartySlot
    ldrh r2, [r0, #0x24]
    ldr r1, .L_next_callback
    orr r2, r2, #0x200
    strh r2, [r0, #0x24]
    str r1, [r4, #4]
    pop {r4, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_next_callback:
    .word BattleParty_UpdateLuigiReloadRequestBaby
.size BattleParty_UpdateLuigiReloadWaitRetreat, . - BattleParty_UpdateLuigiReloadWaitRetreat
