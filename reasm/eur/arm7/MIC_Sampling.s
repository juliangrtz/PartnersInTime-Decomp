/* Initialize microphone state and read raw 12- or 8-bit ADC samples. */

.syntax unified
.arch armv4t
.arm
.section .pit_MIC_Sampling, "ax", %progbits
.balign 4

.extern MICi_Work

.global MIC_Init
.type MIC_Init, %function
MIC_Init:
    mov r3, #0
    ldr r0, .L_mic_work
    str r3, [r0, #32]
    mov r2, r3
.L_clear_mic_work:
    mov r1, r3, lsl #1
    strh r2, [r0, r1]
    add r3, r3, #1
    cmp r3, #16
    blt .L_clear_mic_work
    ldr r1, .L_timer3_control
    ldrh r0, [r1]
    bic r0, r0, #0x80
    strh r0, [r1]
    bx lr
.L_mic_work:
    .word MICi_Work
.L_timer3_control:
    .word 0x0400010e
.size MIC_Init, . - MIC_Init

.global MIC_ExecSampling12
.type MIC_ExecSampling12, %function
MIC_ExecSampling12:
    ldr r1, .L_sample12_spicnt
.L_sample12_wait_command:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_sample12_wait_command
    ldr r0, .L_sample12_enable
    strh r0, [r1]
    mov r1, #0xe4
    ldr r0, .L_sample12_spidata
    strh r1, [r0]
    ldr r1, .L_sample12_spicnt
.L_sample12_wait_high:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_sample12_wait_high
    mov r1, #0
    ldr r0, .L_sample12_spidata
    strh r1, [r0]
    ldr r2, .L_sample12_spicnt
.L_sample12_wait_low:
    ldrh r0, [r2]
    ands r0, r0, #0x80
    bne .L_sample12_wait_low
    ldr r1, .L_sample12_spidata
    ldrh r0, [r1]
    and r0, r0, #0xff
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r0, r0, lsl #24
    mov r3, r0, lsr #16
    ldr r0, .L_sample12_disable
    strh r0, [r2]
    mov r0, #0
    strh r0, [r1]
    ldr r1, .L_sample12_spicnt
.L_sample12_wait_done:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_sample12_wait_done
    ldr r0, .L_sample12_spidata
    ldrh r0, [r0]
    and r0, r0, #0xff
    mov r0, r0, lsl #16
    orr r1, r3, r0, lsr #16
    ldr r0, .L_sample12_mask
    and r0, r1, r0
    mov r0, r0, lsl #17
    mov r0, r0, lsr #16
    bx lr
.L_sample12_spicnt:
    .word 0x040001c0
.L_sample12_enable:
    .word 0x00008a01
.L_sample12_spidata:
    .word 0x040001c2
.L_sample12_disable:
    .word 0x00008201
.L_sample12_mask:
    .word 0x00007ff8
.size MIC_ExecSampling12, . - MIC_ExecSampling12

.global MIC_ExecSampling8
.type MIC_ExecSampling8, %function
MIC_ExecSampling8:
    ldr r1, .L_sample8_spicnt
.L_sample8_wait_command:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_sample8_wait_command
    ldr r0, .L_sample8_enable
    strh r0, [r1]
    mov r1, #0xec
    ldr r0, .L_sample8_spidata
    strh r1, [r0]
    ldr r1, .L_sample8_spicnt
.L_sample8_wait_high:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_sample8_wait_high
    mov r1, #0
    ldr r0, .L_sample8_spidata
    strh r1, [r0]
    ldr r2, .L_sample8_spicnt
.L_sample8_wait_low:
    ldrh r0, [r2]
    ands r0, r0, #0x80
    bne .L_sample8_wait_low
    ldr r1, .L_sample8_spidata
    ldrh r0, [r1]
    and r0, r0, #0xff
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r0, r0, lsl #24
    mov r3, r0, lsr #16
    ldr r0, .L_sample8_disable
    strh r0, [r2]
    mov r0, #0
    strh r0, [r1]
    ldr r1, .L_sample8_spicnt
.L_sample8_wait_done:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_sample8_wait_done
    ldr r0, .L_sample8_spidata
    ldrh r0, [r0]
    and r0, r0, #0xff
    mov r0, r0, lsl #16
    orr r1, r3, r0, lsr #16
    ldr r0, .L_sample8_mask
    and r0, r1, r0
    mov r0, r0, lsl #9
    mov r0, r0, lsr #16
    bx lr
.L_sample8_spicnt:
    .word 0x040001c0
.L_sample8_enable:
    .word 0x00008a01
.L_sample8_spidata:
    .word 0x040001c2
.L_sample8_disable:
    .word 0x00008201
.L_sample8_mask:
    .word 0x00007f80
.size MIC_ExecSampling8, . - MIC_ExecSampling8
