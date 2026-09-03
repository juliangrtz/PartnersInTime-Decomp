/* Execute one-shot, start, and stop microphone requests. */

.syntax unified
.arch armv4t
.arm
.section .pit_MIC_ExecuteProcess, "ax", %progbits
.balign 4

.extern MIC_DisableMultipleInterrupt
.extern MIC_EnableMultipleInterrupt
.extern MIC_ExecSampling12
.extern MIC_ExecSampling8
.extern MIC_SetIrqFunction
.extern MICi_TimerIrqHandler
.extern MICi_Work
.extern OS_DisableInterrupts
.extern OS_EnableIrqMask
.extern OS_RestoreInterrupts
.extern SPIi_CheckException
.extern SPIi_GetException
.extern SPIi_ReleaseException
.extern SPIi_ReturnResult

.global MIC_ExecuteProcess
.type MIC_ExecuteProcess, %function
MIC_ExecuteProcess:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    ldr r2, [r5, #4]
    cmp r2, #64
    beq .L_sample_once
    cmp r2, #65
    beq .L_start_sampling
    cmp r2, #66
    beq .L_stop_sampling
    b .L_execute_done
.L_sample_once:
    bl OS_DisableInterrupts
    mov r4, r0
    mov r0, #2
    bl SPIi_CheckException
    cmp r0, #0
    bne .L_acquire_exception
    mov r0, r4
    bl OS_RestoreInterrupts
    ldr r0, [r5, #4]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_execute_done
.L_acquire_exception:
    mov r0, #2
    bl SPIi_GetException
    mov r0, r4
    bl OS_RestoreInterrupts
    ldr r0, [r5, #8]
    and r0, r0, #1
    cmp r0, #1
    bne .L_sample_once_8bit
    bl MIC_ExecSampling12
    ldr r1, [r5, #8]
    ands r1, r1, #2
    eorne r0, r0, #0x8000
    movne r0, r0, lsl #16
    movne r0, r0, lsr #16
    ldr r1, .L_last_sample
    strh r0, [r1]
    ldr r0, .L_last_sample_ptr
    str r1, [r0]
    b .L_return_sample
.L_sample_once_8bit:
    bl MIC_ExecSampling8
    ldr r1, [r5, #8]
    ands r1, r1, #2
    eorne r0, r0, #0x80
    movne r0, r0, lsl #16
    movne r0, r0, lsr #16
    ldr r1, .L_last_sample
    strh r0, [r1]
    ldr r0, .L_last_sample_ptr
    str r1, [r0]
.L_return_sample:
    ldr r0, [r5, #4]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r1, #0
    bl SPIi_ReturnResult
    mov r0, #2
    bl SPIi_ReleaseException
    b .L_execute_done
.L_start_sampling:
    ldr r0, .L_mic_work
    ldr r1, [r0, #32]
    cmp r1, #1
    bne .L_sampling_busy
    mov r1, #0
    strh r1, [r0, #58]
    strh r1, [r0, #56]
    bl OS_DisableInterrupts
    mov r4, r0
    mov r0, #64
    bl OS_EnableIrqMask
    mov r0, #64
    ldr r1, .L_timer_irq_handler
    bl MIC_SetIrqFunction
    bl MIC_EnableMultipleInterrupt
    ldr r0, .L_mic_work
    ldrh r2, [r0, #52]
    ldr r1, .L_timer3_count
    strh r2, [r1]
    ldrh r0, [r0, #54]
    orr r1, r0, #0xc0
    ldr r0, .L_timer3_control
    strh r1, [r0]
    mov r0, r4
    bl OS_RestoreInterrupts
    ldr r0, [r5, #4]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r1, #0
    bl SPIi_ReturnResult
    mov r1, #2
    ldr r0, .L_mic_work
    str r1, [r0, #32]
    b .L_execute_done
.L_sampling_busy:
    mov r0, r2, lsl #16
    mov r0, r0, lsr #16
    mov r1, #3
    bl SPIi_ReturnResult
    b .L_execute_done
.L_stop_sampling:
    ldr r0, .L_mic_work
    ldr r1, [r0, #32]
    sub r0, r1, #3
    cmp r0, #1
    bhi .L_not_stoppable
    ldr r1, .L_timer3_control
    ldrh r0, [r1]
    bic r0, r0, #0x80
    strh r0, [r1]
    bl OS_DisableInterrupts
    mov r4, r0
    mov r0, #64
    mov r1, #0
    bl MIC_SetIrqFunction
    bl MIC_DisableMultipleInterrupt
    mov r0, r4
    bl OS_RestoreInterrupts
    ldr r0, .L_mic_work
    ldr r0, [r0, #32]
    cmp r0, #3
    bne .L_return_buffer_stop
    mov r0, #66
    mov r1, #0
    bl SPIi_ReturnResult
    b .L_clear_sampling_state
.L_return_buffer_stop:
    mov r0, #81
    mov r1, #0
    bl SPIi_ReturnResult
.L_clear_sampling_state:
    mov r1, #0
    ldr r0, .L_mic_work
    str r1, [r0, #32]
    b .L_execute_done
.L_not_stoppable:
    cmp r1, #3
    bne .L_buffer_not_stoppable
    mov r0, #66
    mov r1, #3
    bl SPIi_ReturnResult
    b .L_execute_done
.L_buffer_not_stoppable:
    mov r0, #81
    mov r1, #3
    bl SPIi_ReturnResult
.L_execute_done:
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_last_sample:
    .word 0x027fff94
.L_last_sample_ptr:
    .word 0x027fff90
.L_mic_work:
    .word MICi_Work
.L_timer_irq_handler:
    .word MICi_TimerIrqHandler
.L_timer3_count:
    .word 0x0400010c
.L_timer3_control:
    .word 0x0400010e
.size MIC_ExecuteProcess, . - MIC_ExecuteProcess

/* Select timer 3 prescaling and reload values for a requested period. */
.global MICi_SetTimerValue
.type MICi_SetTimerValue, %function
MICi_SetTimerValue:
    cmp r0, #0x00010000
    bhs .L_try_div64
    mov r2, #0
    ldr r1, .L_timer_work
    strh r2, [r1, #54]
    rsb r0, r0, #0x00010000
    strh r0, [r1, #52]
    mov r0, #1
    bx lr
.L_try_div64:
    cmp r0, #0x00400000
    bhs .L_try_div256
    mov r2, #1
    ldr r1, .L_timer_work
    strh r2, [r1, #54]
    mov r0, r0, lsr #6
    rsb r0, r0, #0x00010000
    strh r0, [r1, #52]
    mov r0, r2
    bx lr
.L_try_div256:
    cmp r0, #0x01000000
    bhs .L_try_div1024
    mov r2, #2
    ldr r1, .L_timer_work
    strh r2, [r1, #54]
    mov r0, r0, lsr #8
    rsb r0, r0, #0x00010000
    strh r0, [r1, #52]
    mov r0, #1
    bx lr
.L_try_div1024:
    cmp r0, #0x04000000
    movhs r0, #0
    bxhs lr
    mov r2, #3
    ldr r1, .L_timer_work
    strh r2, [r1, #54]
    mov r0, r0, lsr #10
    rsb r0, r0, #0x00010000
    strh r0, [r1, #52]
    mov r0, #1
    bx lr
.L_timer_work:
    .word MICi_Work
.size MICi_SetTimerValue, . - MICi_SetTimerValue
