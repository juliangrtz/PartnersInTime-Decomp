/* Allocate and configure the ARM7 extended-channel voice state. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_ExChannelControl, "ax", %progbits
.balign 4

.extern SNDi_AttackTable
.extern SNDi_CalcDecay
.extern SNDi_ChannelOrder
.extern SNDi_CompareChannelPriority
.extern SNDi_DecibelSquareTable
.extern SNDi_InitExChannel
.extern SNDi_StartExChannel
.extern SNDi_Work
.extern sLockedChannelMask
.extern sWeakLockedChannelMask

.global SND_FreeExChannel
.type SND_FreeExChannel, %function
SND_FreeExChannel:
    cmp r0, #0
    movne r1, #0
    strne r1, [r0, #0x48]
    strne r1, [r0, #0x4C]
    bx lr
.size SND_FreeExChannel, . - SND_FreeExChannel

.global SND_AllocExChannel
.type SND_AllocExChannel, %function
SND_AllocExChannel:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r10, r0
    mov r9, r1
    mov r11, r3
    ldr r0, .L_weak_locked_mask_alloc
    ldr r0, [r0]
    mvn r0, r0
    and r10, r10, r0
    cmp r2, #0
    ldreq r0, .L_locked_mask_alloc
    ldreq r0, [r0]
    mvneq r0, r0
    andeq r10, r10, r0
    mov r8, #0
    mov r7, r8
    mov r5, #1
    mov r4, #0x54
.L_alloc_channel_loop:
    ldr r0, .L_channel_order
    ldrb r1, [r0, r7]
    mov r0, r5, lsl r1
    ands r0, r10, r0
    beq .L_alloc_channel_next
    ldr r0, .L_alloc_sound_work
    mla r6, r1, r4, r0
    cmp r8, #0
    moveq r8, r6
    beq .L_alloc_channel_next
    ldrb r1, [r8, #0x22]
    ldrb r0, [r6, #0x22]
    cmp r0, r1
    bhi .L_alloc_channel_next
    cmp r0, r1
    bne .L_choose_channel
    mov r0, r8
    mov r1, r6
    bl SNDi_CompareChannelPriority
    cmp r0, #0
    bge .L_alloc_channel_next
.L_choose_channel:
    mov r8, r6
.L_alloc_channel_next:
    add r7, r7, #1
    cmp r7, #0x10
    blt .L_alloc_channel_loop
    cmp r8, #0
    moveq r0, #0
    beq .L_alloc_channel_done
    ldrb r0, [r8, #0x22]
    cmp r9, r0
    movlt r0, #0
    blt .L_alloc_channel_done
    ldr r3, [r8, #0x48]
    cmp r3, #0
    beq .L_initialize_channel
    mov r0, r8
    mov r1, #0
    ldr r2, [r8, #0x4C]
    mov lr, pc
    bx r3
.L_initialize_channel:
    ldrb r0, [r8, #3]
    bic r0, r0, #0xF8
    orr r0, r0, #0x10
    strb r0, [r8, #3]
    ldrb r0, [r8, #3]
    bic r0, r0, #1
    strb r0, [r8, #3]
    mov r0, r8
    mov r1, r11
    ldr r2, [sp, #0x28]
    mov r3, r9
    bl SNDi_InitExChannel
    mov r0, r8
.L_alloc_channel_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.size SND_AllocExChannel, . - SND_AllocExChannel
.L_weak_locked_mask_alloc:
    .word sWeakLockedChannelMask
.L_locked_mask_alloc:
    .word sLockedChannelMask
.L_channel_order:
    .word SNDi_ChannelOrder
.L_alloc_sound_work:
    .word SNDi_Work

.global SND_IsExChannelActive
.type SND_IsExChannelActive, %function
SND_IsExChannelActive:
    ldrb r0, [r0, #3]
    mov r0, r0, lsl #31
    mov r0, r0, lsr #31
    bx lr
.size SND_IsExChannelActive, . - SND_IsExChannelActive

.global SND_ReleaseExChannel
.type SND_ReleaseExChannel, %function
SND_ReleaseExChannel:
    mov r1, #3
    strb r1, [r0, #2]
    bx lr
.size SND_ReleaseExChannel, . - SND_ReleaseExChannel

.global SND_SetExChannelRelease
.type SND_SetExChannelRelease, %function
SND_SetExChannelRelease:
    push {r4, lr}
    mov r4, r0
    mov r0, r1
    bl SNDi_CalcDecay
    strh r0, [r4, #0x20]
    pop {r4, lr}
    bx lr
.size SND_SetExChannelRelease, . - SND_SetExChannelRelease

.global SND_SetExChannelSustain
.type SND_SetExChannelSustain, %function
SND_SetExChannelSustain:
    strb r1, [r0, #0x1D]
    bx lr
.size SND_SetExChannelSustain, . - SND_SetExChannelSustain

.global SND_SetExChannelDecay
.type SND_SetExChannelDecay, %function
SND_SetExChannelDecay:
    push {r4, lr}
    mov r4, r0
    mov r0, r1
    bl SNDi_CalcDecay
    strh r0, [r4, #0x1E]
    pop {r4, lr}
    bx lr
.size SND_SetExChannelDecay, . - SND_SetExChannelDecay

.global SND_SetExChannelAttack
.type SND_SetExChannelAttack, %function
SND_SetExChannelAttack:
    cmp r1, #0x6D
    rsblt r1, r1, #0xFF
    strblt r1, [r0, #0x1C]
    rsbge r2, r1, #0x7F
    ldrge r1, .L_attack_table
    ldrbge r1, [r1, r2]
    strbge r1, [r0, #0x1C]
    bx lr
.size SND_SetExChannelAttack, . - SND_SetExChannelAttack
.L_attack_table:
    .word SNDi_AttackTable

.global SND_UpdateExChannelEnvelope
.type SND_UpdateExChannelEnvelope, %function
SND_UpdateExChannelEnvelope:
    cmp r1, #0
    beq .L_envelope_result
    ldrb r1, [r0, #2]
    cmp r1, #3
    addls pc, pc, r1, lsl #2
    b .L_envelope_result
    b .L_envelope_attack
    b .L_envelope_decay
    b .L_envelope_result
    b .L_envelope_release
.L_envelope_attack:
    ldr r1, [r0, #0x10]
    rsb r2, r1, #0
    ldrb r1, [r0, #0x1C]
    mul r1, r2, r1
    mov r1, r1, asr #8
    rsb r1, r1, #0
    str r1, [r0, #0x10]
    ldr r1, [r0, #0x10]
    cmp r1, #0
    moveq r1, #1
    strbeq r1, [r0, #2]
    b .L_envelope_result
.L_envelope_decay:
    ldrb r1, [r0, #0x1D]
    mov r2, r1, lsl #1
    ldr r1, .L_decibel_square_table
    ldrsh r1, [r1, r2]
    mov r3, r1, lsl #7
    ldr r2, [r0, #0x10]
    ldrh r1, [r0, #0x1E]
    sub r1, r2, r1
    str r1, [r0, #0x10]
    ldr r1, [r0, #0x10]
    cmp r1, r3
    strle r3, [r0, #0x10]
    movle r1, #2
    strble r1, [r0, #2]
    b .L_envelope_result
.L_envelope_release:
    ldr r2, [r0, #0x10]
    ldrh r1, [r0, #0x20]
    sub r1, r2, r1
    str r1, [r0, #0x10]
.L_envelope_result:
    ldr r0, [r0, #0x10]
    mov r0, r0, asr #7
    bx lr
.size SND_UpdateExChannelEnvelope, . - SND_UpdateExChannelEnvelope
.L_decibel_square_table:
    .word SNDi_DecibelSquareTable

.global SND_StartExChannelNoise
.type SND_StartExChannelNoise, %function
SND_StartExChannelNoise:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldrb r2, [r0]
    cmp r2, #0x0E
    movlo r0, #0
    blo .L_start_noise_done
    cmp r2, #0x0F
    movhi r0, #0
    bhi .L_start_noise_done
    mov r2, #2
    strb r2, [r0, #1]
    ldr r2, .L_default_ex_channel_timer_noise
    strh r2, [r0, #0x3C]
    bl SNDi_StartExChannel
    mov r0, #1
.L_start_noise_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SND_StartExChannelNoise, . - SND_StartExChannelNoise
.L_default_ex_channel_timer_noise:
    .word 0x00001F46

.global SND_StartExChannelPsg
.type SND_StartExChannelPsg, %function
SND_StartExChannelPsg:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldrb r3, [r0]
    cmp r3, #8
    movlo r0, #0
    blo .L_start_psg_done
    cmp r3, #0x0D
    movhi r0, #0
    bhi .L_start_psg_done
    mov r3, #1
    strb r3, [r0, #1]
    str r1, [r0, #0x44]
    ldr r1, .L_default_ex_channel_timer_psg
    strh r1, [r0, #0x3C]
    mov r1, r2
    bl SNDi_StartExChannel
    mov r0, #1
.L_start_psg_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SND_StartExChannelPsg, . - SND_StartExChannelPsg
.L_default_ex_channel_timer_psg:
    .word 0x00001F46

.global SND_StartExChannelPcm
.type SND_StartExChannelPcm, %function
SND_StartExChannelPcm:
    push {r4, lr}
    mov r4, r0
    mov lr, r2
    mov r0, #0
    strb r0, [r4, #1]
    add r12, r4, #0x38
    ldmia r1, {r0, r1, r2}
    stmia r12, {r0, r1, r2}
    str lr, [r4, #0x44]
    mov r0, r4
    mov r1, r3
    bl SNDi_StartExChannel
    mov r0, #1
    pop {r4, lr}
    bx lr
.size SND_StartExChannelPcm, . - SND_StartExChannelPcm
