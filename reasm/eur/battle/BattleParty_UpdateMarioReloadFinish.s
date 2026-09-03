/* Restore Mario and Baby Mario after the linked knockout resource transition. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_UpdateMarioReloadFinish, "ax", %progbits
.balign 4

.extern BattleActor_GetPartySlot
.extern BattleEntity_BindResource
.extern BattleObjectData_IsLoadPending
.extern BattleSceneObject_GetById
.extern BattleSceneObject_SetAnimation
.extern BattleStatus_TryApply
.extern func_ov002_02090c18
.extern gBattleContext
.extern gSaveData

.global BattleParty_UpdateMarioReloadFinish
.type BattleParty_UpdateMarioReloadFinish, %function
BattleParty_UpdateMarioReloadFinish:
    push {r4, r5, lr}
    sub sp, sp, #4
    ldr r1, .L_battle_context
    mov r5, r0
    ldr r1, [r1]
    mov r0, #0
    add r1, r1, #0xd000
    ldr r2, [r1, #0x3a0]
    orr r2, r2, #4
    str r2, [r1, #0x3a0]
    bl BattleObjectData_IsLoadPending
    cmp r0, #0
    addne sp, sp, #4
    popne {r4, r5, lr}
    bxne lr
    mov r0, #0x38
    bl BattleSceneObject_GetById
    mov r4, r0
    mov r0, #0x3a
    bl BattleSceneObject_GetById
    ldrsh r3, [r4, #4]
    mov r2, #0
    mov r1, #0x38
    strh r3, [r0, #0xa]
    ldrsh r3, [r0, #0xa]
    strh r3, [r0, #0x10]
    ldrsh r3, [r0, #0x10]
    strh r3, [r0, #4]
    ldrsh r3, [r4, #6]
    strh r3, [r0, #0xc]
    ldrsh r3, [r0, #0xc]
    strh r3, [r0, #0x12]
    ldrsh r3, [r0, #0x12]
    strh r3, [r0, #6]
    strh r2, [r0, #0xe]
    ldrsh r2, [r0, #0xe]
    strh r2, [r0, #0x14]
    ldrsh r2, [r0, #0x14]
    strh r2, [r0, #8]
    mov r0, #0x3a
    bl func_ov002_02090c18
    mov r0, #0x38
    bl BattleActor_GetPartySlot
    ldrh r1, [r0, #0x24]
    bic r1, r1, #0x200
    strh r1, [r0, #0x24]
    mov r0, #0x3a
    bl BattleActor_GetPartySlot
    ldrh r2, [r0, #0x24]
    mov r1, #0
    bic r2, r2, #0x200
    strh r2, [r0, #0x24]
    mov r0, #0x38
    bl BattleEntity_BindResource
    mov r0, #0x38
    bl BattleSceneObject_GetById
    mov r1, #2
    mvn r2, #0
    bl BattleSceneObject_SetAnimation
    mov r0, #0x3a
    mov r1, #0
    bl BattleEntity_BindResource
    mov r0, #0x3a
    bl BattleSceneObject_GetById
    mvn r1, #0
    mov r2, r1
    bl BattleSceneObject_SetAnimation
    mov r0, #0x38
    bl BattleActor_GetPartySlot
    mov r4, r0
    ldrh r1, [r4, #0x74]
    lsl r1, r1, #0x1b
    lsrs r1, r1, #0x1f
    bne .L_finish
    ldr r2, [r4, #0x6c]
    ldr r1, .L_save_data
    ldrh r2, [r2]
    ldr r3, [r1]
    mov r1, #0x24
    and r2, r2, #0xff
    mla r1, r2, r1, r3
    ldrb r2, [r1, #0x418]
    ldr r1, .L_equipment_effect
    orr r2, r2, #0x3000
    cmp r2, r1
    bne .L_finish
    ldr r2, .L_always_chance
    mov ip, #0x65
    mov r1, #6
    mov r3, #0x28
    str ip, [sp]
    bl BattleStatus_TryApply
    mov r0, #0
    strb r0, [r4, #0x51]
.L_finish:
    mov r0, #0
    str r0, [r5, #4]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_save_data:
    .word gSaveData
.L_equipment_effect:
    .word 0x3024
.L_always_chance:
    .word 0x7FFF
.size BattleParty_UpdateMarioReloadFinish, . - BattleParty_UpdateMarioReloadFinish
