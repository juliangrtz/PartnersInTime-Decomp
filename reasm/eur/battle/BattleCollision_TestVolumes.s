/* Perform swept overlap testing on two pairs of six-halfword battle volumes. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleCollision_TestVolumes, "ax", %progbits
.balign 4

.extern _s32_div_f

.global BattleCollision_TestVolumes
.type BattleCollision_TestVolumes, %function
BattleCollision_TestVolumes:
    push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    sub sp, sp, #0x3C
    mov sl, r0
    ldrsh r0, [sl]
    mov sb, r1
    str r2, [sp]
    str r0, [sp, #0x14]
    mov r0, r2
    ldrsh r0, [r0, #2]
    mov fp, r3
    ldr r8, [sp, #0x60]
    str r0, [sp, #0x20]
    mov r1, r0
    ldr r0, [sp, #0x14]
    sub r0, r1, r0
    strh r0, [sp, #0x24]
    mov r0, r2
    ldrsh r0, [r0]
    ldrsh r2, [sp, #0x24]
    str r0, [sp, #0x1C]
    ldrsh r0, [sl, #2]
    cmp r2, #0
    str r0, [sp, #0x18]
    mov r1, r0
    ldr r0, [sp, #0x1C]
    sub r0, r1, r0
    strh r0, [sp, #0x26]
    ldr r0, [sp]
    ldrsh r1, [r0, #6]
    ldrsh r0, [sl, #4]
    sub r0, r1, r0
    strh r0, [sp, #0x28]
    ldr r0, [sp]
    ldrsh r1, [sl, #6]
    ldrsh r0, [r0, #4]
    sub r0, r1, r0
    strh r0, [sp, #0x2A]
    ldr r0, [sp]
    ldrsh r1, [r0, #0x0A]
    ldrsh r0, [sl, #8]
    sub r0, r1, r0
    strh r0, [sp, #0x2C]
    ldr r0, [sp]
    ldrsh r1, [sl, #0x0A]
    ldrsh r0, [r0, #8]
    sub r0, r1, r0
    strh r0, [sp, #0x2E]
    ldrsh r0, [sb]
    ldrsh r1, [fp, #2]
    str r0, [sp, #4]
    sub r0, r1, r0
    strh r0, [sp, #0x30]
    str r1, [sp, #0x10]
    ldrsh r0, [fp]
    ldrsh r1, [sb, #2]
    str r0, [sp, #0x0C]
    sub r0, r1, r0
    strh r0, [sp, #0x32]
    str r1, [sp, #8]
    ldrsh r1, [fp, #6]
    ldrsh r0, [sb, #4]
    sub r0, r1, r0
    strh r0, [sp, #0x34]
    ldrsh r1, [sb, #6]
    ldrsh r0, [fp, #4]
    sub r0, r1, r0
    strh r0, [sp, #0x36]
    ldrsh r1, [fp, #0x0A]
    ldrsh r0, [sb, #8]
    sub r0, r1, r0
    strh r0, [sp, #0x38]
    ldrsh r1, [sb, #0x0A]
    ldrsh r0, [fp, #8]
    sub r0, r1, r0
    strh r0, [sp, #0x3A]

    bge .L_axis1_separation
    ldrsh r0, [sp, #0x30]
    cmp r0, #0
    addlt sp, sp, #0x3C
    movlt r0, #0
    poplt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxlt lr
.L_axis1_separation:
    ldrsh r4, [sp, #0x26]
    cmp r4, #0
    bge .L_axis2_separation
    ldrsh r0, [sp, #0x32]
    cmp r0, #0
    addlt sp, sp, #0x3C
    movlt r0, #0
    poplt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxlt lr
.L_axis2_separation:
    ldrsh r3, [sp, #0x28]
    cmp r3, #0
    bge .L_axis3_separation
    ldrsh r0, [sp, #0x34]
    cmp r0, #0
    addlt sp, sp, #0x3C
    movlt r0, #0
    poplt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxlt lr
.L_axis3_separation:
    ldrsh r2, [sp, #0x2A]
    cmp r2, #0
    bge .L_axis4_separation
    ldrsh r0, [sp, #0x36]
    cmp r0, #0
    addlt sp, sp, #0x3C
    movlt r0, #0
    poplt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxlt lr
.L_axis4_separation:
    ldrsh r1, [sp, #0x2C]
    cmp r1, #0
    bge .L_axis5_separation
    ldrsh r0, [sp, #0x38]
    cmp r0, #0
    addlt sp, sp, #0x3C
    movlt r0, #0
    poplt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxlt lr
.L_axis5_separation:
    ldrsh r0, [sp, #0x2E]
    cmp r0, #0
    bge .L_check_motion
    ldrsh r5, [sp, #0x3A]
    cmp r5, #0
    addlt sp, sp, #0x3C
    movlt r0, #0
    poplt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxlt lr

.L_check_motion:
    ldrsh r6, [sp, #0x24]
    ldrsh r5, [sp, #0x30]
    cmp r6, r5
    bne .L_check_all_positive
    ldrsh r5, [sp, #0x32]
    cmp r4, r5
    bne .L_check_all_positive
    ldrsh r5, [sp, #0x34]
    cmp r3, r5
    bne .L_check_all_positive
    ldrsh r5, [sp, #0x36]
    cmp r2, r5
    bne .L_check_all_positive
    ldrsh r5, [sp, #0x38]
    cmp r1, r5
    bne .L_check_all_positive
    ldrsh r5, [sp, #0x3A]
    cmp r0, r5
    addeq sp, sp, #0x3C
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr

.L_check_all_positive:
    ldrsh r5, [sp, #0x24]
    cmp r5, #0
    ble .L_find_entry_time
    cmp r4, #0
    ble .L_find_entry_time
    cmp r3, #0
    ble .L_find_entry_time
    cmp r2, #0
    ble .L_find_entry_time
    cmp r1, #0
    ble .L_find_entry_time
    cmp r0, #0
    addgt sp, sp, #0x3C
    movgt r0, #0
    popgt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxgt lr

.L_find_entry_time:
    mov r7, #0x80000001
    mvn r6, #0
    mov r4, #0
    add r5, sp, #0x24
.L_entry_loop:
    mov r1, r4, lsl #1
    ldrsh r2, [r5, r1]
    cmp r2, #0
    bgt .L_entry_next
    add r0, sp, #0x30
    ldrsh r1, [r0, r1]
    mov r0, r2, lsl #12
    sub r1, r2, r1
    bl _s32_div_f
    cmp r7, r0
    movlt r7, r0
    movlt r6, r4
.L_entry_next:
    add r4, r4, #1
    cmp r4, #6
    blt .L_entry_loop
    cmp r6, #0
    addlt sp, sp, #0x3C
    movlt r0, #0
    poplt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxlt lr

    mvn r5, #0x80000000
    mov r4, #0
.L_exit_loop:
    mov r2, r4, lsl #1
    add r0, sp, #0x30
    ldrsh r1, [r0, r2]
    cmp r1, #0
    bgt .L_exit_next
    add r0, sp, #0x24
    ldrsh r0, [r0, r2]
    sub r1, r0, r1
    mov r0, r0, lsl #12
    bl _s32_div_f
    cmp r0, r5
    movlt r5, r0
.L_exit_next:
    add r4, r4, #1
    cmp r4, #6
    blt .L_exit_loop
    cmp r5, r7
    addlt sp, sp, #0x3C
    movlt r0, #0
    poplt {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxlt lr

    cmp r8, #0
    beq .L_return_mask
    cmp r6, #0
    bne .L_x_axis1
    ldr r1, [sp, #4]
    ldr r0, [sp, #0x14]
    sub r0, r1, r0
    mul r1, r7, r0
    mov r0, r1, asr #13
    add r1, r1, r0, lsr #18
    ldr r0, [sp, #0x14]
    add r0, r0, r1, asr #14
    strh r0, [r8]
    b .L_calculate_y
.L_x_axis1:
    cmp r6, #1
    bne .L_x_midpoint
    ldr r1, [sp, #8]
    ldr r0, [sp, #0x18]
    sub r0, r1, r0
    mul r1, r7, r0
    mov r0, r1, asr #13
    add r1, r1, r0, lsr #18
    ldr r0, [sp, #0x18]
    add r0, r0, r1, asr #14
    strh r0, [r8]
    b .L_calculate_y
.L_x_midpoint:
    ldr r1, [sp, #4]
    ldr r0, [sp, #0x14]
    ldr r3, [sp, #0x0C]
    sub r0, r1, r0
    mul r2, r7, r0
    ldr r1, [sp, #8]
    ldr r0, [sp, #0x18]
    sub r0, r1, r0
    mul r1, r7, r0
    ldr r0, [sp, #0x1C]
    sub r0, r3, r0
    mov r3, r2, asr #13
    add r3, r2, r3, lsr #18
    ldr r2, [sp, #0x14]
    mul r0, r7, r0
    add r2, r2, r3, asr #14
    mov r3, r1, asr #13
    add r3, r1, r3, lsr #18
    ldr r1, [sp, #0x18]
    add r1, r1, r3, asr #14
    mov r3, r0, asr #13
    add r3, r0, r3, lsr #18
    ldr r0, [sp, #0x1C]
    add r0, r0, r3, asr #14
    cmp r2, r0
    movlt r2, r0
    ldr r3, [sp, #0x10]
    ldr r0, [sp, #0x20]
    sub r0, r3, r0
    mul r3, r7, r0
    mov r0, r3, asr #13
    add r3, r3, r0, lsr #18
    ldr r0, [sp, #0x20]
    add r0, r0, r3, asr #14
    cmp r0, r1
    movlt r1, r0
    add r0, r2, r1
    add r0, r0, r0, lsr #31
    mov r0, r0, asr #1
    strh r0, [r8]

.L_calculate_y:
    cmp r6, #2
    bne .L_y_axis3
    ldrsh r2, [sl, #4]
    ldrsh r0, [sb, #4]
    sub r0, r0, r2
    mul r1, r7, r0
    mov r0, r1, asr #13
    add r0, r1, r0, lsr #18
    add r0, r2, r0, asr #14
    strh r0, [r8, #2]
    b .L_calculate_z
.L_y_axis3:
    cmp r6, #3
    bne .L_y_midpoint
    ldrsh r2, [sl, #6]
    ldrsh r0, [sb, #6]
    sub r0, r0, r2
    mul r1, r7, r0
    mov r0, r1, asr #13
    add r0, r1, r0, lsr #18
    add r0, r2, r0, asr #14
    strh r0, [r8, #2]
    b .L_calculate_z
.L_y_midpoint:
    ldr r0, [sp]
    ldrsh r3, [sl, #4]
    ldrsh r1, [sb, #4]
    ldrsh r0, [r0, #4]
    ldrsh r2, [fp, #4]
    sub r1, r1, r3
    mul r1, r7, r1
    sub r2, r2, r0
    mul ip, r7, r2
    mov r2, r1, asr #13
    add r4, r1, r2, lsr #18
    mov r5, ip, asr #13
    ldrsh r2, [sl, #6]
    ldrsh r1, [sb, #6]
    add r5, ip, r5, lsr #18
    add r3, r3, r4, asr #14
    add r4, r0, r5, asr #14
    sub r0, r1, r2
    mul r1, r7, r0
    mov r0, r1, asr #13
    add r0, r1, r0, lsr #18
    add r1, r2, r0, asr #14
    ldr r0, [sp]
    cmp r3, r4
    ldrsh r0, [r0, #6]
    ldrsh r2, [fp, #6]
    movlt r3, r4
    sub r2, r2, r0
    mul r4, r7, r2
    mov r2, r4, asr #13
    add r2, r4, r2, lsr #18
    add r0, r0, r2, asr #14
    cmp r0, r1
    movlt r1, r0
    add r0, r3, r1
    add r0, r0, r0, lsr #31
    mov r0, r0, asr #1
    strh r0, [r8, #2]

.L_calculate_z:
    cmp r6, #4
    bne .L_z_axis5
    ldrsh r2, [sl, #8]
    ldrsh r0, [sb, #8]
    sub r0, r0, r2
    mul r1, r7, r0
    mov r0, r1, asr #13
    add r0, r1, r0, lsr #18
    add r0, r2, r0, asr #14
    strh r0, [r8, #4]
    b .L_transform_position
.L_z_axis5:
    cmp r6, #5
    bne .L_z_midpoint
    ldrsh r2, [sl, #0x0A]
    ldrsh r0, [sb, #0x0A]
    sub r0, r0, r2
    mul r1, r7, r0
    mov r0, r1, asr #13
    add r0, r1, r0, lsr #18
    add r0, r2, r0, asr #14
    strh r0, [r8, #4]
    b .L_transform_position
.L_z_midpoint:
    ldr r0, [sp]
    ldrsh r2, [sl, #8]
    ldrsh r1, [sb, #8]
    ldrsh r0, [r0, #8]
    ldrsh r3, [fp, #8]
    sub r1, r1, r2
    mul r1, r7, r1
    sub r3, r3, r0
    mul r5, r7, r3
    mov r3, r1, asr #13
    add r3, r1, r3, lsr #18
    mov r4, r5, asr #13
    add r4, r5, r4, lsr #18
    add r2, r2, r3, asr #14
    add r3, r0, r4, asr #14
    ldrsh r1, [sl, #0x0A]
    ldrsh r5, [sb, #0x0A]
    cmp r2, r3
    movlt r2, r3
    sub r0, r5, r1
    mul r4, r7, r0
    mov r0, r4, asr #13
    add r0, r4, r0, lsr #18
    add r0, r1, r0, asr #14
    ldr r1, [sp]
    ldrsh r4, [r1, #0x0A]
    ldrsh r1, [fp, #0x0A]
    sub r1, r1, r4
    mul r3, r7, r1
    mov r1, r3, asr #13
    add r1, r3, r1, lsr #18
    add r1, r4, r1, asr #14
    cmp r1, r0
    movlt r0, r1
    add r0, r2, r0
    add r0, r0, r0, lsr #31
    mov r0, r0, asr #1
    strh r0, [r8, #4]

.L_transform_position:
    ldrsh r0, [r8, #4]
    ldrsh r1, [r8, #2]
    rsb r0, r0, #0
    strh r0, [r8, #2]
    rsb r0, r1, #0
    strh r0, [r8, #4]

.L_return_mask:
    mov r0, #1
    mov r0, r0, lsl r6
    add sp, sp, #0x3C
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr
.size BattleCollision_TestVolumes, . - BattleCollision_TestVolumes
