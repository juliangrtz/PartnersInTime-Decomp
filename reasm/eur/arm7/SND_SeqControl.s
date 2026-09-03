/* Public sequence-player controls and generic track/player parameter setters. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_SeqControl, "ax", %progbits
.balign 4

.extern SND_PrepareSeq
.extern SND_StartIntervalTimer
.extern SND_StopIntervalTimer
.extern SNDi_PlayerPool
.extern SNDi_SharedWork

.global SNDi_SetTrackParam
.type SNDi_SetTrackParam, %function
SNDi_SetTrackParam:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r10, r1
    mov r9, r2
    mov r8, r3
    ldr r7, [sp, #0x28]
    ldr r2, .L_player_pool_track_param
    mov r1, #0x24
    mla r11, r0, r1, r2
    mov r6, #0
    mov r0, r8, lsl #16
    mov r4, r0, lsr #16
    and r5, r8, #0xFF
    b .L_track_param_test
.L_track_param_loop:
    ands r0, r10, #1
    beq .L_track_param_next
    mov r0, r11
    mov r1, r6
    bl SNDi_GetPlayerTrack
    cmp r0, #0
    beq .L_track_param_next
    cmp r7, #1
    beq .L_store_track_byte
    cmp r7, #2
    beq .L_store_track_half
    cmp r7, #4
    streq r8, [r0, r9]
    b .L_track_param_next
.L_store_track_byte:
    strb r5, [r0, r9]
    b .L_track_param_next
.L_store_track_half:
    strh r4, [r0, r9]
.L_track_param_next:
    add r6, r6, #1
    mov r10, r10, lsr #1
.L_track_param_test:
    cmp r6, #0x10
    bge .L_track_param_done
    cmp r10, #0
    bne .L_track_param_loop
.L_track_param_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.L_player_pool_track_param:
    .word SNDi_PlayerPool
.size SNDi_SetTrackParam, . - SNDi_SetTrackParam

.global SNDi_SetPlayerParam
.type SNDi_SetPlayerParam, %function
SNDi_SetPlayerParam:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr lr, .L_player_pool_player_param
    mov r12, #0x24
    mla r12, r0, r12, lr
    cmp r3, #1
    beq .L_store_player_byte
    cmp r3, #2
    beq .L_store_player_half
    cmp r3, #4
    streq r2, [r12, r1]
    b .L_player_param_done
.L_store_player_byte:
    strb r2, [r12, r1]
    b .L_player_param_done
.L_store_player_half:
    strh r2, [r12, r1]
.L_player_param_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_player_pool_player_param:
    .word SNDi_PlayerPool
.size SNDi_SetPlayerParam, . - SNDi_SetPlayerParam

.global SND_InvalidateBank
.type SND_InvalidateBank, %function
SND_InvalidateBank:
    push {r4, r5, r6, r7, r8, lr}
    mov r7, r0
    mov r6, r1
    mov r5, #0
    ldr r4, .L_player_pool_bank
    mov r8, #0x24
.L_invalidate_bank_loop:
    mul r1, r5, r8
    add r0, r4, r1
    ldrb r1, [r4, r1]
    mov r1, r1, lsl #31
    movs r1, r1, lsr #31
    beq .L_invalidate_bank_next
    ldr r1, [r0, #0x20]
    cmp r7, r1
    bhi .L_invalidate_bank_next
    cmp r1, r6
    bhi .L_invalidate_bank_next
    bl SNDi_FinishPlayer
.L_invalidate_bank_next:
    add r5, r5, #1
    cmp r5, #0x10
    blt .L_invalidate_bank_loop
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.L_player_pool_bank:
    .word SNDi_PlayerPool
.size SND_InvalidateBank, . - SND_InvalidateBank

.global SND_InvalidateSeq
.type SND_InvalidateSeq, %function
SND_InvalidateSeq:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r10, r0
    mov r9, r1
    mov r7, #0
    mov r5, r7
    ldr r11, .L_player_pool_seq
    mov r4, #0x24
.L_invalidate_seq_player_loop:
    mul r0, r7, r4
    add r8, r11, r0
    ldrb r0, [r11, r0]
    mov r0, r0, lsl #31
    movs r0, r0, lsr #31
    beq .L_invalidate_seq_player_next
    mov r6, r5
    b .L_invalidate_seq_track_test
.L_invalidate_seq_track_loop:
    mov r0, r8
    mov r1, r6
    bl SNDi_GetPlayerTrack
    cmp r0, #0
    beq .L_invalidate_seq_track_next
    ldr r0, [r0, #0x28]
    cmp r10, r0
    bhi .L_invalidate_seq_track_next
    cmp r0, r9
    bhi .L_invalidate_seq_track_next
    mov r0, r8
    bl SNDi_FinishPlayer
    b .L_invalidate_seq_player_next
.L_invalidate_seq_track_next:
    add r6, r6, #1
.L_invalidate_seq_track_test:
    cmp r6, #0x10
    blt .L_invalidate_seq_track_loop
.L_invalidate_seq_player_next:
    add r7, r7, #1
    cmp r7, #0x10
    blt .L_invalidate_seq_player_loop
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.L_player_pool_seq:
    .word SNDi_PlayerPool
.size SND_InvalidateSeq, . - SND_InvalidateSeq

.global SND_SetTrackAllocatableChannel
.type SND_SetTrackAllocatableChannel, %function
SND_SetTrackAllocatableChannel:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r6, r1
    ldr r3, .L_player_pool_allocatable
    mov r1, #0x24
    mla r5, r0, r1, r3
    mov r4, #0
    mov r0, r2, lsl #16
    mov r7, r0, lsr #16
    b .L_allocatable_test
.L_allocatable_loop:
    ands r0, r6, #1
    beq .L_allocatable_next
    mov r0, r5
    mov r1, r4
    bl SNDi_GetPlayerTrack
    cmp r0, #0
    strhne r7, [r0, #0x1E]
    ldrbne r1, [r0]
    orrne r1, r1, #0x80
    strbne r1, [r0]
.L_allocatable_next:
    add r4, r4, #1
    mov r6, r6, lsr #1
.L_allocatable_test:
    cmp r4, #0x10
    bge .L_allocatable_done
    cmp r6, #0
    bne .L_allocatable_loop
.L_allocatable_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_player_pool_allocatable:
    .word SNDi_PlayerPool
.size SND_SetTrackAllocatableChannel, . - SND_SetTrackAllocatableChannel

.global SND_SetTrackMute
.type SND_SetTrackMute, %function
SND_SetTrackMute:
    push {r4, r5, r6, r7, r8, r9, r10, lr}
    mov r9, r1
    mov r8, r2
    ldr r2, .L_player_pool_mute
    mov r1, #0x24
    mla r7, r0, r1, r2
    mov r5, #0
    mov r4, #0x7F
    and r0, r8, #0xFF
    and r0, r0, #1
    mov r10, r0, lsl #2
    b .L_mute_test
.L_mute_loop:
    ands r0, r9, #1
    beq .L_mute_next
    mov r0, r7
    mov r1, r5
    bl SNDi_GetPlayerTrack
    movs r6, r0
    beq .L_mute_next
    cmp r8, #0
    beq .L_set_mute_flag
    mov r1, r7
    mov r2, r4
    bl SNDi_ReleaseTrackChannelAll
    mov r0, r6
    bl SNDi_FreeTrackChannelAll
.L_set_mute_flag:
    ldrb r0, [r6]
    bic r0, r0, #4
    orr r0, r0, r10
    strb r0, [r6]
.L_mute_next:
    add r5, r5, #1
    mov r9, r9, lsr #1
.L_mute_test:
    cmp r5, #0x10
    bge .L_mute_done
    cmp r9, #0
    bne .L_mute_loop
.L_mute_done:
    pop {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
.L_player_pool_mute:
    .word SNDi_PlayerPool
.size SND_SetTrackMute, . - SND_SetTrackMute

.global SND_SkipSeq
.type SND_SkipSeq, %function
SND_SkipSeq:
    push {r4, r5, r6, r7, r8, lr}
    mov r6, r1
    ldr r2, .L_player_pool_skip
    mov r1, #0x24
    mla r5, r0, r1, r2
    mov r8, #0
    mov r4, #0x7F
.L_skip_release_loop:
    mov r0, r5
    mov r1, r8
    bl SNDi_GetPlayerTrack
    movs r7, r0
    beq .L_skip_release_next
    mov r1, r5
    mov r2, r4
    bl SNDi_ReleaseTrackChannelAll
    mov r0, r7
    bl SNDi_FreeTrackChannelAll
.L_skip_release_next:
    add r8, r8, #1
    cmp r8, #0x10
    blt .L_skip_release_loop
    bl SND_StopIntervalTimer
    mov r4, #0
    mov r7, r4
    b .L_skip_tick_test
.L_skip_tick_loop:
    mov r0, r5
    mov r1, r7
    bl SNDi_PlayerSeqMain
    cmp r0, #0
    beq .L_skip_tick_next
    mov r0, r5
    bl SNDi_FinishPlayer
    b .L_skip_tick_done
.L_skip_tick_next:
    add r4, r4, #1
.L_skip_tick_test:
    cmp r4, r6
    blo .L_skip_tick_loop
.L_skip_tick_done:
    bl SND_StartIntervalTimer
    ldr r0, .L_shared_work_skip
    ldr r0, [r0]
    cmp r0, #0
    beq .L_skip_done
    add r3, r0, #0x40
    ldrb r1, [r5, #1]
    mov r0, #0x24
    mul r2, r1, r0
    ldr r0, [r3, r2]
    add r0, r0, r4
    str r0, [r3, r2]
.L_skip_done:
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.L_player_pool_skip:
    .word SNDi_PlayerPool
.L_shared_work_skip:
    .word SNDi_SharedWork
.size SND_SkipSeq, . - SND_SkipSeq

.global SND_PauseSeq
.type SND_PauseSeq, %function
SND_PauseSeq:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    ldr r5, .L_player_pool_pause
    mov r2, #0x24
    mul r3, r0, r2
    add r4, r5, r3
    ldrb r0, [r5, r3]
    bic r2, r0, #4
    and r0, r1, #0xFF
    and r0, r0, #1
    orr r0, r2, r0, lsl #2
    strb r0, [r5, r3]
    cmp r1, #0
    beq .L_pause_done
    mov r7, #0
    mov r5, #0x7F
.L_pause_track_loop:
    mov r0, r4
    mov r1, r7
    bl SNDi_GetPlayerTrack
    movs r6, r0
    beq .L_pause_track_next
    mov r1, r4
    mov r2, r5
    bl SNDi_ReleaseTrackChannelAll
    mov r0, r6
    bl SNDi_FreeTrackChannelAll
.L_pause_track_next:
    add r7, r7, #1
    cmp r7, #0x10
    blt .L_pause_track_loop
.L_pause_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_player_pool_pause:
    .word SNDi_PlayerPool
.size SND_PauseSeq, . - SND_PauseSeq

.global SND_StopSeq
.type SND_StopSeq, %function
SND_StopSeq:
    push {r4, lr}
    mov r4, r0
    ldr r2, .L_player_pool_stop
    mov r0, #0x24
    mul r1, r4, r0
    add r0, r2, r1
    ldrb r1, [r2, r1]
    mov r1, r1, lsl #31
    movs r1, r1, lsr #31
    beq .L_stop_done
    bl SNDi_FinishPlayer
    ldr r0, .L_shared_work_stop
    ldr r2, [r0]
    cmp r2, #0
    ldrne r1, [r2, #4]
    movne r0, #1
    mvnne r0, r0, lsl r4
    andne r0, r1, r0
    strne r0, [r2, #4]
.L_stop_done:
    pop {r4, lr}
    bx lr
.L_player_pool_stop:
    .word SNDi_PlayerPool
.L_shared_work_stop:
    .word SNDi_SharedWork
.size SND_StopSeq, . - SND_StopSeq

.global SND_StartSeq
.type SND_StartSeq, %function
SND_StartSeq:
    push {r4, lr}
    mov r4, r0
    bl SND_PrepareSeq
    mov r0, r4
    bl SND_StartPreparedSeq
    pop {r4, lr}
    bx lr
.size SND_StartSeq, . - SND_StartSeq

.global SND_StartPreparedSeq
.type SND_StartPreparedSeq, %function
SND_StartPreparedSeq:
    ldr r2, .L_player_pool_start
    mov r1, #0x24
    mul r1, r0, r1
    ldrb r0, [r2, r1]
    orr r0, r0, #2
    strb r0, [r2, r1]
    bx lr
.L_player_pool_start:
    .word SNDi_PlayerPool
.size SND_StartPreparedSeq, . - SND_StartPreparedSeq
