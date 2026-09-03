/* Poll the ARM7 X/Y/hinge inputs into the shared key-state buffer. */

.syntax unified
.arch armv4t
.arm
.section .pit_PAD_XYButton, "ax", %progbits
.balign 4

.extern HW_BUTTON_XY_BUF
.extern OS_CreateAlarm
.extern OS_GetTick
.extern OS_IsAlarmAvailable
.extern OS_IsTickAvailable
.extern OS_SetPeriodicAlarm
.extern PADi_XYButtonAlarm
.extern PADi_XYButtonAvailable
.extern REG_EXTKEYIN
.extern REG_RCNT

.global PADi_SetRcntMode
.type PADi_SetRcntMode, %function
PADi_SetRcntMode:
    mov r1, r0
    mov r0, #0xC000
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    ldr r12, .L_write_rcnt_target
    bx r12
.size PADi_SetRcntMode, . - PADi_SetRcntMode
.L_write_rcnt_target:
    .word PADi_WriteRcnt

.global PADi_WriteRcnt
.type PADi_WriteRcnt, %function
PADi_WriteRcnt:
    mvn r3, r0
    ldr r2, .L_rcnt_register
    ldrh r0, [r2]
    and r0, r3, r0
    orr r0, r1, r0
    strh r0, [r2]
    bx lr
.size PADi_WriteRcnt, . - PADi_WriteRcnt
.L_rcnt_register:
    .word REG_RCNT

.global PADi_XYButtonAlarmHandler
.type PADi_XYButtonAlarmHandler, %function
PADi_XYButtonAlarmHandler:
    push {r4, lr}
    mov r4, #0
    mov r0, #0x8000
    bl PADi_SetRcntMode
    ldr r0, .L_extkeyin
    ldrh r1, [r0]
    ands r0, r1, #0x80
    movne r4, #0x8000
    and r0, r1, #0x0B
    orr r1, r4, r0, lsl #10
    ldr r0, .L_button_xy_buffer
    strh r1, [r0]
    pop {r4, lr}
    bx lr
.size PADi_XYButtonAlarmHandler, . - PADi_XYButtonAlarmHandler
.L_extkeyin:
    .word REG_EXTKEYIN
.L_button_xy_buffer:
    .word HW_BUTTON_XY_BUF

.global PAD_InitXYButton
.type PAD_InitXYButton, %function
PAD_InitXYButton:
    stmdb sp!, {lr}
    sub sp, sp, #0x0C
    bl OS_IsTickAvailable
    cmp r0, #0
    beq .L_xy_unavailable
    bl OS_IsAlarmAvailable
    cmp r0, #0
    bne .L_xy_initialize
.L_xy_unavailable:
    mov r0, #0
    b .L_xy_return
.L_xy_initialize:
    ldr r0, .L_xy_available
    ldr r0, [r0]
    cmp r0, #0
    movne r0, #0
    bne .L_xy_return
    ldr r0, .L_xy_alarm
    bl OS_CreateAlarm
    bl OS_GetTick
    mov r2, r0
    ldr r0, .L_xy_handler
    str r0, [sp, #4]
    mov r0, #0
    str r0, [sp, #8]
    ldr r3, .L_xy_period
    str r0, [sp]
    ldr r0, .L_xy_alarm
    adds r12, r2, r3
    adc r2, r1, #0
    mov r1, r12
    bl OS_SetPeriodicAlarm
    mov r0, #1
    ldr r1, .L_xy_available
    str r0, [r1]
.L_xy_return:
    add sp, sp, #0x0C
    ldmia sp!, {lr}
    bx lr
.size PAD_InitXYButton, . - PAD_InitXYButton
.L_xy_available:
    .word PADi_XYButtonAvailable
.L_xy_alarm:
    .word PADi_XYButtonAlarm
.L_xy_handler:
    .word PADi_XYButtonAlarmHandler
.L_xy_period:
    .word 0x0000082E
