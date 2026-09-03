/* Advance party knockout completion and linked-character transitions. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_UpdateKnockout, "ax", %progbits
.balign 4

.extern BattleActor_GetPartySlot
.extern BattleSceneObject_GetById
.extern BattleSceneObject_SetAnimation
.extern BattleObjectData_EnsureLoaded
.extern BattleSceneObject_AdjustPosition
.extern func_ov002_020a8bec
.extern func_ov002_020a906c
.extern gBattleContext

.global BattleParty_UpdateKnockout
.type BattleParty_UpdateKnockout, %function
BattleParty_UpdateKnockout:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    ldr r1, .L_battle_context
    mov r6, r0
    ldr r0, [r1]
    add r5, r6, #0x0C
    add r0, r0, #0xD000
    ldr r1, [r0, #0x3A0]
    orr r1, r1, #4
    str r1, [r0, #0x3A0]
    ldrh r0, [r5, #4]
    bl BattleActor_GetPartySlot
    mov r4, r0
    ldr r0, [r4]
    ldr r0, [r0, #0xC0]
    cmp r0, #0
    beq .L_animation_finished
    ldr r0, [r0, #0x7C]
    mov r1, r0, lsl #23
    movs r1, r1, lsr #31
    beq .L_animation_finished
    mov r0, r0, lsl #29
    movs r0, r0, lsr #31
    addeq sp, sp, #4
    popeq {r4, r5, r6, r7, lr}
    bxeq lr
.L_animation_finished:
    ldrh r0, [r4, #0x7E]
    cmp r0, #4
    beq .L_linked_form
    cmp r0, #5
    beq .L_linked_form
    ldrh r1, [r4, #0x24]
    mov r0, #0
    add sp, sp, #4
    bic r1, r1, #0x200
    strh r1, [r4, #0x24]
    str r0, [r6, #4]
    pop {r4, r5, r6, r7, lr}
    bx lr

.L_linked_form:
    ldr r0, .L_battle_context
    ldr r0, [r0]
    add r0, r0, #0xD000
    ldr r0, [r0, #0x3A0]
    mov r0, r0, lsl #15
    movs r0, r0, lsr #31
    beq .L_select_party_load
    ldrh r0, [r5, #4]
    bl BattleSceneObject_GetById
    mov r7, r0
    ldrh r0, [r4, #0x80]
    bl BattleSceneObject_GetById
    mov r4, r0
    mov r1, #0x0E
    mvn r2, #0
    bl BattleSceneObject_SetAnimation
    mov r0, r4
    ldrsh lr, [r7, #4]
    ldrsh ip, [r7, #6]
    ldrsh r1, [r4, #4]
    ldrsh r2, [r4, #6]
    ldrsh r3, [r4, #8]
    sub r1, lr, r1
    sub r2, ip, r2
    rsb r3, r3, #0
    bl BattleSceneObject_AdjustPosition
    mov r0, #0x30
    strh r0, [r4, #0xEA]

.L_select_party_load:
    ldrh r0, [r5, #4]
    cmp r0, #0x38
    bne .L_luigi
    mov r0, #5
    mov r1, #0xC000003B
    bl BattleObjectData_EnsureLoaded
    ldr r0, .L_battle_context
    mov r2, #3
    ldr r0, [r0]
    ldr r1, .L_mario_callback
    add r0, r0, #0x6500
    strh r2, [r0, #0x7E]
    add sp, sp, #4
    str r1, [r6, #4]
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_luigi:
    mov r0, #6
    mov r1, #0xC000003C
    bl BattleObjectData_EnsureLoaded
    ldr r0, .L_battle_context
    mov r2, #3
    ldr r0, [r0]
    ldr r1, .L_luigi_callback
    add r0, r0, #0x6500
    strh r2, [r0, #0x9A]
    str r1, [r6, #4]
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_mario_callback:
    .word func_ov002_020a906c
.L_luigi_callback:
    .word func_ov002_020a8bec
.size BattleParty_UpdateKnockout, . - BattleParty_UpdateKnockout
