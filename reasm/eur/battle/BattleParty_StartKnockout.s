/* Start a party actor's knockout task, animation, sounds, and global state. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_StartKnockout, "ax", %progbits
.balign 4

.extern BattleActor_GetPartySlot
.extern BattleParty_UpdateKnockout
.extern BattleSceneObject_SetAnimation
.extern BattleSceneObject_SetStateFlags
.extern BattleScreenEffect_StartPrimary
.extern BattleSound_Play
.extern BattleStatus_ClearAll
.extern BattleTaskList_Insert
.extern gBattleContext

.global BattleParty_StartKnockout
.type BattleParty_StartKnockout, %function
BattleParty_StartKnockout:
    push {r4, r5, r6, lr}
    ldr r2, .L_battle_context
    ldr r1, .L_task_pool_offset
    ldr r2, [r2]
    mov r6, r0
    add r0, r2, r1
    mov r1, #0
    bl BattleTaskList_Insert
    mov r5, r0
    ldrh r0, [r6, #0xEC]
    bl BattleActor_GetPartySlot
    mov r4, r0
    bl BattleStatus_ClearAll
    mov r0, r6
    mov r1, #0x0D
    mvn r2, #0
    bl BattleSceneObject_SetAnimation
    mov r0, r6
    mov r1, #1
    mov r2, #0
    bl BattleSceneObject_SetStateFlags
    ldrh r2, [r6, #0xEC]
    ldr r0, .L_battle_context
    ldr r1, .L_update_callback
    strh r2, [r5, #0x10]
    ldrh r2, [r4, #0x24]
    orr r2, r2, #0x200
    strh r2, [r4, #0x24]
    ldr r0, [r0]
    add r0, r0, #0xD000
    ldr r2, [r0, #0x3A0]
    orr r2, r2, #0x200
    str r2, [r0, #0x3A0]
    ldrh r0, [r4, #0x24]
    bic r0, r0, #0x800
    strh r0, [r4, #0x24]
    str r1, [r5, #4]

    ldrh r0, [r4, #0x7E]
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_after_sounds
    b .L_form_0
    b .L_form_1
    b .L_form_2
    b .L_form_3
    b .L_form_4
    b .L_form_5
.L_form_0:
    mov r2, #0
    mov r3, r2
    mov r0, #0x3E
    mov r1, #0x0C
    bl BattleSound_Play
    mov r1, #0
    ldr r0, .L_sound_4023
    mov r2, r1
    mov r3, r1
    bl BattleSound_Play
    b .L_after_sounds
.L_form_1:
    mov r2, #0
    mov r3, r2
    mov r0, #0x3E
    mov r1, #0x23
    bl BattleSound_Play
    mov r1, #0
    ldr r0, .L_sound_404b
    mov r2, r1
    mov r3, r1
    bl BattleSound_Play
    b .L_after_sounds
.L_form_2:
    mov r2, #0
    mov r3, r2
    mov r0, #0x3E
    mov r1, #0x0D
    bl BattleSound_Play
    mov r1, #0
    ldr r0, .L_sound_4060
    mov r2, r1
    mov r3, r1
    bl BattleSound_Play
    b .L_after_sounds
.L_form_3:
    mov r2, #0
    mov r3, r2
    mov r0, #0x3E
    mov r1, #0x28
    bl BattleSound_Play
    mov r1, #0
    ldr r0, .L_sound_4073
    mov r2, r1
    mov r3, r1
    bl BattleSound_Play
    b .L_after_sounds
.L_form_4:
    mov r2, #0
    mov r3, r2
    mov r0, #0x3E
    mov r1, #0x0A
    bl BattleSound_Play
    mov r1, #0
    ldr r0, .L_sound_4023
    mov r2, r1
    mov r3, r1
    bl BattleSound_Play
    b .L_after_sounds
.L_form_5:
    mov r2, #0
    mov r3, r2
    mov r0, #0x3E
    mov r1, #0x1B
    bl BattleSound_Play
    mov r1, #0
    ldr r0, .L_sound_404b
    mov r2, r1
    mov r3, r1
    bl BattleSound_Play
.L_after_sounds:
    mov r0, #3
    mov r1, #0x0A
    mov r2, #0x0C
    bl BattleScreenEffect_StartPrimary
    mov r0, r5
    pop {r4, r5, r6, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_task_pool_offset:
    .word 0x8B44
.L_update_callback:
    .word BattleParty_UpdateKnockout
.L_sound_4023:
    .word 0x4023
.L_sound_404b:
    .word 0x404B
.L_sound_4060:
    .word 0x4060
.L_sound_4073:
    .word 0x4073
.size BattleParty_StartKnockout, . - BattleParty_StartKnockout
