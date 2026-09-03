/* Schedule ARM7 sound alarms and report expirations to ARM9 over PXI. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_Alarm, "ax", %progbits
.balign 4

.extern OS_CancelAlarm
.extern OS_CreateAlarm
.extern OS_GetTick
.extern OS_SetAlarm
.extern OS_SetPeriodicAlarm
.extern PXI_SendWordByFifo
.extern SNDi_AlarmWork

.global SNDi_AlarmHandler
.type SNDi_AlarmHandler, %function
SNDi_AlarmHandler:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, #7
    mov r4, #0
.L_send_alarm_message:
    mov r0, r5
    mov r1, r6
    mov r2, r4
    bl PXI_SendWordByFifo
    cmp r0, #0
    blt .L_send_alarm_message
    pop {r4, r5, r6, lr}
    bx lr
.size SNDi_AlarmHandler, . - SNDi_AlarmHandler

.global SND_StopAlarm
.type SND_StopAlarm, %function
SND_StopAlarm:
    push {r4, lr}
    ldr r1, .L_alarm_work_stop
    add r4, r1, r0, lsl #6
    ldrb r0, [r1, r0, lsl #6]
    cmp r0, #0
    beq .L_stop_alarm_done
    add r0, r4, #0x14
    bl OS_CancelAlarm
    ldrb r0, [r4, #1]
    add r0, r0, #1
    strb r0, [r4, #1]
    mov r0, #0
    strb r0, [r4]
.L_stop_alarm_done:
    pop {r4, lr}
    bx lr
.L_alarm_work_stop:
    .word SNDi_AlarmWork
.size SND_StopAlarm, . - SND_StopAlarm

.global SND_StartAlarm
.type SND_StartAlarm, %function
SND_StartAlarm:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #0xc
    mov r4, r0
    ldr r0, .L_alarm_work_start
    add r5, r0, r4, lsl #6
    ldrb r0, [r0, r4, lsl #6]
    cmp r0, #0
    beq .L_load_alarm_parameters
    add r0, r5, #0x14
    bl OS_CancelAlarm
    mov r0, #0
    strb r0, [r5]
.L_load_alarm_parameters:
    ldr r9, [r5, #4]
    ldr r8, [r5, #8]
    ldr r7, [r5, #0xc]
    ldr r6, [r5, #0x10]
    ldrb r0, [r5, #1]
    orr r4, r4, r0, lsl #8
    add r0, r5, #0x14
    bl OS_CreateAlarm
    mov r0, #0
    cmp r6, r0
    cmpeq r7, r0
    bne .L_start_periodic_alarm
    str r4, [sp]
    add r0, r5, #0x14
    mov r1, r9
    mov r2, r8
    ldr r3, .L_alarm_handler_start
    bl OS_SetAlarm
    b .L_mark_alarm_active
.L_start_periodic_alarm:
    bl OS_GetTick
    mov r2, r0
    ldr r0, .L_alarm_handler_start
    str r0, [sp, #4]
    str r4, [sp, #8]
    mov r3, r7
    str r6, [sp]
    add r0, r5, #0x14
    adds r4, r9, r2
    adc r2, r8, r1
    mov r1, r4
    bl OS_SetPeriodicAlarm
.L_mark_alarm_active:
    mov r0, #1
    strb r0, [r5]
    add sp, sp, #0xc
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.L_alarm_work_start:
    .word SNDi_AlarmWork
.L_alarm_handler_start:
    .word SNDi_AlarmHandler
.size SND_StartAlarm, . - SND_StartAlarm

.global SND_SetupAlarm
.type SND_SetupAlarm, %function
SND_SetupAlarm:
    push {r4, r5, r6, r7, r8, lr}
    mov r8, r1
    mov r7, r2
    mov r6, r3
    ldr r5, [sp, #0x18]
    ldr r1, .L_alarm_work_setup
    add r4, r1, r0, lsl #6
    ldrb r0, [r1, r0, lsl #6]
    cmp r0, #0
    beq .L_store_alarm_parameters
    add r0, r4, #0x14
    bl OS_CancelAlarm
    mov r0, #0
    strb r0, [r4]
.L_store_alarm_parameters:
    str r8, [r4, #4]
    str r7, [r4, #8]
    str r6, [r4, #0xc]
    str r5, [r4, #0x10]
    ldr r0, [sp, #0x1c]
    strb r0, [r4, #1]
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.L_alarm_work_setup:
    .word SNDi_AlarmWork
.size SND_SetupAlarm, . - SND_SetupAlarm
