/* Execute queued touch-panel sampling and auto-sampling commands. */

.syntax unified
.arch armv4t
.arm
.section .pit_TP_ExecuteProcess, "ax", %progbits
.balign 4

.extern HW_TOUCHPANEL_X
.extern HW_TOUCHPANEL_Y
.extern OS_CancelVAlarms
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SetPeriodicVAlarm
.extern SPIi_CheckException
.extern SPIi_GetException
.extern SPIi_ReleaseException
.extern SPIi_ReturnResult
.extern TPi_UpdateTouchData
.extern TPi_VAlarmArray
.extern TPi_VAlarmCallback
.extern TPi_Work
.extern TP_ExecSampling
.extern _u32_div_f

.global TP_ExecuteProcess
.type TP_ExecuteProcess, %function
TP_ExecuteProcess:
    stmdb sp!, {r4-r10, lr}
    sub sp, sp, #16
    mov r10, r0
    ldr r1, [r10, #4]
    cmp r1, #2
    bhi .L_check_single_sample
    cmp r1, #0
    beq .L_execute_sample
    cmp r1, #1
    beq .L_start_auto_sampling
    cmp r1, #2
    beq .L_stop_auto_sampling
    b .L_process_done
.L_check_single_sample:
    cmp r1, #16
    bne .L_process_done
    ldr r0, .L_touch_work
    ldr r0, [r0, #32]
    cmp r0, #2
    bne .L_process_done
.L_execute_sample:
    bl OS_DisableInterrupts
    mov r4, r0
    mov r0, #0
    bl SPIi_CheckException
    cmp r0, #0
    bne .L_acquire_sample
    mov r0, r4
    bl OS_RestoreInterrupts
    ldr r0, [r10, #4]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_process_done
.L_acquire_sample:
    mov r0, #0
    bl SPIi_GetException
    mov r0, r4
    bl OS_RestoreInterrupts
    add r0, sp, #8
    ldr r1, .L_touch_work
    ldr r1, [r1, #36]
    add r2, sp, #4
    bl TP_ExecSampling
    add r0, sp, #8
    ldrh r1, [sp, #4]
    bl TPi_UpdateTouchData
    ldrh r1, [sp, #8]
    ldr r0, .L_touch_x
    strh r1, [r0]
    ldrh r1, [sp, #10]
    ldr r0, .L_touch_y
    strh r1, [r0]
    ldr r0, [r10, #4]
    cmp r0, #0
    bne .L_return_sample_status
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r1, #0
    bl SPIi_ReturnResult
    b .L_release_sample
.L_return_sample_status:
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    ldr r1, [r10, #8]
    and r1, r1, #0xff
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    bl SPIi_ReturnResult
.L_release_sample:
    mov r0, #0
    bl SPIi_ReleaseException
    b .L_process_done
.L_start_auto_sampling:
    ldr r7, .L_touch_work
    ldr r0, [r7, #32]
    cmp r0, #1
    bne .L_auto_sampling_busy
    mov r9, #0
    ldr r8, .L_vblank_ratio
    ldr r6, .L_valarm_array
    mov r5, #10
    ldr r4, .L_valarm_callback
    b .L_test_alarm
.L_create_alarm:
    mul r0, r9, r8
    bl _u32_div_f
    ldr r1, [r10, #12]
    add r0, r1, r0
    mov r1, r8
    bl _u32_div_f
    add r2, r7, r9, lsl #1
    strh r1, [r2, #0xcc]
    str r9, [sp]
    mov r0, #40
    mla r0, r9, r0, r6
    ldrsh r1, [r2, #0xcc]
    mov r2, r5
    mov r3, r4
    bl OS_SetPeriodicVAlarm
    add r9, r9, #1
.L_test_alarm:
    ldr r1, [r10, #8]
    cmp r9, r1
    bcc .L_create_alarm
    ldr r0, [r10, #4]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r1, #0
    bl SPIi_ReturnResult
    mov r1, #2
    ldr r0, .L_touch_work
    str r1, [r0, #32]
    b .L_process_done
.L_auto_sampling_busy:
    mov r0, r1, lsl #16
    mov r0, r0, lsr #16
    mov r1, #3
    bl SPIi_ReturnResult
    b .L_process_done
.L_stop_auto_sampling:
    ldr r0, .L_touch_work
    ldr r0, [r0, #32]
    cmp r0, #3
    bne .L_auto_sampling_busy_stop
    ldr r0, .L_valarm_tag
    bl OS_CancelVAlarms
    ldr r0, [r10, #4]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r1, #0
    bl SPIi_ReturnResult
    mov r1, #0
    ldr r0, .L_touch_work
    str r1, [r0, #32]
    b .L_process_done
.L_auto_sampling_busy_stop:
    mov r0, r1, lsl #16
    mov r0, r0, lsr #16
    mov r1, #3
    bl SPIi_ReturnResult
.L_process_done:
    add sp, sp, #16
    ldmia sp!, {r4-r10, lr}
    bx lr
.L_touch_work:
    .word TPi_Work
.L_touch_x:
    .word HW_TOUCHPANEL_X
.L_touch_y:
    .word HW_TOUCHPANEL_Y
.L_vblank_ratio:
    .word 0x00000107
.L_valarm_array:
    .word TPi_VAlarmArray
.L_valarm_callback:
    .word TPi_VAlarmCallback
.L_valarm_tag:
    .word 0x54505641
.size TP_ExecuteProcess, . - TP_ExecuteProcess
