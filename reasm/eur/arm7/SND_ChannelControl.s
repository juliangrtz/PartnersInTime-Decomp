/* NitroSDK sound-channel register control and PCM/PSG/noise setup. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_ChannelControl, "ax", %progbits
.balign 4

.extern REG_SOUND_CHANNEL0
.extern sMasterPan
.extern sOrgPan
.extern sOrgVolume
.extern sSurroundDecay

.global CalcSurroundDecay
.type CalcSurroundDecay, %function
CalcSurroundDecay:
    cmp r1, #0x18
    bge .L_decay_right
    ldr r2, .L_surround_decay_calc
    ldr r3, [r2]
    ldr r2, .L_decay_full_scale
    sub r2, r2, r3
    add r1, r1, #0x28
    mul r1, r3, r1
    add r1, r1, r2, lsl #6
    mul r1, r0, r1
    mov r0, r1, asr #21
    bx lr
.L_decay_right:
    cmp r1, #0x68
    bxle lr
    ldr r2, .L_surround_decay_calc
    ldr r12, [r2]
    ldr r2, .L_decay_full_scale
    add r3, r12, r2
    rsb r2, r12, #0
    sub r1, r1, #0x28
    mul r1, r2, r1
    add r1, r1, r3, lsl #6
    mul r1, r0, r1
    mov r0, r1, asr #21
    bx lr
.size CalcSurroundDecay, . - CalcSurroundDecay
.L_surround_decay_calc:
    .word sSurroundDecay
.L_decay_full_scale:
    .word 0x00007FFF

.global SNDi_SetSurroundDecay
.type SNDi_SetSurroundDecay, %function
SNDi_SetSurroundDecay:
    push {r4, r5, r6, r7, r8, lr}
    ldr r1, .L_surround_decay_set
    str r0, [r1]
    mov r4, #0
    ldr r6, .L_original_volume_decay
    mov r7, #1
    ldr r5, .L_surround_channel_mask
.L_decay_channels:
    mov r0, r7, lsl r4
    ands r0, r0, r5
    beq .L_decay_next
    mov r8, r4, lsl #4
    add r0, r8, #0x04000000
    ldrb r1, [r0, #0x402]
    ldrb r0, [r6, r4]
    bl CalcSurroundDecay
    add r1, r8, #0x04000000
    strb r0, [r1, #0x400]
.L_decay_next:
    add r4, r4, #1
    cmp r4, #0x10
    blt .L_decay_channels
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.size SNDi_SetSurroundDecay, . - SNDi_SetSurroundDecay
.L_surround_decay_set:
    .word sSurroundDecay
.L_original_volume_decay:
    .word sOrgVolume
.L_surround_channel_mask:
    .word 0x0000FFF5

.global SND_GetChannelControl
.type SND_GetChannelControl, %function
SND_GetChannelControl:
    mov r0, r0, lsl #4
    add r0, r0, #0x04000000
    ldr r0, [r0, #0x400]
    bx lr
.size SND_GetChannelControl, . - SND_GetChannelControl

.global SND_SetMasterPan
.type SND_SetMasterPan, %function
SND_SetMasterPan:
    ldr r1, .L_master_pan_set
    str r0, [r1]
    cmp r0, #0
    blt .L_restore_channel_pans
    mov r2, #0
    and r1, r0, #0xFF
.L_set_master_pan_channels:
    mov r0, r2, lsl #4
    add r0, r0, #0x04000000
    strb r1, [r0, #0x402]
    add r2, r2, #1
    cmp r2, #0x10
    blt .L_set_master_pan_channels
    bx lr
.L_restore_channel_pans:
    mov r3, #0
    ldr r2, .L_original_pan_master
.L_restore_pan_channels:
    ldrb r1, [r2, r3]
    mov r0, r3, lsl #4
    add r0, r0, #0x04000000
    strb r1, [r0, #0x402]
    add r3, r3, #1
    cmp r3, #0x10
    blt .L_restore_pan_channels
    bx lr
.size SND_SetMasterPan, . - SND_SetMasterPan
.L_master_pan_set:
    .word sMasterPan
.L_original_pan_master:
    .word sOrgPan

.global SND_IsChannelActive
.type SND_IsChannelActive, %function
SND_IsChannelActive:
    mov r0, r0, lsl #4
    add r0, r0, #0x04000000
    ldrb r0, [r0, #0x403]
    ands r0, r0, #0x80
    movne r0, #1
    moveq r0, #0
    bx lr
.size SND_IsChannelActive, . - SND_IsChannelActive

.global SND_SetChannelPan
.type SND_SetChannelPan, %function
SND_SetChannelPan:
    push {r4, lr}
    ldr r2, .L_original_pan_channel
    strb r1, [r2, r0]
    ldr r2, .L_master_pan_channel
    ldr r2, [r2]
    cmp r2, #0
    movge r1, r2
    mov r4, r0, lsl #4
    add r2, r4, #0x04000000
    strb r1, [r2, #0x402]
    ldr r2, .L_surround_decay_channel
    ldr r2, [r2]
    cmp r2, #0
    ble .L_set_pan_return
    mov r2, #1
    mov r3, r2, lsl r0
    ldr r2, .L_surround_mask_channel
    ands r2, r3, r2
    beq .L_set_pan_return
    ldr r2, .L_original_volume_channel
    ldrb r0, [r2, r0]
    bl CalcSurroundDecay
    add r1, r4, #0x04000000
    strb r0, [r1, #0x400]
.L_set_pan_return:
    pop {r4, lr}
    bx lr
.size SND_SetChannelPan, . - SND_SetChannelPan
.L_original_pan_channel:
    .word sOrgPan
.L_master_pan_channel:
    .word sMasterPan
.L_surround_decay_channel:
    .word sSurroundDecay
.L_surround_mask_channel:
    .word 0x0000FFF5
.L_original_volume_channel:
    .word sOrgVolume

.global SND_SetChannelTimer
.type SND_SetChannelTimer, %function
SND_SetChannelTimer:
    rsb r1, r1, #0x10000
    mov r0, r0, lsl #4
    add r0, r0, #0x04000000
    add r0, r0, #0x400
    strh r1, [r0, #8]
    bx lr
.size SND_SetChannelTimer, . - SND_SetChannelTimer

.global SND_SetChannelVolume
.type SND_SetChannelVolume, %function
SND_SetChannelVolume:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, r2
    ldr r0, .L_original_volume_set
    strb r1, [r0, r5]
    ldr r0, .L_surround_decay_volume
    ldr r0, [r0]
    cmp r0, #0
    ble .L_set_volume_register
    mov r0, #1
    mov r2, r0, lsl r5
    ldr r0, .L_surround_mask_volume
    ands r0, r2, r0
    beq .L_set_volume_register
    mov r0, r5, lsl #4
    add r0, r0, #0x04000000
    ldrb r2, [r0, #0x402]
    mov r0, r1
    mov r1, r2
    bl CalcSurroundDecay
    mov r1, r0
.L_set_volume_register:
    orr r1, r1, r4, lsl #8
    mov r0, r5, lsl #4
    add r0, r0, #0x04000000
    add r0, r0, #0x400
    strh r1, [r0]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SND_SetChannelVolume, . - SND_SetChannelVolume
.L_original_volume_set:
    .word sOrgVolume
.L_surround_decay_volume:
    .word sSurroundDecay
.L_surround_mask_volume:
    .word 0x0000FFF5

.global SND_StopChannel
.type SND_StopChannel, %function
SND_StopChannel:
    mov r3, r0, lsl #4
    ldr r2, .L_channel0_stop
    add r12, r2, r0, lsl #4
    add r0, r3, #0x04000000
    ldr r0, [r0, #0x400]
    bic r2, r0, #0x80000000
    ands r0, r1, #1
    orrne r2, r2, #0x8000
    str r2, [r12]
    bx lr
.size SND_StopChannel, . - SND_StopChannel
.L_channel0_stop:
    .word REG_SOUND_CHANNEL0

.global SND_SetupChannelNoise
.type SND_SetupChannelNoise, %function
SND_SetupChannelNoise:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r2
    mov r6, r3
    ldr r5, [sp, #0x18]
    mov r4, r0, lsl #4
    ldr r2, .L_original_pan_noise
    strb r5, [r2, r0]
    ldr r2, .L_master_pan_noise
    ldr r2, [r2]
    cmp r2, #0
    movge r5, r2
    ldr r2, .L_original_volume_noise
    strb r1, [r2, r0]
    ldr r2, .L_surround_decay_noise
    ldr r2, [r2]
    cmp r2, #0
    ble .L_setup_noise_registers
    mov r2, #1
    mov r2, r2, lsl r0
    ldr r0, .L_surround_mask_noise
    ands r0, r2, r0
    beq .L_setup_noise_registers
    mov r0, r1
    mov r1, r5
    bl CalcSurroundDecay
    mov r1, r0
.L_setup_noise_registers:
    mov r0, r5, lsl #16
    orr r0, r0, #0x60000000
    orr r0, r0, r7, lsl #8
    orr r1, r1, r0
    add r0, r4, #0x04000000
    str r1, [r0, #0x400]
    rsb r1, r6, #0x10000
    add r0, r0, #0x400
    strh r1, [r0, #8]
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size SND_SetupChannelNoise, . - SND_SetupChannelNoise
.L_original_pan_noise:
    .word sOrgPan
.L_master_pan_noise:
    .word sMasterPan
.L_original_volume_noise:
    .word sOrgVolume
.L_surround_decay_noise:
    .word sSurroundDecay
.L_surround_mask_noise:
    .word 0x0000FFF5

.global SND_SetupChannelPsg
.type SND_SetupChannelPsg, %function
SND_SetupChannelPsg:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r1
    mov r6, r3
    ldr r5, [sp, #0x1C]
    mov r4, r0, lsl #4
    ldr r1, .L_original_pan_psg
    strb r5, [r1, r0]
    ldr r1, .L_master_pan_psg
    ldr r1, [r1]
    cmp r1, #0
    movge r5, r1
    ldr r1, .L_original_volume_psg
    strb r2, [r1, r0]
    ldr r1, .L_surround_decay_psg
    ldr r1, [r1]
    cmp r1, #0
    ble .L_setup_psg_registers
    mov r1, #1
    mov r1, r1, lsl r0
    ldr r0, .L_surround_mask_psg
    ands r0, r1, r0
    beq .L_setup_psg_registers
    mov r0, r2
    mov r1, r5
    bl CalcSurroundDecay
    mov r2, r0
.L_setup_psg_registers:
    mov r0, r7, lsl #24
    orr r0, r0, #0x60000000
    orr r0, r0, r5, lsl #16
    orr r0, r0, r6, lsl #8
    orr r1, r2, r0
    add r0, r4, #0x04000000
    str r1, [r0, #0x400]
    ldr r1, [sp, #0x18]
    rsb r1, r1, #0x10000
    add r0, r0, #0x400
    strh r1, [r0, #8]
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size SND_SetupChannelPsg, . - SND_SetupChannelPsg
.L_original_pan_psg:
    .word sOrgPan
.L_master_pan_psg:
    .word sMasterPan
.L_original_volume_psg:
    .word sOrgVolume
.L_surround_decay_psg:
    .word sSurroundDecay
.L_surround_mask_psg:
    .word 0x0000FFF5

.global SND_SetupChannelPcm
.type SND_SetupChannelPcm, %function
SND_SetupChannelPcm:
    push {r4, r5, r6, r7, r8, lr}
    mov r8, r1
    mov r7, r2
    mov r6, r3
    ldr r1, [sp, #0x20]
    ldr r5, [sp, #0x2C]
    mov r4, r0, lsl #4
    ldr r2, .L_original_pan_pcm
    strb r5, [r2, r0]
    ldr r2, .L_master_pan_pcm
    ldr r2, [r2]
    cmp r2, #0
    movge r5, r2
    ldr r2, .L_original_volume_pcm
    strb r1, [r2, r0]
    ldr r2, .L_surround_decay_pcm
    ldr r2, [r2]
    cmp r2, #0
    ble .L_setup_pcm_registers
    mov r2, #1
    mov r2, r2, lsl r0
    ldr r0, .L_surround_mask_pcm
    ands r0, r2, r0
    beq .L_setup_pcm_registers
    mov r0, r1
    mov r1, r5
    bl CalcSurroundDecay
    mov r1, r0
.L_setup_pcm_registers:
    ldr r2, [sp, #0x24]
    mov r0, r7, lsl #29
    orr r0, r0, r6, lsl #27
    orr r0, r0, r5, lsl #16
    orr r0, r0, r2, lsl #8
    orr r0, r1, r0
    add r1, r4, #0x04000000
    str r0, [r1, #0x400]
    ldr r0, [sp, #0x28]
    rsb r2, r0, #0x10000
    add r0, r1, #0x400
    strh r2, [r0, #8]
    ldr r2, [sp, #0x18]
    strh r2, [r0, #0x0A]
    ldr r0, [sp, #0x1C]
    str r0, [r1, #0x40C]
    str r8, [r1, #0x404]
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.size SND_SetupChannelPcm, . - SND_SetupChannelPcm
.L_original_pan_pcm:
    .word sOrgPan
.L_master_pan_pcm:
    .word sMasterPan
.L_original_volume_pcm:
    .word sOrgVolume
.L_surround_decay_pcm:
    .word sSurroundDecay
.L_surround_mask_pcm:
    .word 0x0000FFF5
