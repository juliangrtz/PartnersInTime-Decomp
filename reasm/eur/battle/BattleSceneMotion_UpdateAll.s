/* Run all active scene-object motion channels and retire idle objects. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneMotion_UpdateAll, "ax", %progbits
.balign 4

.extern BattleSceneObject_StopMotionChannel
.extern BattleSceneObject_UpdateTravelDistance
.extern gBattleMotionObjectList

.global BattleSceneMotion_UpdateAll
.type BattleSceneMotion_UpdateAll, %function
BattleSceneMotion_UpdateAll:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    ldr r0, .L_motion_list
    mov r5, #0
    ldr r10, [r0]
    cmp r10, #0
    addeq sp, sp, #4
    popeq {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bxeq lr
    mov r11, r5
    mov r4, #1
.L_object:
    ldrsh r0, [r10, #4]
    mov r9, r11
    add r8, r10, #0x1c
    strh r0, [r10, #0xa]
    ldrsh r0, [r10, #6]
    strh r0, [r10, #0xc]
    ldrsh r0, [r10, #8]
    strh r0, [r10, #0xe]
.L_find_active_channel:
    cmp r9, #4
    bne .L_test_channel
    ldrsh r0, [r10, #4]
    cmp r5, #0
    strh r0, [r10, #0x10]
    ldrsh r0, [r10, #6]
    strh r0, [r10, #0x12]
    ldrsh r0, [r10, #8]
    strh r0, [r10, #0x14]
    ldrne r0, [r10]
    strne r0, [r5]
    ldreq r1, [r10]
    ldreq r0, .L_motion_list
    streq r1, [r0]
    b .L_finish_object
.L_test_channel:
    ldr r0, [r8]
    cmp r0, #0
    beq .L_next_empty_channel
    ldrsh r0, [r10, #0x10]
    cmp r9, #4
    strh r0, [r10, #4]
    ldrsh r0, [r10, #0x12]
    strh r0, [r10, #6]
    ldrsh r0, [r10, #0x14]
    strh r0, [r10, #8]
    bge .L_after_channels
    add r7, r10, #4
    add r6, r10, #6
    add r5, r10, #8
.L_channel:
    ldr r0, [r8]
    cmp r0, #0
    beq .L_apply_deferred_delta
    ldrsh r0, [r10, #4]
    strh r0, [r8, #0x12]
    ldrsh r0, [r10, #6]
    strh r0, [r8, #0x14]
    ldrsh r0, [r10, #8]
    strh r0, [r8, #0x16]
.L_apply_deferred_delta:
    ldrh r0, [r8, #0xa]
    cmp r0, #0
    beq .L_run_callback
    ldrsh r1, [r7]
    ldrsh r0, [r8, #0xc]
    add r0, r1, r0
    strh r0, [r7]
    ldrsh r1, [r6]
    ldrsh r0, [r8, #0xe]
    add r0, r1, r0
    strh r0, [r6]
    ldrsh r1, [r5]
    ldrsh r0, [r8, #0x10]
    add r0, r1, r0
    strh r0, [r5]
.L_run_callback:
    ldr r0, [r8]
    cmp r0, #0
    beq .L_channel_complete
    ldrh r0, [r10, #0x1a]
    cmp r0, #0
    ldrsheq r0, [r10, #0x16]
    ldreq r1, [r8, #4]
    addeq r0, r0, #0x100
    addeq r0, r1, r0
    streq r0, [r8, #4]
    ldrsh r1, [r8, #8]
    cmp r1, #0
    beq .L_call_callback
    ldr r0, [r8, #4]
    lsl r1, r1, #8
    cmp r1, r0
    strlt r1, [r8, #4]
.L_call_callback:
    ldr r2, [r8]
    mov r0, r10
    mov r1, r8
    blx r2
    ldrsh r1, [r10, #4]
    ldrsh r0, [r8, #0x12]
    sub r0, r1, r0
    strh r0, [r8, #0x12]
    ldrsh r1, [r10, #6]
    ldrsh r0, [r8, #0x14]
    sub r0, r1, r0
    strh r0, [r8, #0x14]
    ldrsh r1, [r10, #8]
    ldrsh r0, [r8, #0x16]
    sub r0, r1, r0
    strh r0, [r8, #0x16]
    ldrsh r1, [r8, #8]
    cmp r1, #0
    beq .L_channel_complete
    ldr r0, [r8, #4]
    cmp r0, r1, lsl #8
    bne .L_channel_complete
    mov r0, r10
    mov r1, r9
    mov r2, r4
    bl BattleSceneObject_StopMotionChannel
.L_channel_complete:
    add r9, r9, #1
    cmp r9, #4
    add r8, r8, #0x28
    blt .L_channel
.L_after_channels:
    mov r5, r10
    b .L_finish_object
.L_next_empty_channel:
    add r8, r8, #0x28
    add r9, r9, #1
    b .L_find_active_channel
.L_finish_object:
    mov r0, r10
    bl BattleSceneObject_UpdateTravelDistance
    ldr r10, [r10]
    cmp r10, #0
    bne .L_object
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.L_motion_list:
    .word gBattleMotionObjectList
.size BattleSceneMotion_UpdateAll, . - BattleSceneMotion_UpdateAll
