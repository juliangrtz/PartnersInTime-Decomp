/* Low-level helpers used by the ARM7 sound command dispatcher. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_CommandHelpers, "ax", %progbits
.balign 4

.extern MIi_CpuCopy32
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SendMessage
.extern PXI_SetFifoRecvCallback
.extern SND_GetChannelControl
.extern SND_GetLockedChannel
.extern SND_SendWakeupMessage
.extern SND_SetChannelPan
.extern SND_SetChannelTimer
.extern SND_SetChannelVolume
.extern SND_StartAlarm
.extern SND_StopAlarm
.extern SND_StopChannel
.extern SND_UpdateSharedWork
.extern SNDi_CommandMesgQueue
.extern SNDi_Work

.global SNDi_ReadDriverInfo
.type SNDi_ReadDriverInfo, %function
SNDi_ReadDriverInfo:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    ldr r0, .L_sound_work_driver_info
    mov r1, r5
    mov r2, #0x1180
    bl MIi_CpuCopy32
    ldr r1, .L_sound_work_driver_info
    add r0, r5, #0x1000
    str r1, [r0, #0x1c0]
    mov r4, #0
.L_copy_channel_controls:
    mov r0, r4
    bl SND_GetChannelControl
    add r1, r5, r4, lsl #2
    add r1, r1, #0x1000
    str r0, [r1, #0x180]
    add r4, r4, #1
    cmp r4, #0x10
    blt .L_copy_channel_controls
    mov r0, #0
    bl SND_GetLockedChannel
    add r1, r5, #0x1000
    str r0, [r1, #0x1c4]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_sound_work_driver_info:
    .word SNDi_Work
.size SNDi_ReadDriverInfo, . - SNDi_ReadDriverInfo

.global SNDi_StopTimer
.type SNDi_StopTimer, %function
SNDi_StopTimer:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r7, r0
    mov r6, r1
    mov r9, r2
    mov r5, r3
    bl OS_DisableInterrupts
    mov r4, r0
    mov r8, #0
    b .L_check_stop_alarm
.L_stop_alarm_bit:
    ands r0, r9, #1
    beq .L_next_stop_alarm
    mov r0, r8
    bl SND_StopAlarm
.L_next_stop_alarm:
    add r8, r8, #1
    lsr r9, r9, #1
.L_check_stop_alarm:
    cmp r8, #8
    bge .L_begin_stop_channel
    cmp r9, #0
    bne .L_stop_alarm_bit
.L_begin_stop_channel:
    mov r8, #0
    b .L_check_stop_channel
.L_stop_channel_bit:
    ands r0, r7, #1
    beq .L_next_stop_channel
    mov r0, r8
    mov r1, r5
    bl SND_StopChannel
.L_next_stop_channel:
    add r8, r8, #1
    lsr r7, r7, #1
.L_check_stop_channel:
    cmp r8, #0x10
    bge .L_stop_captures
    cmp r7, #0
    bne .L_stop_channel_bit
.L_stop_captures:
    ands r0, r6, #1
    movne r1, #0
    ldrne r0, .L_capture_0_stop
    strbne r1, [r0]
    ands r0, r6, #2
    movne r1, #0
    ldrne r0, .L_capture_1_stop
    strbne r1, [r0]
    mov r0, r4
    bl OS_RestoreInterrupts
    bl SND_UpdateSharedWork
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.L_capture_0_stop:
    .word 0x04000508
.L_capture_1_stop:
    .word 0x04000509
.size SNDi_StopTimer, . - SNDi_StopTimer

.global SNDi_StartTimer
.type SNDi_StartTimer, %function
SNDi_StartTimer:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r0
    mov r6, r1
    mov r5, r2
    bl OS_DisableInterrupts
    mov r4, r0
    mov r2, #0
    b .L_check_start_channel
.L_start_channel_bit:
    ands r0, r7, #1
    lslne r0, r2, #4
    addne r0, r0, #0x04000000
    ldrbne r1, [r0, #0x403]
    orrne r1, r1, #0x80
    strbne r1, [r0, #0x403]
    add r2, r2, #1
    lsr r7, r7, #1
.L_check_start_channel:
    cmp r2, #0x10
    bge .L_start_captures
    cmp r7, #0
    bne .L_start_channel_bit
.L_start_captures:
    ands r0, r6, #1
    beq .L_only_capture_1
    ands r0, r6, #2
    ldreq r1, .L_capture_0_start
    ldrbeq r0, [r1]
    orreq r0, r0, #0x80
    strbeq r0, [r1]
    beq .L_begin_start_alarm
    ldr r2, .L_capture_0_start
    ldrh r1, [r2]
    ldr r0, .L_both_capture_enable
    orr r0, r1, r0
    strh r0, [r2]
    b .L_begin_start_alarm
.L_only_capture_1:
    ands r0, r6, #2
    ldrne r1, .L_capture_1_start
    ldrbne r0, [r1]
    orrne r0, r0, #0x80
    strbne r0, [r1]
.L_begin_start_alarm:
    mov r6, #0
    b .L_check_start_alarm
.L_start_alarm_bit:
    ands r0, r5, #1
    beq .L_next_start_alarm
    mov r0, r6
    bl SND_StartAlarm
.L_next_start_alarm:
    add r6, r6, #1
    lsr r5, r5, #1
.L_check_start_alarm:
    cmp r6, #8
    bge .L_finish_start_timer
    cmp r5, #0
    bne .L_start_alarm_bit
.L_finish_start_timer:
    mov r0, r4
    bl OS_RestoreInterrupts
    bl SND_UpdateSharedWork
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_capture_0_start:
    .word 0x04000508
.L_both_capture_enable:
    .word 0x00008080
.L_capture_1_start:
    .word 0x04000509
.size SNDi_StartTimer, . - SNDi_StartTimer

.global SNDi_SetChannelPan
.type SNDi_SetChannelPan, %function
SNDi_SetChannelPan:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, r1
    mov r4, #0
    b .L_check_pan_channel
.L_set_pan_channel:
    ands r0, r6, #1
    beq .L_next_pan_channel
    mov r0, r4
    mov r1, r5
    bl SND_SetChannelPan
.L_next_pan_channel:
    add r4, r4, #1
    lsr r6, r6, #1
.L_check_pan_channel:
    cmp r4, #0x10
    bge .L_set_pan_done
    cmp r6, #0
    bne .L_set_pan_channel
.L_set_pan_done:
    pop {r4, r5, r6, lr}
    bx lr
.size SNDi_SetChannelPan, . - SNDi_SetChannelPan

.global SNDi_SetChannelVolume
.type SNDi_SetChannelVolume, %function
SNDi_SetChannelVolume:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r0
    mov r6, r1
    mov r5, r2
    mov r4, #0
    b .L_check_volume_channel
.L_set_volume_channel:
    ands r0, r7, #1
    beq .L_next_volume_channel
    mov r0, r4
    mov r1, r6
    mov r2, r5
    bl SND_SetChannelVolume
.L_next_volume_channel:
    add r4, r4, #1
    lsr r7, r7, #1
.L_check_volume_channel:
    cmp r4, #0x10
    bge .L_set_volume_done
    cmp r7, #0
    bne .L_set_volume_channel
.L_set_volume_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size SNDi_SetChannelVolume, . - SNDi_SetChannelVolume

.global SNDi_SetChannelTimer
.type SNDi_SetChannelTimer, %function
SNDi_SetChannelTimer:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, r1
    mov r4, #0
    b .L_check_timer_channel
.L_set_timer_channel:
    ands r0, r6, #1
    beq .L_next_timer_channel
    mov r0, r4
    mov r1, r5
    bl SND_SetChannelTimer
.L_next_timer_channel:
    add r4, r4, #1
    lsr r6, r6, #1
.L_check_timer_channel:
    cmp r4, #0x10
    bge .L_set_timer_done
    cmp r6, #0
    bne .L_set_timer_channel
.L_set_timer_done:
    pop {r4, r5, r6, lr}
    bx lr
.size SNDi_SetChannelTimer, . - SNDi_SetChannelTimer

.global SNDi_InitPxi
.type SNDi_InitPxi, %function
SNDi_InitPxi:
    mov r0, #7
    ldr r1, .L_pxi_callback
    ldr r12, .L_set_pxi_callback
    bx r12
.L_pxi_callback:
    .word SNDi_PxiFifoCallback
.L_set_pxi_callback:
    .word PXI_SetFifoRecvCallback
.size SNDi_InitPxi, . - SNDi_InitPxi

.global SNDi_PxiFifoCallback
.type SNDi_PxiFifoCallback, %function
SNDi_PxiFifoCallback:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r1
    bl OS_DisableInterrupts
    mov r4, r0
    cmp r5, #0x02000000
    blo .L_check_wakeup_message
    ldr r0, .L_command_message_queue
    mov r1, r5
    mov r2, #0
    bl OS_SendMessage
    b .L_restore_pxi_interrupts
.L_check_wakeup_message:
    cmp r5, #0
    bne .L_restore_pxi_interrupts
    bl SND_SendWakeupMessage
.L_restore_pxi_interrupts:
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_command_message_queue:
    .word SNDi_CommandMesgQueue
.size SNDi_PxiFifoCallback, . - SNDi_PxiFifoCallback
