/* Commit staged extended-channel state to the DS sound registers. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_UpdateExChannel, "ax", %progbits
.balign 4

.extern SND_SetChannelPan
.extern SND_SetChannelTimer
.extern SND_SetChannelVolume
.extern SND_SetupChannelNoise
.extern SND_SetupChannelPcm
.extern SND_SetupChannelPsg
.extern SND_StopChannel
.extern SNDi_Work
.extern sLockedChannelMask
.extern sWeakLockedChannelMask

.global SND_UpdateExChannel
.type SND_UpdateExChannel, %function
SND_UpdateExChannel:
    push {r4, r5, r6, r7, r8, r9, r10, lr}
    sub sp, sp, #0x18
    mov r8, #0
    mov r4, #2
    mov r5, #1
    mov r6, r8
    ldr r7, .L_update_sound_work
    mov r10, #0x54
.L_update_channel_loop:
    mla r9, r8, r10, r7
    ldrb r0, [r9, #3]
    mov r0, r0, lsl #24
    movs r0, r0, lsr #27
    beq .L_update_channel_next
    ands r0, r0, #2
    beq .L_check_channel_start
    mov r0, r8
    mov r1, r6
    bl SND_StopChannel
.L_check_channel_start:
    ldrb r0, [r9, #3]
    mov r0, r0, lsl #24
    mov r1, r0, lsr #27
    ands r0, r1, #1
    beq .L_update_channel_parameters
    ldrb r0, [r9, #1]
    cmp r0, #0
    beq .L_start_pcm_channel
    cmp r0, #1
    beq .L_start_psg_channel
    cmp r0, #2
    beq .L_start_noise_channel
    b .L_update_channel_next
.L_start_pcm_channel:
    ldrb r0, [r9, #0x39]
    cmp r0, #0
    movne r3, r5
    moveq r3, r4
    ldrh r1, [r9, #0x24]
    ldrh r0, [r9, #0x3E]
    str r0, [sp]
    ldr r0, [r9, #0x40]
    str r0, [sp, #4]
    and r0, r1, #0xFF
    str r0, [sp, #8]
    mov r0, r1, asr #8
    str r0, [sp, #0x0C]
    ldrh r0, [r9, #0x26]
    str r0, [sp, #0x10]
    ldrb r0, [r9, #0x23]
    str r0, [sp, #0x14]
    mov r0, r8
    ldr r1, [r9, #0x44]
    ldrb r2, [r9, #0x38]
    bl SND_SetupChannelPcm
    b .L_update_channel_next
.L_start_psg_channel:
    ldrh r3, [r9, #0x24]
    ldrh r0, [r9, #0x26]
    str r0, [sp]
    ldrb r0, [r9, #0x23]
    str r0, [sp, #4]
    mov r0, r8
    ldr r1, [r9, #0x44]
    and r2, r3, #0xFF
    mov r3, r3, asr #8
    bl SND_SetupChannelPsg
    b .L_update_channel_next
.L_start_noise_channel:
    ldrh r2, [r9, #0x24]
    ldrb r0, [r9, #0x23]
    str r0, [sp]
    mov r0, r8
    and r1, r2, #0xFF
    mov r2, r2, asr #8
    ldrh r3, [r9, #0x26]
    bl SND_SetupChannelNoise
    b .L_update_channel_next
.L_update_channel_parameters:
    ands r0, r1, #4
    beq .L_update_channel_volume
    mov r0, r8
    ldrh r1, [r9, #0x26]
    bl SND_SetChannelTimer
.L_update_channel_volume:
    ldrb r0, [r9, #3]
    mov r0, r0, lsl #24
    mov r0, r0, lsr #27
    ands r0, r0, #8
    beq .L_update_channel_pan
    ldrh r2, [r9, #0x24]
    mov r0, r8
    and r1, r2, #0xFF
    mov r2, r2, asr #8
    bl SND_SetChannelVolume
.L_update_channel_pan:
    ldrb r0, [r9, #3]
    mov r0, r0, lsl #24
    mov r0, r0, lsr #27
    ands r0, r0, #0x10
    beq .L_update_channel_next
    mov r0, r8
    ldrb r1, [r9, #0x23]
    bl SND_SetChannelPan
.L_update_channel_next:
    add r8, r8, #1
    cmp r8, #0x10
    blt .L_update_channel_loop
    mov r5, #0
    ldr r3, .L_update_sound_work
    mov r1, #0x54
.L_start_staged_channels:
    mla r4, r5, r1, r3
    ldrb r0, [r4, #3]
    mov r0, r0, lsl #24
    movs r0, r0, lsr #27
    beq .L_start_staged_next
    ands r0, r0, #1
    movne r0, r5, lsl #4
    addne r0, r0, #0x04000000
    ldrbne r2, [r0, #0x403]
    orrne r2, r2, #0x80
    strbne r2, [r0, #0x403]
    ldrb r0, [r4, #3]
    bic r0, r0, #0xF8
    strb r0, [r4, #3]
.L_start_staged_next:
    add r5, r5, #1
    cmp r5, #0x10
    blt .L_start_staged_channels
    add sp, sp, #0x18
    pop {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
.size SND_UpdateExChannel, . - SND_UpdateExChannel
.L_update_sound_work:
    .word SNDi_Work

.global SND_ExChannelInit
.type SND_ExChannelInit, %function
SND_ExChannelInit:
    mov r12, #0
    ldr r2, .L_init_sound_work
    mov r0, #0x54
.L_init_channel_loop:
    mul r1, r12, r0
    add r3, r2, r1
    strb r12, [r2, r1]
    ldrb r1, [r3, #3]
    bic r1, r1, #0xF8
    strb r1, [r3, #3]
    ldrb r1, [r3, #3]
    bic r1, r1, #1
    strb r1, [r3, #3]
    add r12, r12, #1
    cmp r12, #0x10
    blt .L_init_channel_loop
    mov r1, #0
    ldr r0, .L_init_weak_locked_mask
    str r1, [r0]
    ldr r0, .L_init_locked_mask
    str r1, [r0]
    bx lr
.size SND_ExChannelInit, . - SND_ExChannelInit
.L_init_sound_work:
    .word SNDi_Work
.L_init_weak_locked_mask:
    .word sWeakLockedChannelMask
.L_init_locked_mask:
    .word sLockedChannelMask
