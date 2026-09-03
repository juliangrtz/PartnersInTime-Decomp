/* Initialize sequence state and decode byte-stream command arguments. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_SeqData, "ax", %progbits
.balign 4

.extern SND_CalcRandom
.extern SND_InitLfoParam
.extern SNDi_SeqCacheData
.extern SNDi_SeqCacheState
.extern SNDi_SharedWork

.global SNDi_InitPlayer
.type SNDi_InitPlayer, %function
SNDi_InitPlayer:
    push {r4, lr}
    ldrb r2, [r0]
    bic r2, r2, #4
    strb r2, [r0]
    str r1, [r0, #0x20]
    mov r1, #0x78
    strh r1, [r0, #0x18]
    mov r1, #0x100
    strh r1, [r0, #0x1A]
    mov r1, #0xF0
    strh r1, [r0, #0x1C]
    mov r1, #0x7F
    strb r1, [r0, #5]
    mov r3, #0
    strh r3, [r0, #6]
    mov r1, #0x40
    strb r1, [r0, #4]
    mov r2, #0xFF
.L_init_player_track_loop:
    add r1, r0, r3
    strb r2, [r1, #8]
    add r3, r3, #1
    cmp r3, #0x10
    blt .L_init_player_track_loop
    ldr r2, .L_shared_work_init
    ldr r3, [r2]
    cmp r3, #0
    beq .L_init_player_done
    mov r4, #0
    ldrb r12, [r0, #1]
    mov r1, #0x24
    mla r3, r12, r1, r3
    str r4, [r3, #0x40]
    mvn lr, #0
.L_init_player_variable_loop:
    ldr r3, [r2]
    ldrb r12, [r0, #1]
    mla r3, r12, r1, r3
    add r3, r3, r4, lsl #1
    strh lr, [r3, #0x20]
    add r4, r4, #1
    cmp r4, #0x10
    blt .L_init_player_variable_loop
.L_init_player_done:
    pop {r4, lr}
    bx lr
.L_shared_work_init:
    .word SNDi_SharedWork
.size SNDi_InitPlayer, . - SNDi_InitPlayer

.global SNDi_StartTrack
.type SNDi_StartTrack, %function
SNDi_StartTrack:
    str r1, [r0, #0x24]
    ldr r1, [r0, #0x24]
    add r1, r1, r2
    str r1, [r0, #0x28]
    bx lr
.size SNDi_StartTrack, . - SNDi_StartTrack

.global SNDi_InitTrack
.type SNDi_InitTrack, %function
SNDi_InitTrack:
    push {r4, lr}
    mov r4, r0
    mov r0, #0
    str r0, [r4, #0x24]
    str r0, [r4, #0x28]
    ldrb r1, [r4]
    orr r1, r1, #2
    strb r1, [r4]
    ldrb r1, [r4]
    bic r1, r1, #4
    strb r1, [r4]
    ldrb r1, [r4]
    bic r1, r1, #8
    strb r1, [r4]
    ldrb r1, [r4]
    bic r1, r1, #0x10
    strb r1, [r4]
    ldrb r1, [r4]
    bic r1, r1, #0x20
    strb r1, [r4]
    ldrb r1, [r4]
    orr r1, r1, #0x40
    strb r1, [r4]
    ldrb r1, [r4]
    bic r1, r1, #0x80
    strb r1, [r4]
    strb r0, [r4, #0x3B]
    strh r0, [r4, #2]
    mov r1, #0x40
    strb r1, [r4, #0x12]
    mov r2, #0x7F
    strb r2, [r4, #4]
    strb r2, [r4, #5]
    strh r0, [r4, #0xA]
    strb r0, [r4, #8]
    strb r0, [r4, #9]
    strb r0, [r4, #6]
    strh r0, [r4, #0xC]
    mov r1, #0xFF
    strb r1, [r4, #0xE]
    strb r1, [r4, #0xF]
    strb r1, [r4, #0x10]
    strb r1, [r4, #0x11]
    strb r2, [r4, #1]
    mov r1, #2
    strb r1, [r4, #7]
    mov r1, #0x3C
    strb r1, [r4, #0x14]
    strb r0, [r4, #0x15]
    strh r0, [r4, #0x16]
    strb r0, [r4, #0x13]
    ldr r0, .L_all_channel_mask
    strh r0, [r4, #0x1E]
    add r0, r4, #0x18
    bl SND_InitLfoParam
    mov r0, #0
    str r0, [r4, #0x20]
    str r0, [r4, #0x3C]
    pop {r4, lr}
    bx lr
.L_all_channel_mask:
    .word 0x0000FFFF
.size SNDi_InitTrack, . - SNDi_InitTrack

.global SNDi_ReadArg
.type SNDi_ReadArg, %function
SNDi_ReadArg:
    push {r4, r5, r6, lr}
    mov r4, r0
    mov r6, r1
    cmp r2, #4
    addls pc, pc, r2, lsl #2
    b .L_read_arg_done
    b .L_read_arg_u8
    b .L_read_arg_s16
    b .L_read_arg_varint
    b .L_read_arg_random
    b .L_read_arg_variable
.L_read_arg_u8:
    ldr r0, [r4, #0x28]
    bl SNDi_GetByteCache
    mov r5, r0
    ldr r0, [r4, #0x28]
    add r0, r0, #1
    str r0, [r4, #0x28]
    b .L_read_arg_done
.L_read_arg_s16:
    bl SNDi_Read16
    mov r5, r0
    b .L_read_arg_done
.L_read_arg_varint:
    bl SNDi_ReadVar
    mov r5, r0
    b .L_read_arg_done
.L_read_arg_variable:
    ldr r0, [r4, #0x28]
    bl SNDi_GetByteCache
    mov r1, r0
    ldr r0, [r4, #0x28]
    add r0, r0, #1
    str r0, [r4, #0x28]
    mov r0, r6
    bl SNDi_GetVariablePtr
    cmp r0, #0
    ldrshne r5, [r0]
    b .L_read_arg_done
.L_read_arg_random:
    bl SNDi_Read16
    mov r5, r0, lsl #16
    mov r0, r4
    bl SNDi_Read16
    mov r0, r0, lsl #16
    mov r4, r0, asr #16
    bl SND_CalcRandom
    sub r1, r4, r5, asr #16
    add r1, r1, #1
    mul r1, r0, r1
    mov r0, r1, asr #16
    add r5, r0, r5, asr #16
.L_read_arg_done:
    mov r0, r5
    pop {r4, r5, r6, lr}
    bx lr
.size SNDi_ReadArg, . - SNDi_ReadArg

.global SNDi_ReadVar
.type SNDi_ReadVar, %function
SNDi_ReadVar:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, #0
    add r4, r6, #0x28
.L_read_var_loop:
    ldr r0, [r6, #0x28]
    bl SNDi_GetByteCache
    ldr r1, [r4]
    add r1, r1, #1
    str r1, [r4]
    and r1, r0, #0x7F
    orr r5, r1, r5, lsl #7
    ands r0, r0, #0x80
    bne .L_read_var_loop
    mov r0, r5
    pop {r4, r5, r6, lr}
    bx lr
.size SNDi_ReadVar, . - SNDi_ReadVar

.global SNDi_Read24
.type SNDi_Read24, %function
SNDi_Read24:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    ldr r0, [r4, #0x28]
    bl SNDi_GetByteCache
    mov r5, r0
    ldr r0, [r4, #0x28]
    add r0, r0, #1
    str r0, [r4, #0x28]
    ldr r0, [r4, #0x28]
    bl SNDi_GetByteCache
    ldr r1, [r4, #0x28]
    add r1, r1, #1
    str r1, [r4, #0x28]
    orr r5, r5, r0, lsl #8
    ldr r0, [r4, #0x28]
    bl SNDi_GetByteCache
    ldr r1, [r4, #0x28]
    add r1, r1, #1
    str r1, [r4, #0x28]
    orr r0, r5, r0, lsl #16
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SNDi_Read24, . - SNDi_Read24

.global SNDi_Read16
.type SNDi_Read16, %function
SNDi_Read16:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    ldr r0, [r5, #0x28]
    bl SNDi_GetByteCache
    mov r4, r0
    ldr r0, [r5, #0x28]
    add r0, r0, #1
    str r0, [r5, #0x28]
    ldr r0, [r5, #0x28]
    bl SNDi_GetByteCache
    ldr r1, [r5, #0x28]
    add r1, r1, #1
    str r1, [r5, #0x28]
    orr r0, r4, r0, lsl #8
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SNDi_Read16, . - SNDi_Read16

.global SNDi_GetByteCache
.type SNDi_GetByteCache, %function
SNDi_GetByteCache:
    push {r4, lr}
    mov r4, r0
    ldr r0, .L_cache_state_get
    ldr r1, [r0]
    cmp r4, r1
    blo .L_cache_reload
    ldr r0, [r0, #4]
    cmp r4, r0
    blo .L_cache_ready
.L_cache_reload:
    mov r0, r4
    bl SNDi_InitCache
.L_cache_ready:
    ldr r0, .L_cache_state_get
    ldr r0, [r0]
    sub r1, r4, r0
    ldr r0, .L_cache_data_get
    ldrb r0, [r0, r1]
    pop {r4, lr}
    bx lr
.L_cache_state_get:
    .word SNDi_SeqCacheState
.L_cache_data_get:
    .word SNDi_SeqCacheData
.size SNDi_GetByteCache, . - SNDi_GetByteCache

.global SNDi_InitCache
.type SNDi_InitCache, %function
SNDi_InitCache:
    bic r2, r0, #3
    ldr r0, .L_cache_state_init
    str r2, [r0]
    add r1, r2, #0x10
    str r1, [r0, #4]
    ldr r1, [r2]
    str r1, [r0, #8]
    ldr r1, [r2, #4]
    str r1, [r0, #0xC]
    ldr r1, [r2, #8]
    str r1, [r0, #0x10]
    ldr r1, [r2, #0xC]
    str r1, [r0, #0x14]
    bx lr
.L_cache_state_init:
    .word SNDi_SeqCacheState
.size SNDi_InitCache, . - SNDi_InitCache
