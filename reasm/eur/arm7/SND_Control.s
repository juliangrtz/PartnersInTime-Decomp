/* NitroSDK sound output, sleep, shutdown, and master-enable controls. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_Control, "ax", %progbits
.balign 4

.extern OS_SpinWait
.extern PMi_ResetControl
.extern PMi_SetControl
.extern REG_POWCNT2
.extern REG_SNDCAP0CNT
.extern REG_SNDCAP1CNT
.extern REG_SOUNDCNT_ENABLE
.extern REG_SOUNDCNT_VOLUME
.extern SND_StopChannel
.extern SVC_SoundBiasReset
.extern SVC_SoundBiasSet

.global SND_SetOutputSelector
.type SND_SetOutputSelector, %function
SND_SetOutputSelector:
    ldr r12, .L_sound_enable_output
    ldrb r12, [r12]
    ands r12, r12, #0x80
    movne r12, #1
    moveq r12, #0
    mov r12, r12, lsl #7
    orr r3, r12, r3, lsl #5
    orr r2, r3, r2, lsl #4
    orr r1, r2, r1, lsl #2
    orr r1, r0, r1
    ldr r0, .L_sound_enable_output
    strb r1, [r0]
    bx lr
.size SND_SetOutputSelector, . - SND_SetOutputSelector
.L_sound_enable_output:
    .word REG_SOUNDCNT_ENABLE

.global SND_SetMasterVolume
.type SND_SetMasterVolume, %function
SND_SetMasterVolume:
    ldr r1, .L_master_volume
    strb r0, [r1]
    bx lr
.size SND_SetMasterVolume, . - SND_SetMasterVolume
.L_master_volume:
    .word REG_SOUNDCNT_VOLUME

.global SND_EndSleep
.type SND_EndSleep, %function
SND_EndSleep:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_powcnt_end_sleep
    ldrh r0, [r1]
    orr r0, r0, #1
    strh r0, [r1]
    mov r0, #1
    bl PMi_SetControl
    mov r0, #0x100
    bl SND_SetSoundBias
    ldr r0, .L_end_sleep_delay
    bl OS_SpinWait
    bl SND_Enable
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SND_EndSleep, . - SND_EndSleep
.L_powcnt_end_sleep:
    .word REG_POWCNT2
.L_end_sleep_delay:
    .word 0x0007AB80

.global SND_SetSoundBias
.type SND_SetSoundBias, %function
SND_SetSoundBias:
    ldr r12, .L_sound_bias_set
    bx r12
.size SND_SetSoundBias, . - SND_SetSoundBias
.L_sound_bias_set:
    /* ELF marks the now-defined .thumb_func symbol odd as a callable pointer. */
    .word SVC_SoundBiasSet

.global SND_BeginSleep
.type SND_BeginSleep, %function
SND_BeginSleep:
    stmdb sp!, {lr}
    sub sp, sp, #4
    bl SND_Disable
    mov r0, #0x80
    bl SND_ResetSoundBias
    mov r0, #0x40000
    bl OS_SpinWait
    mov r0, #1
    bl PMi_ResetControl
    ldr r1, .L_powcnt_begin_sleep
    ldrh r0, [r1]
    bic r0, r0, #1
    strh r0, [r1]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SND_BeginSleep, . - SND_BeginSleep
.L_powcnt_begin_sleep:
    .word REG_POWCNT2

.global SND_ResetSoundBias
.type SND_ResetSoundBias, %function
SND_ResetSoundBias:
    ldr r12, .L_sound_bias_reset
    bx r12
.size SND_ResetSoundBias, . - SND_ResetSoundBias
.L_sound_bias_reset:
    /* ELF marks the now-defined .thumb_func symbol odd as a callable pointer. */
    .word SVC_SoundBiasReset

.global SND_Shutdown
.type SND_Shutdown, %function
SND_Shutdown:
    push {r4, r5, lr}
    sub sp, sp, #4
    bl SND_Disable
    mov r5, #0
    mov r4, #1
.L_shutdown_channels:
    mov r0, r5
    mov r1, r4
    bl SND_StopChannel
    add r5, r5, #1
    cmp r5, #0x10
    blt .L_shutdown_channels
    mov r1, #0
    ldr r0, .L_capture0_shutdown
    strb r1, [r0]
    ldr r0, .L_capture1_shutdown
    strb r1, [r0]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SND_Shutdown, . - SND_Shutdown
.L_capture0_shutdown:
    .word REG_SNDCAP0CNT
.L_capture1_shutdown:
    .word REG_SNDCAP1CNT

.global SND_Disable
.type SND_Disable, %function
SND_Disable:
    ldr r1, .L_sound_enable_disable
    ldrb r0, [r1]
    bic r0, r0, #0x80
    strb r0, [r1]
    bx lr
.size SND_Disable, . - SND_Disable
.L_sound_enable_disable:
    .word REG_SOUNDCNT_ENABLE

.global SND_Enable
.type SND_Enable, %function
SND_Enable:
    ldr r1, .L_sound_enable_enable
    ldrb r0, [r1]
    orr r0, r0, #0x80
    strb r0, [r1]
    bx lr
.size SND_Enable, . - SND_Enable
.L_sound_enable_enable:
    .word REG_SOUNDCNT_ENABLE
