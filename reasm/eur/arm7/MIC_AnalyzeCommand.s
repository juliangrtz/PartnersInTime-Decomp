/* Reassemble and validate microphone commands received over PXI. */

.syntax unified
.arch armv4t
.arm
.section .pit_MIC_AnalyzeCommand, "ax", %progbits
.balign 4

.extern MICi_SetTimerValue
.extern MICi_Work
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern SPIi_ReturnResult
.extern SPIi_SetEntry

.global MIC_AnalyzeCommand
.type MIC_AnalyzeCommand, %function
MIC_AnalyzeCommand:
    stmdb sp!, {r4, lr}
    ands r1, r0, #0x02000000
    beq .L_store_fragment
    mov r4, #0
    mov r3, r4
    ldr r1, .L_mic_work
.L_clear_fragments:
    mov r2, r4, lsl #1
    strh r3, [r1, r2]
    add r4, r4, #1
    cmp r4, #16
    blt .L_clear_fragments
.L_store_fragment:
    and r1, r0, #0x000f0000
    mov r1, r1, lsr #16
    mov r2, r1, lsl #1
    ldr r1, .L_mic_work
    strh r0, [r1, r2]
    ands r0, r0, #0x01000000
    beq .L_done
    ldrh r3, [r1]
    and r0, r3, #0xff00
    mov r0, r0, lsl #8
    mov r4, r0, lsr #16
    sub r0, r4, #64
    cmp r0, #3
    addls pc, pc, r0, lsl #2
    b .L_bad_command
    b .L_sample_once
    b .L_start_sampling
    b .L_stop_sampling
    b .L_adjust_rate
.L_sample_once:
    mov r0, #2
    mov r1, r4
    mov r2, #1
    and r3, r3, #0xff
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_clear_last_sample
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
.L_clear_last_sample:
    mov r1, #0
    ldr r0, .L_last_sample
    strh r1, [r0]
    ldr r0, .L_last_sample_ptr
    str r1, [r0]
    b .L_done
.L_start_sampling:
    ldr r0, [r1, #32]
    cmp r0, #0
    beq .L_validate_buffer
    mov r0, r4
    mov r1, #3
    bl SPIi_ReturnResult
    b .L_done
.L_validate_buffer:
    and r0, r3, #0xff
    strh r0, [r1, #36]
    ldrh r2, [r1, #2]
    ldrh r0, [r1, #4]
    orr r3, r0, r2, lsl #16
    cmp r3, #0x02000000
    blo .L_bad_parameter
    cmp r3, #0x02400000
    blo .L_validate_buffer_end
.L_bad_parameter:
    mov r0, r4
    mov r1, #2
    bl SPIi_ReturnResult
    b .L_done
.L_validate_buffer_end:
    str r3, [r1, #40]
    ldrh r2, [r1, #6]
    ldrh r0, [r1, #8]
    orr r2, r0, r2, lsl #16
    add r0, r3, r2
    cmp r0, #0x02400000
    bls .L_validate_period
    mov r0, r4
    mov r1, #2
    bl SPIi_ReturnResult
    b .L_done
.L_validate_period:
    str r2, [r1, #48]
    ldrh r2, [r1, #10]
    ldrh r0, [r1, #12]
    orr r0, r0, r2, lsl #16
    bl MICi_SetTimerValue
    cmp r0, #0
    bne .L_queue_start
    mov r0, r4
    mov r1, #2
    bl SPIi_ReturnResult
    b .L_done
.L_queue_start:
    mov r2, #0
    ldr r0, .L_mic_work
    str r2, [r0, #44]
    ldrh r1, [r0, #36]
    and r1, r1, #7
    strh r1, [r0, #38]
    mov r0, #2
    mov r1, r4
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_mark_start_pending
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_done
.L_mark_start_pending:
    mov r1, #0
    ldr r0, .L_last_sample
    strh r1, [r0]
    ldr r0, .L_last_sample_ptr
    str r1, [r0]
    mov r1, #1
    ldr r0, .L_mic_work
    str r1, [r0, #32]
    b .L_done
.L_stop_sampling:
    ldr r0, [r1, #32]
    cmp r0, #2
    beq .L_queue_stop
    mov r0, r4
    mov r1, #3
    bl SPIi_ReturnResult
    b .L_done
.L_queue_stop:
    mov r0, #2
    mov r1, r4
    mov r2, #0
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_mark_stop_pending
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_done
.L_mark_stop_pending:
    mov r1, #3
    ldr r0, .L_mic_work
    str r1, [r0, #32]
    ldr r1, .L_timer3_control
    ldrh r0, [r1]
    bic r0, r0, #0x80
    strh r0, [r1]
    b .L_done
.L_adjust_rate:
    ldr r0, [r1, #32]
    cmp r0, #2
    beq .L_validate_new_period
    mov r0, r4
    mov r1, #3
    bl SPIi_ReturnResult
    b .L_done
.L_validate_new_period:
    ldrh r2, [r1, #2]
    ldrh r0, [r1, #4]
    orr r0, r0, r2, lsl #16
    bl MICi_SetTimerValue
    cmp r0, #0
    bne .L_program_new_period
    mov r0, r4
    mov r1, #2
    bl SPIi_ReturnResult
    b .L_done
.L_program_new_period:
    bl OS_DisableInterrupts
    ldr ip, .L_timer3_control
    ldrh r1, [ip]
    bic r1, r1, #0x80
    strh r1, [ip]
    ldr r1, .L_mic_work
    ldrh r3, [r1, #52]
    ldr r2, .L_timer3_count
    strh r3, [r2]
    ldrh r1, [r1, #54]
    orr r1, r1, #0xc0
    strh r1, [ip]
    bl OS_RestoreInterrupts
    mov r0, r4
    mov r1, #0
    bl SPIi_ReturnResult
    b .L_done
.L_bad_command:
    mov r0, r4
    mov r1, #1
    bl SPIi_ReturnResult
.L_done:
    ldmia sp!, {r4, lr}
    bx lr
.L_mic_work:
    .word MICi_Work
.L_last_sample:
    .word 0x027fff94
.L_last_sample_ptr:
    .word 0x027fff90
.L_timer3_control:
    .word 0x0400010e
.L_timer3_count:
    .word 0x0400010c
.size MIC_AnalyzeCommand, . - MIC_AnalyzeCommand
