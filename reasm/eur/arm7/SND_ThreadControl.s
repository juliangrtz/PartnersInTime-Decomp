/* ARM7 sound-thread messaging, interval timer, and startup control. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_ThreadControl, "ax", %progbits
.balign 4

.extern OS_CancelAlarm
.extern OS_CreateAlarm
.extern OS_CreateThread
.extern OS_GetTick
.extern OS_InitMessageQueue
.extern OS_ReceiveMessage
.extern OS_SendMessage
.extern OS_SetPeriodicAlarm
.extern OS_WakeupThreadDirect
.extern OSi_WarningPtr
.extern SND_CommandInit
.extern SndThread
.extern sFailedSoundAlarmMessage
.extern sSndInitialized
.extern sSndIntervalAlarm
.extern sSndMessageBuffer
.extern sSndMessageQueue
.extern sSndThread
.extern sSndThreadStackTop

.type SndAlarmCallback, %function
SndAlarmCallback:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_alarm_message_queue
    mov r1, #1
    mov r2, #0
    bl OS_SendMessage
    cmp r0, #0
    bne .L_alarm_message_sent
    ldr r0, .L_alarm_warning_message
    ldr r1, .L_warning_function_pointer
    ldr r1, [r1]
    mov lr, pc
    bx r1
.L_alarm_message_sent:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SndAlarmCallback, . - SndAlarmCallback
.L_alarm_message_queue:
    .word sSndMessageQueue
.L_alarm_warning_message:
    .word sFailedSoundAlarmMessage
.L_warning_function_pointer:
    .word OSi_WarningPtr

.global SNDi_UnlockMutex
.type SNDi_UnlockMutex, %function
SNDi_UnlockMutex:
    bx lr
.size SNDi_UnlockMutex, . - SNDi_UnlockMutex

.global SNDi_LockMutex
.type SNDi_LockMutex, %function
SNDi_LockMutex:
    bx lr
.size SNDi_LockMutex, . - SNDi_LockMutex

.global SND_SendWakeupMessage
.type SND_SendWakeupMessage, %function
SND_SendWakeupMessage:
    ldr r0, .L_wakeup_message_queue
    mov r1, #2
    mov r2, #0
    ldr r12, .L_send_message
    bx r12
.size SND_SendWakeupMessage, . - SND_SendWakeupMessage
.L_wakeup_message_queue:
    .word sSndMessageQueue
.L_send_message:
    .word OS_SendMessage

.global SND_WaitForIntervalTimer
.type SND_WaitForIntervalTimer, %function
SND_WaitForIntervalTimer:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_wait_message_queue
    add r1, sp, #0
    mov r2, #1
    bl OS_ReceiveMessage
    ldr r0, [sp]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SND_WaitForIntervalTimer, . - SND_WaitForIntervalTimer
.L_wait_message_queue:
    .word sSndMessageQueue

.global SND_StopIntervalTimer
.type SND_StopIntervalTimer, %function
SND_StopIntervalTimer:
    ldr r0, .L_stop_interval_alarm
    ldr r12, .L_cancel_alarm
    bx r12
.size SND_StopIntervalTimer, . - SND_StopIntervalTimer
.L_stop_interval_alarm:
    .word sSndIntervalAlarm
.L_cancel_alarm:
    .word OS_CancelAlarm

.global SND_StartIntervalTimer
.type SND_StartIntervalTimer, %function
SND_StartIntervalTimer:
    stmdb sp!, {lr}
    sub sp, sp, #0x0C
    bl OS_GetTick
    mov r12, r0
    ldr r0, .L_interval_callback
    str r0, [sp, #4]
    mov r0, #0
    str r0, [sp, #8]
    ldr r3, .L_interval_period
    str r0, [sp]
    ldr r0, .L_start_interval_alarm
    mov r2, #0x10000
    adds r12, r12, r2
    adc r2, r1, #0
    mov r1, r12
    bl OS_SetPeriodicAlarm
    add sp, sp, #0x0C
    ldmia sp!, {lr}
    bx lr
.size SND_StartIntervalTimer, . - SND_StartIntervalTimer
.L_interval_callback:
    .word SndAlarmCallback
.L_interval_period:
    .word 0x00000AA8
.L_start_interval_alarm:
    .word sSndIntervalAlarm

.global SND_InitIntervalTimer
.type SND_InitIntervalTimer, %function
SND_InitIntervalTimer:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_init_message_queue
    ldr r1, .L_init_message_buffer
    mov r2, #8
    bl OS_InitMessageQueue
    ldr r0, .L_init_interval_alarm
    bl OS_CreateAlarm
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SND_InitIntervalTimer, . - SND_InitIntervalTimer
.L_init_message_queue:
    .word sSndMessageQueue
.L_init_message_buffer:
    .word sSndMessageBuffer
.L_init_interval_alarm:
    .word sSndIntervalAlarm

.global SND_CreateThread
.type SND_CreateThread, %function
SND_CreateThread:
    stmdb sp!, {lr}
    sub sp, sp, #0x0C
    mov r1, #0x400
    str r1, [sp]
    str r0, [sp, #4]
    ldr r0, .L_sound_thread
    ldr r1, .L_sound_thread_entry
    mov r2, #0
    ldr r3, .L_sound_thread_stack_top
    bl OS_CreateThread
    ldr r0, .L_sound_thread
    bl OS_WakeupThreadDirect
    add sp, sp, #0x0C
    ldmia sp!, {lr}
    bx lr
.size SND_CreateThread, . - SND_CreateThread
.L_sound_thread:
    .word sSndThread
.L_sound_thread_entry:
    .word SndThread
.L_sound_thread_stack_top:
    .word sSndThreadStackTop

.global SND_Init
.type SND_Init, %function
SND_Init:
    push {r4, lr}
    mov r4, r0
    ldr r0, .L_sound_initialized
    ldr r1, [r0]
    cmp r1, #0
    bne .L_sound_init_done
    mov r1, #1
    str r1, [r0]
    bl SND_CommandInit
    mov r0, r4
    bl SND_CreateThread
.L_sound_init_done:
    pop {r4, lr}
    bx lr
.size SND_Init, . - SND_Init
.L_sound_initialized:
    .word sSndInitialized
