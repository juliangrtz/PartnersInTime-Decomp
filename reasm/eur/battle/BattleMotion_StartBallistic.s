/* Derive a signed launch velocity with the DS square-root unit and start motion. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleMotion_StartBallistic, "ax", %progbits
.balign 4

.extern BattleSceneObject_StartAcceleratedMotion

.global BattleMotion_StartBallistic
.type BattleMotion_StartBallistic, %function
BattleMotion_StartBallistic:
    push {r4, r5, r6, lr}
    sub sp, sp, #0x18
    ldr ip, [sp, #0x30]
    ldr r5, [sp, #0x34]
    mul r4, ip, r5
    cmp r4, #0
    ldrgt r4, [sp, #0x2C]
    movgt ip, #0
    rsbgt r5, r4, #0
    bgt .L_velocity_ready
    mov lr, ip, lsl #1
    rsb r4, lr, #0
    mul lr, r5, r4
    ldr r5, .L_sqrt_control
    mov r6, #0
    ldr r4, .L_sqrt_parameter
    strh r6, [r5]
    mov lr, lr, lsl #12
    str lr, [r4]
.L_wait_sqrt:
    ldrh r4, [r5]
    ands r4, r4, #0x8000
    bne .L_wait_sqrt
    ldr lr, .L_sqrt_result
    ldr r4, [sp, #0x34]
    ldr lr, [lr]
    cmp r4, #0
    mov r5, lr, lsl #6
    rsblt r5, r5, #0
.L_velocity_ready:
    ldr r4, [sp, #0x28]
    ldr lr, [sp, #0x2C]
    str r4, [sp]
    str lr, [sp, #4]
    str r5, [sp, #8]
    mov lr, ip, lsl #8
    ldr ip, [sp, #0x38]
    str lr, [sp, #0x0C]
    str ip, [sp, #0x10]
    bl BattleSceneObject_StartAcceleratedMotion
    add sp, sp, #0x18
    pop {r4, r5, r6, lr}
    bx lr
.L_sqrt_control:
    .word 0x040002B0
.L_sqrt_parameter:
    .word 0x040002B8
.L_sqrt_result:
    .word 0x040002B4
.size BattleMotion_StartBallistic, . - BattleMotion_StartBallistic
