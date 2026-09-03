/* Normalize a three-axis path, solve its duration, and start accelerated motion. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_StartAcceleratedMotion, "ax", %progbits
.balign 4

.extern BattleSceneObject_BeginMotionChannel
.extern BattleSceneObject_UpdateAcceleratedMotion
.extern _s32_div_f

.global BattleSceneObject_StartAcceleratedMotion
.type BattleSceneObject_StartAcceleratedMotion, %function
BattleSceneObject_StartAcceleratedMotion:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x14
    mul r4, r3, r3
    str r3, [sp, #8]
    mla r3, r2, r2, r4
    str r2, [sp, #4]
    ldr r2, [sp, #0x38]
    ldr r4, .L_sqrt_control
    mov r5, #0
    mla r3, r2, r2, r3
    str r2, [sp, #0x38]
    ldr r2, .L_sqrt_parameter
    strh r5, [r4]
    ldr r9, [sp, #0x3c]
    ldr r8, [sp, #0x40]
    ldr r7, [sp, #0x44]
    mov r10, r0
    str r1, [sp]
    str r3, [r2]
.L_wait_path_sqrt:
    ldrh r0, [r4]
    ands r0, r0, #0x8000
    bne .L_wait_path_sqrt
    ldr r1, [sp, #4]
    mul r0, r1, r9
    ldr r1, .L_sqrt_result
    ldr r1, [r1]
    str r1, [sp, #0x10]
    bl _s32_div_f
    ldr r1, [sp, #8]
    mov r5, r0
    mul r0, r1, r9
    ldr r1, [sp, #0x10]
    bl _s32_div_f
    ldr r1, [sp, #0x38]
    mov r4, r0
    mul r0, r1, r9
    ldr r1, [sp, #0x10]
    bl _s32_div_f
    mov r11, r0
    cmp r7, #0
    bne .L_solve_accelerated_duration
    muls r0, r9, r8
    bpl .L_constant_duration
    ldrsh r1, [r10, #4]
    add sp, sp, #0x14
    mov r0, #0
    add r1, r1, r5
    strh r1, [r10, #4]
    ldrsh r1, [r10, #6]
    add r1, r1, r4
    strh r1, [r10, #6]
    ldrsh r1, [r10, #8]
    add r1, r1, r11
    strh r1, [r10, #8]
    ldrsh r1, [r10, #0x10]
    add r1, r1, r5
    strh r1, [r10, #0x10]
    ldrsh r1, [r10, #0x12]
    add r1, r1, r4
    strh r1, [r10, #0x12]
    ldrsh r1, [r10, #0x14]
    add r1, r1, r11
    strh r1, [r10, #0x14]
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.L_constant_duration:
    mov r1, r8
    lsl r0, r9, #0x10
    bl _s32_div_f
    mov r6, r0
    b .L_start_motion
.L_solve_accelerated_duration:
    asr r0, r8, #7
    add r0, r8, r0, lsr #24
    asr r1, r8, #3
    asr r2, r0, #8
    add r0, r8, r1, lsr #28
    asr r0, r0, #4
    mul r1, r2, r0
    mul r0, r7, r9
    add r2, r1, r0, lsl #5
    cmp r2, #0
    movle r6, #0
    ble .L_roots_ready
    ldr r1, .L_sqrt_control
    mov r0, #0
    strh r0, [r1]
    ldr r0, .L_sqrt_parameter
    str r2, [r0]
.L_wait_duration_sqrt:
    ldrh r0, [r1]
    ands r0, r0, #0x8000
    bne .L_wait_duration_sqrt
    ldr r0, .L_sqrt_result
    ldr r0, [r0]
    lsl r6, r0, #6
.L_roots_ready:
    mov r1, r7
    sub r0, r6, r8
    bl _s32_div_f
    add r1, r8, r6
    str r0, [sp, #0xc]
    rsb r0, r1, #0
    mov r1, r7
    bl _s32_div_f
    mov r6, r0
    ldr r0, [sp, #0x48]
    cmp r0, #0
    beq .L_choose_earlier_root
    ldr r0, [sp, #0xc]
    cmp r0, r6
    movge r6, r0
    b .L_validate_duration
.L_choose_earlier_root:
    ldr r0, [sp, #0xc]
    cmp r0, r6
    strge r6, [sp, #0xc]
    ldr r6, [sp, #0xc]
.L_validate_duration:
    cmp r6, #0
    bgt .L_start_motion
    mov r2, #0
    ldr r1, [sp]
    mov r0, r10
    mov r3, r2
    bl BattleSceneObject_BeginMotionChannel
    ldrsh r1, [r10, #4]
    add sp, sp, #0x14
    mov r0, #0
    add r1, r1, r5
    strh r1, [r10, #4]
    ldrsh r1, [r10, #6]
    add r1, r1, r4
    strh r1, [r10, #6]
    ldrsh r1, [r10, #8]
    add r1, r1, r11
    strh r1, [r10, #8]
    ldrsh r1, [r10, #0x10]
    add r1, r1, r5
    strh r1, [r10, #0x10]
    ldrsh r1, [r10, #0x12]
    add r1, r1, r4
    strh r1, [r10, #0x12]
    ldrsh r1, [r10, #0x14]
    add r1, r1, r11
    strh r1, [r10, #0x14]
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.L_start_motion:
    ldr r3, .L_update_callback
    ldr r1, [sp]
    mov r0, r10
    mov r2, r6
    bl BattleSceneObject_BeginMotionChannel
    mov r4, r0
    str r7, [r4]
    lsl r0, r8, #1
    str r0, [r4, #4]
    ldr r0, [sp, #4]
    ldr r1, [sp, #0x10]
    strh r9, [r4, #8]
    lsl r0, r0, #0xe
    bl _s32_div_f
    strh r0, [r4, #0xa]
    ldr r0, [sp, #8]
    ldr r1, [sp, #0x10]
    lsl r0, r0, #0xe
    bl _s32_div_f
    strh r0, [r4, #0xc]
    ldr r0, [sp, #0x38]
    ldr r1, [sp, #0x10]
    lsl r0, r0, #0xe
    bl _s32_div_f
    strh r0, [r4, #0xe]
    mov r0, r6
    add sp, sp, #0x14
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.L_sqrt_control:
    .word 0x040002B0
.L_sqrt_parameter:
    .word 0x040002B8
.L_sqrt_result:
    .word 0x040002B4
.L_update_callback:
    .word BattleSceneObject_UpdateAcceleratedMotion
.size BattleSceneObject_StartAcceleratedMotion, . - BattleSceneObject_StartAcceleratedMotion
