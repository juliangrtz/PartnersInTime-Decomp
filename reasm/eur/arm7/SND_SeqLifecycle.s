/* Maintain sequence-player tracks and their linked extended sound channels. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_SeqLifecycle, "ax", %progbits
.balign 4

.extern SND_FreeExChannel
.extern SND_IsExChannelActive
.extern SND_ReleaseExChannel
.extern SND_SetExChannelRelease
.extern SNDi_DecibelSquareTable
.extern SNDi_SharedWork
.extern SNDi_TrackPool

.global SNDi_UpdatePlayerChannel
.type SNDi_UpdatePlayerChannel, %function
SNDi_UpdatePlayerChannel:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, #0
    mov r4, #1
.L_update_player_track_loop:
    mov r0, r6
    mov r1, r5
    bl SNDi_GetPlayerTrack
    cmp r0, #0
    beq .L_update_player_track_next
    mov r1, r6
    mov r2, r4
    bl SNDi_UpdateTrackChannel
.L_update_player_track_next:
    add r5, r5, #1
    cmp r5, #0x10
    blt .L_update_player_track_loop
    pop {r4, r5, r6, lr}
    bx lr
.size SNDi_UpdatePlayerChannel, . - SNDi_UpdatePlayerChannel

.global SNDi_UpdateTrackChannel
.type SNDi_UpdateTrackChannel, %function
SNDi_UpdateTrackChannel:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r10, r0
    mov r11, r2
    ldrb r0, [r1, #5]
    mov r0, r0, lsl #1
    ldr r4, .L_decibel_square_table
    ldrsh r3, [r4, r0]
    ldrb r0, [r10, #4]
    mov r0, r0, lsl #1
    ldrsh r2, [r4, r0]
    ldrb r0, [r10, #5]
    mov r0, r0, lsl #1
    ldrsh r0, [r4, r0]
    add r0, r2, r0
    add r3, r3, r0
    ldrsh r2, [r10, #0xA]
    ldrsh r0, [r1, #6]
    add r2, r2, r0
    ldrsb r4, [r10, #6]
    ldrb r0, [r10, #7]
    mov r0, r0, lsl #6
    mul r1, r4, r0
    ldrsh r0, [r10, #0xC]
    add r0, r0, r1, asr #7
    ldrsb r1, [r10, #8]
    ldrb r4, [r10, #1]
    cmp r4, #0x7F
    mulne r4, r1, r4
    addne r1, r4, #0x40
    movne r1, r1, asr #7
    ldrsb r4, [r10, #9]
    add r1, r1, r4
    mvn r4, #0x7F
    cmp r1, r4
    movlt r1, r4
    blt .L_channel_values_ready
    cmp r1, #0x7F
    movgt r1, #0x7F
.L_channel_values_ready:
    ldr r9, [r10, #0x3C]
    mov r4, #1
    mov r3, r3, lsl #16
    mov r7, r3, asr #16
    mov r0, r0, lsl #16
    mov r6, r0, asr #16
    mov r0, r1, lsl #24
    mov r5, r0, asr #24
    mov r0, r2, lsl #16
    mov r8, r0, asr #16
    b .L_update_link_test
.L_update_link_loop:
    strh r8, [r9, #6]
    ldrb r0, [r9, #2]
    cmp r0, #3
    beq .L_update_link_next
    strh r7, [r9, #0xC]
    strh r6, [r9, #0xE]
    strb r5, [r9, #0xB]
    ldrb r0, [r10, #1]
    strb r0, [r9, #4]
    ldrh r0, [r10, #0x18]
    strh r0, [r9, #0x28]
    ldrh r0, [r10, #0x1A]
    strh r0, [r9, #0x2A]
    ldrh r0, [r10, #0x1C]
    strh r0, [r9, #0x2C]
    ldr r0, [r9, #0x34]
    cmp r0, #0
    bne .L_update_link_next
    cmp r11, #0
    beq .L_update_link_next
    strb r4, [r9, #0x22]
    mov r0, r9
    bl SND_ReleaseExChannel
.L_update_link_next:
    ldr r9, [r9, #0x50]
.L_update_link_test:
    cmp r9, #0
    bne .L_update_link_loop
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.L_decibel_square_table:
    .word SNDi_DecibelSquareTable
.size SNDi_UpdateTrackChannel, . - SNDi_UpdateTrackChannel

.global SNDi_ChannelCallback
.type SNDi_ChannelCallback, %function
SNDi_ChannelCallback:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, r2
    cmp r1, #1
    bne .L_unlink_callback_channel
    mov r1, #0
    strb r1, [r5, #0x22]
    bl SND_FreeExChannel
.L_unlink_callback_channel:
    ldr r1, [r4, #0x3C]
    cmp r1, r5
    ldreq r0, [r5, #0x50]
    streq r0, [r4, #0x3C]
    beq .L_callback_done
    b .L_callback_scan_test
.L_callback_scan_loop:
    cmp r0, r5
    ldreq r0, [r5, #0x50]
    streq r0, [r1, #0x50]
    beq .L_callback_done
    mov r1, r0
.L_callback_scan_test:
    ldr r0, [r1, #0x50]
    cmp r0, #0
    bne .L_callback_scan_loop
.L_callback_done:
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SNDi_ChannelCallback, . - SNDi_ChannelCallback

.global SNDi_FinishPlayer
.type SNDi_FinishPlayer, %function
SNDi_FinishPlayer:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, #0
.L_finish_track_loop:
    mov r0, r5
    mov r1, r4
    bl SNDi_ClosePlayerTrack
    add r4, r4, #1
    cmp r4, #0x10
    blt .L_finish_track_loop
    ldrb r0, [r5]
    bic r0, r0, #1
    strb r0, [r5]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SNDi_FinishPlayer, . - SNDi_FinishPlayer

.global SNDi_ClosePlayerTrack
.type SNDi_ClosePlayerTrack, %function
SNDi_ClosePlayerTrack:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, r1
    bl SNDi_GetPlayerTrack
    cmp r0, #0
    beq .L_close_player_track_done
    mov r1, r5
    bl SNDi_CloseTrack
    add r3, r5, #8
    ldr r2, .L_track_pool_close
    ldrb r1, [r3, r4]
    ldrb r0, [r2, r1, lsl #6]
    bic r0, r0, #1
    strb r0, [r2, r1, lsl #6]
    mov r0, #0xFF
    strb r0, [r3, r4]
.L_close_player_track_done:
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_track_pool_close:
    .word SNDi_TrackPool
.size SNDi_ClosePlayerTrack, . - SNDi_ClosePlayerTrack

.global SNDi_CloseTrack
.type SNDi_CloseTrack, %function
SNDi_CloseTrack:
    push {r4, lr}
    mov r4, r0
    mvn r2, #0
    bl SNDi_ReleaseTrackChannelAll
    mov r0, r4
    bl SNDi_FreeTrackChannelAll
    pop {r4, lr}
    bx lr
.size SNDi_CloseTrack, . - SNDi_CloseTrack

.global SNDi_GetPlayerTrack
.type SNDi_GetPlayerTrack, %function
SNDi_GetPlayerTrack:
    cmp r1, #0xF
    movgt r0, #0
    bxgt lr
    add r0, r0, r1
    ldrb r1, [r0, #8]
    cmp r1, #0xFF
    moveq r0, #0
    ldrne r0, .L_track_pool_get
    addne r0, r0, r1, lsl #6
    bx lr
.L_track_pool_get:
    .word SNDi_TrackPool
.size SNDi_GetPlayerTrack, . - SNDi_GetPlayerTrack

.global SNDi_PlayerTempoMain
.type SNDi_PlayerTempoMain, %function
SNDi_PlayerTempoMain:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r6, r0
    mov r5, #0
    add r1, r6, #0x1C
    b .L_tempo_tick_test
.L_tempo_tick_loop:
    ldrh r0, [r1]
    sub r0, r0, #0xF0
    strh r0, [r1]
    add r5, r5, #1
.L_tempo_tick_test:
    ldrh r0, [r6, #0x1C]
    cmp r0, #0xF0
    bhs .L_tempo_tick_loop
    mov r4, #0
    mov r7, #1
    b .L_tempo_sequence_test
.L_tempo_sequence_loop:
    mov r0, r6
    mov r1, r7
    bl SNDi_PlayerSeqMain
    cmp r0, #0
    beq .L_tempo_sequence_next
    mov r0, r6
    bl SNDi_FinishPlayer
    b .L_tempo_sequence_done
.L_tempo_sequence_next:
    add r4, r4, #1
.L_tempo_sequence_test:
    cmp r4, r5
    blt .L_tempo_sequence_loop
.L_tempo_sequence_done:
    ldr r0, .L_shared_work_tempo
    ldr r0, [r0]
    cmp r0, #0
    beq .L_update_tempo_counter
    add r3, r0, #0x40
    ldrb r1, [r6, #1]
    mov r0, #0x24
    mul r2, r1, r0
    ldr r0, [r3, r2]
    add r0, r0, r4
    str r0, [r3, r2]
.L_update_tempo_counter:
    ldrh r2, [r6, #0x18]
    ldrh r0, [r6, #0x1A]
    mul r1, r2, r0
    ldrh r0, [r6, #0x1C]
    add r0, r0, r1, asr #8
    strh r0, [r6, #0x1C]
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_shared_work_tempo:
    .word SNDi_SharedWork
.size SNDi_PlayerTempoMain, . - SNDi_PlayerTempoMain

.global SNDi_FreeTrackChannelAll
.type SNDi_FreeTrackChannelAll, %function
SNDi_FreeTrackChannelAll:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    ldr r4, [r5, #0x3C]
    b .L_free_track_channel_test
.L_free_track_channel_loop:
    mov r0, r4
    bl SND_FreeExChannel
    ldr r4, [r4, #0x50]
.L_free_track_channel_test:
    cmp r4, #0
    bne .L_free_track_channel_loop
    mov r0, #0
    str r0, [r5, #0x3C]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SNDi_FreeTrackChannelAll, . - SNDi_FreeTrackChannelAll

.global SNDi_ReleaseTrackChannelAll
.type SNDi_ReleaseTrackChannelAll, %function
SNDi_ReleaseTrackChannelAll:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r4, r0
    mov r7, r2
    mov r2, #0
    bl SNDi_UpdateTrackChannel
    ldr r6, [r4, #0x3C]
    and r5, r7, #0xFF
    mov r4, #1
    b .L_release_track_channel_test
.L_release_track_channel_loop:
    mov r0, r6
    bl SND_IsExChannelActive
    cmp r0, #0
    beq .L_release_track_channel_next
    cmp r7, #0
    blt .L_release_track_channel
    mov r0, r6
    mov r1, r5
    bl SND_SetExChannelRelease
.L_release_track_channel:
    strb r4, [r6, #0x22]
    mov r0, r6
    bl SND_ReleaseExChannel
.L_release_track_channel_next:
    ldr r6, [r6, #0x50]
.L_release_track_channel_test:
    cmp r6, #0
    bne .L_release_track_channel_loop
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size SNDi_ReleaseTrackChannelAll, . - SNDi_ReleaseTrackChannelAll
