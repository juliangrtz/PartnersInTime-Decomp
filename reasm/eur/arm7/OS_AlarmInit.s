/* Construct alarms and initialize the ARM7 timer-1 alarm queue. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_AlarmInit, "ax", %progbits
.balign 4

.extern OS_DisableIrqMask
.extern OSi_AlarmInitialized
.extern OSi_AlarmQueue
.extern OSi_SetTimerReserved

.global OS_CreateAlarm
.type OS_CreateAlarm, %function
OS_CreateAlarm:
    mov r1, #0
    str r1, [r0]
    str r1, [r0, #8]
    bx lr
.size OS_CreateAlarm, . - OS_CreateAlarm

.global OS_IsAlarmAvailable
.type OS_IsAlarmAvailable, %function
OS_IsAlarmAvailable:
    ldr r0, .L_alarm_initialized_get
    ldrh r0, [r0]
    bx lr
.L_alarm_initialized_get:
    .word OSi_AlarmInitialized
.size OS_IsAlarmAvailable, . - OS_IsAlarmAvailable

.global OS_InitAlarm
.type OS_InitAlarm, %function
OS_InitAlarm:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_alarm_initialized_init
    ldrh r0, [r1]
    cmp r0, #0
    bne .L_init_alarm_done
    mov r0, #1
    strh r0, [r1]
    bl OSi_SetTimerReserved
    mov r1, #0
    ldr r0, .L_alarm_queue
    str r1, [r0]
    str r1, [r0, #4]
    mov r0, #0x10
    bl OS_DisableIrqMask
.L_init_alarm_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_alarm_initialized_init:
    .word OSi_AlarmInitialized
.L_alarm_queue:
    .word OSi_AlarmQueue
.size OS_InitAlarm, . - OS_InitAlarm
