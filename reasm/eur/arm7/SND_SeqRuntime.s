/* Advance and initialize sequence players, plus bank wave-archive lookup. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_SeqRuntime, "ax", %progbits
.balign 4

.extern SND_GetWaveDataAddress
.extern SNDi_PlayerPool
.extern SNDi_SharedWork
.extern SNDi_TrackPool

.global SND_SeqMain
.type SND_SeqMain, %function
SND_SeqMain:
    push {r4, r5, r6, r7, r8, r9, r10, lr}
    mov r8, r0
    mov r5, #0
    mov r6, r5
    mov r10, #1
    ldr r4, .L_player_pool_main
    mov r9, #0x24
.L_seq_player_loop:
    mul r0, r6, r9
    add r7, r4, r0
    ldrb r1, [r4, r0]
    mov r0, r1, lsl #31
    movs r0, r0, lsr #31
    beq .L_seq_player_next
    mov r0, r1, lsl #30
    movs r0, r0, lsr #31
    beq .L_record_active_player
    cmp r8, #0
    beq .L_update_player_channels
    mov r0, r1, lsl #29
    movs r0, r0, lsr #31
    bne .L_update_player_channels
    mov r0, r7
    bl SNDi_PlayerTempoMain
.L_update_player_channels:
    mov r0, r7
    bl SNDi_UpdatePlayerChannel
.L_record_active_player:
    ldrb r0, [r7]
    mov r0, r0, lsl #31
    movs r0, r0, lsr #31
    orrne r5, r5, r10, lsl r6
.L_seq_player_next:
    add r6, r6, #1
    cmp r6, #0x10
    blt .L_seq_player_loop
    ldr r0, .L_shared_work_main
    ldr r0, [r0]
    cmp r0, #0
    strne r5, [r0, #4]
    pop {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
.L_player_pool_main:
    .word SNDi_PlayerPool
.L_shared_work_main:
    .word SNDi_SharedWork
.size SND_SeqMain, . - SND_SeqMain

.global SND_SeqInit
.type SND_SeqInit, %function
SND_SeqInit:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov lr, #0
    ldr r3, .L_player_pool_init
    mov r0, #0x24
.L_init_seq_player_loop:
    mul r2, lr, r0
    add r12, r3, r2
    ldrb r1, [r3, r2]
    bic r1, r1, #1
    strb r1, [r3, r2]
    strb lr, [r12, #1]
    add lr, lr, #1
    cmp lr, #0x10
    blt .L_init_seq_player_loop
    mov r2, #0
    ldr r1, .L_track_pool_init
.L_init_seq_track_loop:
    ldrb r0, [r1, r2, lsl #6]
    bic r0, r0, #1
    strb r0, [r1, r2, lsl #6]
    add r2, r2, #1
    cmp r2, #0x20
    blt .L_init_seq_track_loop
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_player_pool_init:
    .word SNDi_PlayerPool
.L_track_pool_init:
    .word SNDi_TrackPool
.size SND_SeqInit, . - SND_SeqInit

.global SNDi_GetWaveData
.type SNDi_GetWaveData, %function
SNDi_GetWaveData:
    stmdb sp!, {lr}
    sub sp, sp, #4
    add r0, r0, r1, lsl #3
    ldr r0, [r0, #0x18]
    cmp r0, #0
    moveq r0, #0
    beq .L_get_wave_done
    ldr r1, [r0, #0x38]
    cmp r2, r1
    movhs r0, #0
    bhs .L_get_wave_done
    mov r1, r2
    bl SND_GetWaveDataAddress
.L_get_wave_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SNDi_GetWaveData, . - SNDi_GetWaveData
