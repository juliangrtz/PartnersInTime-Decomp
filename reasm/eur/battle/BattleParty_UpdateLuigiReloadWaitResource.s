/* Bind Luigi's requested rebound resource after its asynchronous load. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_UpdateLuigiReloadWaitResource, "ax", %progbits
.balign 4

.extern BattleEntity_BindResource
.extern BattleObjectData_IsLoadPending
.extern BattleParty_UpdateLuigiReloadWaitAnimation
.extern BattleSceneObject_GetById
.extern BattleSceneObject_SetAnimation
.extern BattleSound_Play
.extern gBattleContext

.global BattleParty_UpdateLuigiReloadWaitResource
.type BattleParty_UpdateLuigiReloadWaitResource, %function
BattleParty_UpdateLuigiReloadWaitResource:
    push {r4, lr}
    ldr r1, .L_battle_context
    mov r4, r0
    ldr r1, [r1]
    mov r0, #6
    add r1, r1, #0xd000
    ldr r2, [r1, #0x3a0]
    orr r2, r2, #4
    str r2, [r1, #0x3a0]
    bl BattleObjectData_IsLoadPending
    cmp r0, #0
    popne {r4, lr}
    bxne lr
    mov r0, #0x39
    mov r1, #6
    bl BattleEntity_BindResource
    mov r0, #0x39
    bl BattleSceneObject_GetById
    mov r1, #0
    mvn r2, #0
    bl BattleSceneObject_SetAnimation
    mov r0, #0x39
    bl BattleSceneObject_GetById
    mov r0, #0xda
    mov r1, #0
    mov r2, r1
    mov r3, r1
    bl BattleSound_Play
    ldr r0, .L_next_callback
    str r0, [r4, #4]
    pop {r4, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_next_callback:
    .word BattleParty_UpdateLuigiReloadWaitAnimation
.size BattleParty_UpdateLuigiReloadWaitResource, . - BattleParty_UpdateLuigiReloadWaitResource
