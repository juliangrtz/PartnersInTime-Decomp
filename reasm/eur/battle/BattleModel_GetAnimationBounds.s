/* Decode one frame's model metadata into a signed two-dimensional bounds box. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleModel_GetAnimationBounds, "ax", %progbits
.balign 4

.extern data_02049918
.extern data_02049924
.extern data_ov002_020c0660
.extern func_0200940c
.extern func_0202cc58
.extern func_0202cd2c

.global BattleModel_GetAnimationBounds
.type BattleModel_GetAnimationBounds, %function
BattleModel_GetAnimationBounds:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r6, r1
    ldr r1, [r6, #0x7c]
    mov r7, r0
    lsl r0, r1, #6
    lsrs r0, r0, #0x1f
    mov r4, r2
    mov r5, r3
    addeq sp, sp, #4
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, r9, lr}
    bxeq lr
    cmp r5, #0
    beq .L_select_frame
    mov r0, r6
    ldr r1, [r0]
    ldr r1, [r1, #0xc]
    blx r1
    ldr r2, .L_runtime_context
    ldr r1, .L_model_restore_offset
    ldr r3, [r2]
    mov r2, r0
    add r1, r3, r1
    cmp r6, r1
    bhs .L_prepare_high_model
    mov r0, r6
    bl func_0202cd2c
    b .L_select_frame
.L_prepare_high_model:
    mov r0, r6
    bl func_0202cc58
.L_select_frame:
    ldrsh r1, [r6, #0x5a]
    cmp r5, #0
    movge r2, #1
    mvnlt r2, #0
    mul r2, r1, r2
    lsl r1, r2, #0x10
    mov r0, r6
    asr r1, r1, #0x10
    bl func_0200940c
    cmp r5, #0
    rsble r5, r5, #0
    cmp r5, #0
    mov r8, #0
    ble .L_decode_frame
.L_advance_frame:
    mov r0, r6
    ldr r1, [r0]
    ldr r1, [r1]
    blx r1
    add r8, r8, #1
    cmp r8, r5
    blt .L_advance_frame
.L_decode_frame:
    ldrsh r1, [r6, #0x54]
    ldr r2, [r6, #0x48]
    ldrsh r0, [r6, #0x56]
    lsl r1, r1, #3
    ldrh r3, [r2, r1]
    ldr r2, [r6, #0x44]
    ldr r1, [r6, #0x40]
    add r0, r0, r3
    lsl r0, r0, #2
    ldrh ip, [r2, r0]
    ldr r0, [r6, #0x3c]
    mov r8, #0xc
    lsl r9, ip, #2
    ldrh lr, [r1, r9]
    add r9, r1, ip, lsl #2
    ldrh ip, [r9, #2]
    mla r9, lr, r8, r0
    ldrh r9, [r9, #4]
    cmp r9, #0
    beq .L_have_sequence
    lsl r3, r3, #2
    ldrh r3, [r2, r3]
    lsl r2, r3, #2
    ldrh lr, [r1, r2]
    add r2, r1, r3, lsl #2
    ldrh ip, [r2, #2]
    mla r2, lr, r8, r0
    ldrh r2, [r2, #4]
    cmp r2, #0
    ldrhne ip, [r1, #2]
    movne lr, #0
.L_have_sequence:
    mov r1, #0xc
    mla r0, lr, r1, r0
    mla r0, r4, r1, r0
    add r1, lr, r4
    cmp r1, ip
    mov r4, #0
    bge .L_default_bounds
    ldrh r1, [r0, #4]
    cmp r1, #0
    bne .L_default_bounds
    ldrh r1, [r0]
    ldrh r3, [r0, #2]
    ldr r4, .L_extent_y_table
    lsl r1, r1, #0x10
    lsr r8, r1, #0x1e
    lsl r1, r3, #0x17
    lsr r2, r1, #0x17
    lsl r1, r3, #0x10
    lsl r8, r8, #2
    orr r8, r8, r1, lsr #30
    ands r1, r2, #0x100
    ldr r3, .L_extent_x_table
    movne lr, #0x200
    rsbne lr, lr, #0
    ldrb ip, [r4, r8]
    ldrb r1, [r0]
    ldrb r3, [r3, r8]
    orrne r2, r2, lr
    ands r8, r1, #0x80
    ldrh r8, [r0]
    mvnne lr, #0xff
    mov r4, #1
    lsl r8, r8, #0x16
    orrne r1, r1, lr
    lsrs r9, r8, #0x1e
    bne .L_center_bounds
    strh r2, [r7]
    strh r1, [r7, #2]
    add r0, r2, ip
    strh r0, [r7, #4]
    add r0, r1, r3
    strh r0, [r7, #6]
    b .L_restore_frame
.L_center_bounds:
    ands r8, r9, #2
    addne r2, r2, ip
    addne r1, r1, r3
    addeq r2, r2, ip, asr #1
    addeq r1, r1, r3, asr #1
    ands r8, r9, #1
    beq .L_store_centered
    ldrh r8, [r0, #8]
    ldr lr, [r6, #0x4c]
    mov r0, #0xc
    lsl r8, r8, #0x16
    lsr r8, r8, #0x16
    mla r0, r8, r0, lr
    ldrsh r8, [r0, #2]
    ldrsh r0, [r0, #4]
    mul r8, ip, r8
    mul r0, r3, r0
    asr ip, r8, #9
    asr r3, r0, #9
.L_store_centered:
    sub r0, r2, ip
    strh r0, [r7]
    sub r0, r1, r3
    strh r0, [r7, #2]
    add r0, r2, ip
    strh r0, [r7, #4]
    add r0, r1, r3
    strh r0, [r7, #6]
    b .L_restore_frame
.L_default_bounds:
    mvn r0, #0xf
    strh r0, [r7]
    mvn r0, #0x1f
    strh r0, [r7, #2]
    mov r0, #0x10
    strh r0, [r7, #4]
    mov r0, #0
    strh r0, [r7, #6]
.L_restore_frame:
    cmp r5, #0
    beq .L_return
    mov r0, r6
    ldr r1, [r0]
    ldr r1, [r1, #0xc]
    blx r1
    ldr r2, .L_runtime_context
    ldr r1, .L_model_restore_offset
    ldr r3, [r2]
    mov r2, r0
    add r0, r3, r1
    cmp r0, r6
    bhs .L_restore_low_model
    mov r1, r6
    bl func_0202cd2c
    b .L_return
.L_restore_low_model:
    mov r1, r6
    bl func_0202cc58
.L_return:
    mov r0, r4
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.L_runtime_context:
    .word data_ov002_020c0660
.L_model_restore_offset:
    .word 0x11384
.L_extent_y_table:
    .word data_02049924
.L_extent_x_table:
    .word data_02049918
.size BattleModel_GetAnimationBounds, . - BattleModel_GetAnimationBounds
