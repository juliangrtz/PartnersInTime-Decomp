/* Sample microphone data from timer 3 and acknowledge its IRQ. */

.syntax unified
.arch armv4t
.arm
.section .pit_MIC_Timer, "ax", %progbits
.balign 4

.extern MIC_ExecSampling12
.extern MIC_ExecSampling8
.extern MICi_Work
.extern SPIi_CheckEntry
.extern SPIi_CheckException
.extern SPIi_ReturnResult
.extern SPIi_SetEntry

.global MICi_TimerHandler
.type MICi_TimerHandler, %function
MICi_TimerHandler:
    stmdb sp!, {r4-r7, lr}
    ldr r4, .L_mic_work
    ldrh r5, [r4, #38]
    and r0, r5, #4
    cmp r0, #4
    ldrh r6, [r4, #56]
    ldrhne r7, [r4, #58]
    ldreq r7, .L_invalid_sample
    bl SPIi_CheckEntry
    cmp r0, #0
    bne .L_store_sample
    mov r0, #2
    bl SPIi_CheckException
    cmp r0, #0
    beq .L_store_sample
    and r0, r5, #1
    cmp r0, #1
    bne .L_sample_8bit
    bl MIC_ExecSampling12
    tst r5, #2
    moveq r7, r0
    eorne r7, r0, #0x8000
    b .L_store_sample
.L_sample_8bit:
    bl MIC_ExecSampling8
    tst r5, #2
    moveq r7, r0
    eorne r7, r0, #0x80
.L_store_sample:
    and r0, r5, #1
    ldr r3, .L_main_memory_work
    ldr r1, [r4, #44]
    cmp r0, #1
    bne .L_store_8bit
    ldr r2, [r4, #40]
    strh r7, [r2, r1]!
    str r2, [r3, #0x390]
    add r3, r3, #0x394
    strh r7, [r3]
    add r1, r1, #2
    b .L_update_position
.L_store_8bit:
    and r7, r7, #0xff
    tst r1, #1
    bne .L_store_8bit_pair
    mov r6, r7
    add r1, r1, #1
    b .L_update_position
.L_store_8bit_pair:
    orr r0, r6, r7, lsl #8
    ldr r2, [r4, #40]
    sub r1, r1, #1
    strh r0, [r2, r1]!
    str r2, [r3, #0x390]
    add r3, r3, #0x394
    strh r0, [r3]
    add r1, r1, #2
.L_update_position:
    strh r6, [r4, #56]
    strh r7, [r4, #58]
    ldr r0, [r4, #48]
    cmp r1, r0
    movhs r1, #0
    str r1, [r4, #44]
    blo .L_timer_done
    ldrh r0, [r4, #36]
    and r0, r0, #16
    cmp r0, #16
    bne .L_queue_buffer_complete
    mov r0, #81
    mov r1, #0
    bl SPIi_ReturnResult
    b .L_timer_done
.L_queue_buffer_complete:
    mov r0, #2
    mov r1, #66
    mov r2, #0
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_stop_sampling
    mov r0, #81
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_timer_done
.L_stop_sampling:
    mov r0, #4
    str r0, [r4, #32]
    ldr r1, .L_timer3_control
    ldrh r0, [r1]
    bic r0, r0, #0x80
    strh r0, [r1]
.L_timer_done:
    ldmia sp!, {r4-r7, lr}
    bx lr
.L_mic_work:
    .word MICi_Work
.L_invalid_sample:
    .word 0x0000ffff
.L_main_memory_work:
    .word 0x027ffc00
.L_timer3_control:
    .word 0x0400010e
.size MICi_TimerHandler, . - MICi_TimerHandler

.global MICi_TimerIrqHandler
.type MICi_TimerIrqHandler, %function
MICi_TimerIrqHandler:
    stmdb sp!, {lr}
    sub sp, sp, #4
    bl MICi_TimerHandler
    ldr r1, .L_irq_check
    ldr r0, [r1]
    orr r0, r0, #64
    str r0, [r1]
    mov r1, #64
    ldr r0, .L_if
    str r1, [r0]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_irq_check:
    .word 0x0380fff8
.L_if:
    .word 0x04000214
.size MICi_TimerIrqHandler, . - MICi_TimerIrqHandler
