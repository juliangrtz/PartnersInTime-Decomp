/* Drain and execute the linked list of ARM9-to-ARM7 sound commands. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_CommandProc, "ax", %progbits
.balign 4

.extern OS_ReceiveMessage
.extern SND_InvalidateBank
.extern SND_InvalidateSeq
.extern SND_InvalidateWave
.extern SND_LockChannel
.extern SND_PauseSeq
.extern SND_PrepareSeq
.extern SND_SetMasterPan
.extern SND_SetMasterVolume
.extern SND_SetOutputSelector
.extern SND_SetPlayerGlobalVariable
.extern SND_SetPlayerLocalVariable
.extern SND_SetTrackAllocatableChannel
.extern SND_SetTrackMute
.extern SND_SetupAlarm
.extern SND_SetupCapture
.extern SND_SetupChannelNoise
.extern SND_SetupChannelPcm
.extern SND_SetupChannelPsg
.extern SND_SkipSeq
.extern SND_StartPreparedSeq
.extern SND_StartSeq
.extern SND_StopSeq
.extern SND_StopUnlockedChannel
.extern SND_UnlockChannel
.extern SNDi_CommandMesgQueue
.extern SNDi_ReadDriverInfo
.extern SNDi_SetChannelPan
.extern SNDi_SetChannelTimer
.extern SNDi_SetChannelVolume
.extern SNDi_SetPlayerParam
.extern SNDi_SetSurroundDecay
.extern SNDi_SetTrackParam
.extern SNDi_SharedWork
.extern SNDi_StartTimer
.extern SNDi_StopTimer

.global SND_CommandProc
.type SND_CommandProc, %function
SND_CommandProc:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #0x34
    ldr r7, .L_shared_work
    ldr r9, .L_command_queue
    add r8, sp, #0x18
    mov r4, #0
    ldr r6, .L_mask_16
    ldr r5, .L_mask_22
    b .L_receive_command_list
.L_have_command_list:
    ldr lr, [sp, #0x18]
    b .L_check_command
.L_dispatch_command:
    add r12, sp, #0x1c
    ldmia lr!, {r0, r1, r2, r3}
    stmia r12!, {r0, r1, r2, r3}
    ldmia lr, {r0, r1}
    stmia r12, {r0, r1}
    ldr r0, [sp, #0x20]
    cmp r0, #0x21
    addls pc, pc, r0, lsl #2
    b .L_command_complete
    b .L_cmd_start_seq
    b .L_cmd_stop_seq
    b .L_cmd_prepare_seq
    b .L_cmd_start_prepared_seq
    b .L_cmd_pause_seq
    b .L_cmd_skip_seq
    b .L_cmd_set_player_param
    b .L_cmd_set_track_param
    b .L_cmd_set_track_mute
    b .L_cmd_set_track_channels
    b .L_cmd_set_local_variable
    b .L_cmd_set_global_variable
    b .L_cmd_start_timer
    b .L_cmd_stop_timer
    b .L_cmd_setup_pcm
    b .L_cmd_setup_psg
    b .L_cmd_setup_noise
    b .L_cmd_setup_capture
    b .L_cmd_setup_alarm
    b .L_cmd_set_channel_timer
    b .L_cmd_set_channel_volume
    b .L_cmd_set_channel_pan
    b .L_cmd_set_surround_decay
    b .L_cmd_set_master_volume
    b .L_cmd_set_master_pan
    b .L_cmd_set_output_selector
    b .L_cmd_lock_channel
    b .L_cmd_unlock_channel
    b .L_cmd_stop_unlocked_channel
    b .L_cmd_publish_tag_pointer
    b .L_cmd_invalidate_seq
    b .L_cmd_invalidate_bank
    b .L_cmd_invalidate_wave
    b .L_cmd_read_driver_info
.L_cmd_start_seq:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    ldr r3, [sp, #0x30]
    bl SND_StartSeq
    b .L_command_complete
.L_cmd_stop_seq:
    ldr r0, [sp, #0x24]
    bl SND_StopSeq
    b .L_command_complete
.L_cmd_prepare_seq:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    ldr r3, [sp, #0x30]
    bl SND_PrepareSeq
    b .L_command_complete
.L_cmd_start_prepared_seq:
    ldr r0, [sp, #0x24]
    bl SND_StartPreparedSeq
    b .L_command_complete
.L_cmd_pause_seq:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SND_PauseSeq
    b .L_command_complete
.L_cmd_skip_seq:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SND_SkipSeq
    b .L_command_complete
.L_cmd_set_player_param:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    ldr r3, [sp, #0x30]
    bl SNDi_SetPlayerParam
    b .L_command_complete
.L_cmd_set_track_param:
    ldr r1, [sp, #0x24]
    lsr r0, r1, #0x18
    and r0, r0, #0xff
    str r0, [sp]
    bic r0, r1, #0xff000000
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    ldr r3, [sp, #0x30]
    bl SNDi_SetTrackParam
    b .L_command_complete
.L_cmd_set_track_mute:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    bl SND_SetTrackMute
    b .L_command_complete
.L_cmd_set_track_channels:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    bl SND_SetTrackAllocatableChannel
    b .L_command_complete
.L_cmd_set_local_variable:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    lsl r2, r2, #0x10
    asr r2, r2, #0x10
    bl SND_SetPlayerLocalVariable
    b .L_command_complete
.L_cmd_set_global_variable:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    lsl r1, r1, #0x10
    asr r1, r1, #0x10
    bl SND_SetPlayerGlobalVariable
    b .L_command_complete
.L_cmd_start_timer:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    ldr r3, [sp, #0x30]
    bl SNDi_StartTimer
    b .L_command_complete
.L_cmd_stop_timer:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    ldr r3, [sp, #0x30]
    bl SNDi_StopTimer
    b .L_command_complete
.L_cmd_setup_capture:
    ldr r1, [sp, #0x2c]
    lsr r0, r1, #0x1d
    and r0, r0, #1
    str r0, [sp]
    lsr r0, r1, #0x1c
    and r0, r0, #1
    str r0, [sp, #4]
    lsr r0, r1, #0x1b
    and r0, r0, #1
    str r0, [sp, #8]
    lsr r0, r1, #0x1f
    and r0, r0, #1
    lsr r1, r1, #0x1e
    and r1, r1, #1
    ldr r2, [sp, #0x24]
    ldr r3, [sp, #0x28]
    bl SND_SetupCapture
    b .L_command_complete
.L_cmd_setup_alarm:
    ldr r0, [sp, #0x30]
    str r0, [sp, #4]
    ldr r3, [sp, #0x2c]
    str r4, [sp]
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    mov r2, #0
    bl SND_SetupAlarm
    b .L_command_complete
.L_cmd_set_channel_timer:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SNDi_SetChannelTimer
    b .L_command_complete
.L_cmd_set_channel_volume:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    bl SNDi_SetChannelVolume
    b .L_command_complete
.L_cmd_set_channel_pan:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SNDi_SetChannelPan
    b .L_command_complete
.L_cmd_setup_pcm:
    ldr r3, [sp, #0x30]
    ldr r1, [sp, #0x24]
    ldr r0, [sp, #0x2c]
    and r2, r3, r6
    str r2, [sp]
    and r2, r0, r5
    str r2, [sp, #4]
    lsr r2, r0, #0x18
    and r2, r2, #0x7f
    str r2, [sp, #8]
    lsr r0, r0, #0x16
    and r0, r0, #3
    str r0, [sp, #0xc]
    and r0, r6, r1, lsr #0x10
    str r0, [sp, #0x10]
    lsr r0, r3, #0x10
    and r0, r0, #0x7f
    str r0, [sp, #0x14]
    and r0, r1, r6
    ldr r1, [sp, #0x28]
    bic r1, r1, #0xf8000000
    lsr r2, r3, #0x18
    and r2, r2, #3
    lsr r3, r3, #0x1a
    and r3, r3, #3
    bl SND_SetupChannelPcm
    b .L_command_complete
.L_cmd_setup_psg:
    ldr r1, [sp, #0x2c]
    ldr r3, [sp, #0x28]
    and r0, r6, r1, lsr #8
    str r0, [sp]
    and r0, r1, #0x7f
    str r0, [sp, #4]
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x30]
    and r2, r3, #0x7f
    lsr r3, r3, #8
    and r3, r3, #3
    bl SND_SetupChannelPsg
    b .L_command_complete
.L_cmd_setup_noise:
    ldr r3, [sp, #0x2c]
    ldr r2, [sp, #0x28]
    and r0, r3, #0x7f
    str r0, [sp]
    ldr r0, [sp, #0x24]
    and r1, r2, #0x7f
    lsr r2, r2, #8
    and r2, r2, #3
    and r3, r6, r3, lsr #8
    bl SND_SetupChannelNoise
    b .L_command_complete
.L_cmd_set_surround_decay:
    ldr r0, [sp, #0x24]
    bl SNDi_SetSurroundDecay
    b .L_command_complete
.L_cmd_set_master_volume:
    ldr r0, [sp, #0x24]
    bl SND_SetMasterVolume
    b .L_command_complete
.L_cmd_set_master_pan:
    ldr r0, [sp, #0x24]
    bl SND_SetMasterPan
    b .L_command_complete
.L_cmd_set_output_selector:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    ldr r2, [sp, #0x2c]
    ldr r3, [sp, #0x30]
    bl SND_SetOutputSelector
    b .L_command_complete
.L_cmd_lock_channel:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SND_LockChannel
    b .L_command_complete
.L_cmd_unlock_channel:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SND_UnlockChannel
    b .L_command_complete
.L_cmd_stop_unlocked_channel:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SND_StopUnlockedChannel
    b .L_command_complete
.L_cmd_invalidate_seq:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SND_InvalidateSeq
    b .L_command_complete
.L_cmd_invalidate_bank:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SND_InvalidateBank
    b .L_command_complete
.L_cmd_invalidate_wave:
    ldr r0, [sp, #0x24]
    ldr r1, [sp, #0x28]
    bl SND_InvalidateWave
    b .L_command_complete
.L_cmd_publish_tag_pointer:
    ldr r0, [sp, #0x24]
    str r0, [r7]
    b .L_command_complete
.L_cmd_read_driver_info:
    ldr r0, [sp, #0x24]
    bl SNDi_ReadDriverInfo
.L_command_complete:
    ldr lr, [sp, #0x1c]
.L_check_command:
    cmp lr, #0
    bne .L_dispatch_command
    ldr r1, [r7]
    ldr r0, [r1]
    add r0, r0, #1
    str r0, [r1]
.L_receive_command_list:
    mov r0, r9
    mov r1, r8
    mov r2, r4
    bl OS_ReceiveMessage
    cmp r0, #0
    bne .L_have_command_list
    add sp, sp, #0x34
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.L_shared_work:
    .word SNDi_SharedWork
.L_command_queue:
    .word SNDi_CommandMesgQueue
.L_mask_16:
    .word 0x0000ffff
.L_mask_22:
    .word 0x003fffff
.size SND_CommandProc, . - SND_CommandProc
