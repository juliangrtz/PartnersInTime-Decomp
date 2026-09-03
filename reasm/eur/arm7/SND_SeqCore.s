/* Core track allocation, variable lookup, and per-player sequence dispatch. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_SeqCore, "ax", %progbits
.balign 4

.extern SNDi_ClosePlayerTrack
.extern SNDi_SharedWork
.extern SNDi_TrackPool
.extern SNDi_TrackSeqMain
.extern SNDi_GetPlayerTrack

.global SNDi_AllocTrack
.type SNDi_AllocTrack, %function
SNDi_AllocTrack:
    mov r0, #0
    ldr r3, .L_track_pool
    b .L_alloc_track_test
.L_alloc_track_loop:
    add r12, r3, r0, lsl #6
    ldrb r1, [r3, r0, lsl #6]
    mov r2, r1, lsl #31
    movs r2, r2, lsr #31
    biceq r1, r1, #1
    orreq r1, r1, #1
    strbeq r1, [r12]
    bxeq lr
    add r0, r0, #1
.L_alloc_track_test:
    cmp r0, #0x20
    blt .L_alloc_track_loop
    mvn r0, #0
    bx lr
.L_track_pool:
    .word SNDi_TrackPool
.size SNDi_AllocTrack, . - SNDi_AllocTrack

.global SNDi_GetVariablePtr
.type SNDi_GetVariablePtr, %function
SNDi_GetVariablePtr:
    ldr r2, .L_shared_work
    ldr r2, [r2]
    cmp r2, #0
    moveq r0, #0
    bxeq lr
    cmp r1, #0x10
    addge r2, r2, #0x260
    subge r0, r1, #0x10
    addge r0, r2, r0, lsl #1
    bxge lr
    add r3, r2, #0x20
    ldrb r2, [r0, #1]
    mov r0, #0x24
    mla r0, r2, r0, r3
    add r0, r0, r1, lsl #1
    bx lr
.L_shared_work:
    .word SNDi_SharedWork
.size SNDi_GetVariablePtr, . - SNDi_GetVariablePtr

.global SNDi_PlayerSeqMain
.type SNDi_PlayerSeqMain, %function
SNDi_PlayerSeqMain:
    push {r4, r5, r6, r7, r8, lr}
    mov r8, r0
    mov r7, r1
    mov r6, #0
    mov r5, r6
    mov r4, #1
.L_player_track_loop:
    mov r0, r8
    mov r1, r5
    bl SNDi_GetPlayerTrack
    cmp r0, #0
    beq .L_player_track_next
    ldr r1, [r0, #0x28]
    cmp r1, #0
    beq .L_player_track_next
    mov r1, r8
    mov r2, r5
    mov r3, r7
    bl SNDi_TrackSeqMain
    cmp r0, #0
    moveq r6, r4
    beq .L_player_track_next
    mov r0, r8
    mov r1, r5
    bl SNDi_ClosePlayerTrack
.L_player_track_next:
    add r5, r5, #1
    cmp r5, #0x10
    blt .L_player_track_loop
    cmp r6, #0
    moveq r0, #1
    movne r0, #0
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.size SNDi_PlayerSeqMain, . - SNDi_PlayerSeqMain
