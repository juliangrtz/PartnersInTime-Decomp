/* Program hardware timer 1 for the head of the ARM7 alarm queue. */

.syntax unified
.arch armv4t
.arm
.section .pit_OSi_SetTimer, "ax", %progbits
.balign 4

.extern OS_EnableIrqMask
.extern OS_GetTick
.extern OSi_AlarmHandler
.extern OSi_SetTimerCallback
.extern REG_TM1CNT_H
.extern REG_TM1CNT_L

.global OSi_SetTimer
.type OSi_SetTimer, %function
OSi_SetTimer:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    bl OS_GetTick
    mov r2, #0
    ldr r3, .L_timer1_control_clear
    strh r2, [r3]
    ldr r12, [r4, #0xC]
    ldr r3, [r4, #0x10]
    subs r5, r12, r0
    sbc r4, r3, r1
    mov r0, #1
    ldr r1, .L_alarm_handler
    bl OSi_SetTimerCallback
    mov r1, #0
    subs r0, r5, r1
    sbcs r0, r4, r1
    ldrlt r1, .L_immediate_reload
    blt .L_store_reload
    mov r0, #0x10000
    subs r0, r5, r0
    sbcs r0, r4, r1
    mvnlt r0, r5
    movlt r0, r0, lsl #16
    movlt r1, r0, lsr #16
.L_store_reload:
    ldr r0, .L_timer1_count
    strh r1, [r0]
    mov r1, #0xC1
    ldr r0, .L_timer1_control_start
    strh r1, [r0]
    mov r0, #0x10
    bl OS_EnableIrqMask
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_timer1_control_clear:
    .word REG_TM1CNT_H
.L_alarm_handler:
    .word OSi_AlarmHandler
.L_immediate_reload:
    .word 0x0000FFFE
.L_timer1_count:
    .word REG_TM1CNT_L
.L_timer1_control_start = .L_timer1_control_clear
.size OSi_SetTimer, . - OSi_SetTimer
