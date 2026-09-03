/* Prepare one sequence player and allocate tracks declared by its MML header. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_PrepareSeq, "ax", %progbits
.balign 4

.extern SNDi_PlayerPool
.extern SNDi_SharedWork
.extern SNDi_TrackPool

.global SND_PrepareSeq
.type SND_PrepareSeq, %function
SND_PrepareSeq:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r4, r0
    mov r8, r1
    mov r6, r2
    mov r5, r3
    ldr r1, .L_player_pool_prepare
    mov r0, #0x24
    mul r0, r4, r0
    add r7, r1, r0
    ldrb r0, [r1, r0]
    mov r0, r0, lsl #31
    movs r0, r0, lsr #31
    beq .L_initialize_player
    mov r0, r7
    bl SNDi_FinishPlayer
.L_initialize_player:
    mov r0, r7
    mov r1, r5
    bl SNDi_InitPlayer
    bl SNDi_AllocTrack
    movs r9, r0
    bmi .L_prepare_done
    ldr r0, .L_track_pool_prepare
    add r5, r0, r9, lsl #6
    mov r0, r5
    bl SNDi_InitTrack
    mov r0, r5
    mov r1, r8
    mov r2, r6
    bl SNDi_StartTrack
    strb r9, [r7, #8]
    ldr r0, [r5, #0x28]
    bl SNDi_InitCache
    ldr r0, [r5, #0x28]
    bl SNDi_GetByteCache
    add r2, r5, #0x28
    ldr r1, [r5, #0x28]
    add r1, r1, #1
    str r1, [r5, #0x28]
    cmp r0, #0xFE
    ldrne r0, [r2]
    subne r0, r0, #1
    strne r0, [r2]
    bne .L_activate_player
    mov r0, r5
    bl SNDi_Read16
    mov r0, r0, lsl #15
    mov r5, r0, lsr #16
    mov r6, #1
    ldr r8, .L_track_pool_prepare
    b .L_allocate_declared_track_test
.L_allocate_declared_track_loop:
    ands r0, r5, #1
    beq .L_allocate_declared_track_next
    bl SNDi_AllocTrack
    movs r9, r0
    bmi .L_activate_player
    add r0, r8, r9, lsl #6
    bl SNDi_InitTrack
    add r0, r7, r6
    strb r9, [r0, #8]
.L_allocate_declared_track_next:
    add r6, r6, #1
    mov r0, r5, lsl #15
    mov r5, r0, lsr #16
.L_allocate_declared_track_test:
    cmp r5, #0
    bne .L_allocate_declared_track_loop
.L_activate_player:
    ldrb r0, [r7]
    bic r0, r0, #1
    orr r0, r0, #1
    strb r0, [r7]
    ldrb r0, [r7]
    bic r0, r0, #2
    strb r0, [r7]
    ldr r0, .L_shared_work_prepare
    ldr r2, [r0]
    cmp r2, #0
    ldrne r1, [r2, #4]
    movne r0, #1
    orrne r0, r1, r0, lsl r4
    strne r0, [r2, #4]
.L_prepare_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.L_player_pool_prepare:
    .word SNDi_PlayerPool
.L_track_pool_prepare:
    .word SNDi_TrackPool
.L_shared_work_prepare:
    .word SNDi_SharedWork
.size SND_PrepareSeq, . - SND_PrepareSeq
