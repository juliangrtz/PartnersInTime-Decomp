/* Millisecond sleep implemented with an alarm and thread suspension. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Sleep, "ax", %progbits
.balign 4

.extern OS_CreateAlarm
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SetAlarm
.extern OS_SleepThread
.extern OS_WakeupThreadDirect
.extern OSi_CurrentThreadPtr

.global OSi_SleepAlarmCallback
.type OSi_SleepAlarmCallback, %function
OSi_SleepAlarmCallback:
    ldr r12, .L_wakeup_thread_direct
    bx r12
.L_wakeup_thread_direct:
    .word OS_WakeupThreadDirect
.size OSi_SleepAlarmCallback, . - OSi_SleepAlarmCallback

.global OS_Sleep
.type OS_Sleep, %function
OS_Sleep:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #0x34
    mov r5, r0
    add r0, sp, #4
    bl OS_CreateAlarm
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r0, .L_current_thread
    ldr r0, [r0]
    ldr r0, [r0]
    str r0, [sp]
    add r0, sp, #4
    ldr r1, .L_milliseconds_to_ticks
    mul r1, r5, r1
    lsr r1, r1, #6
    mov r2, #0
    ldr r3, .L_sleep_alarm_callback
    bl OS_SetAlarm
    mov r0, #0
    bl OS_SleepThread
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #0x34
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_current_thread:
    .word OSi_CurrentThreadPtr
.L_milliseconds_to_ticks:
    .word 0x000082EA
.L_sleep_alarm_callback:
    .word OSi_SleepAlarmCallback
.size OS_Sleep, . - OS_Sleep
