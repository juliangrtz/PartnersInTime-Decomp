/* Internal modulation, priority, and initialization helpers for ARM7 voices. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_ExChannelHelpers, "ax", %progbits
.balign 4

.extern SND_GetLfoValue
.extern SND_InitLfoParam
.extern SND_SetExChannelAttack
.extern SND_SetExChannelDecay
.extern SND_SetExChannelRelease
.extern SND_SetExChannelSustain
.extern SND_StartLfo
.extern SND_UpdateLfo
.extern SNDi_VolumeShiftTable
.extern _ll_sdiv
.extern _s32_div_f

.global SNDi_GetExChannelLfoValue
.type SNDi_GetExChannelLfoValue, %function
SNDi_GetExChannelLfoValue:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, r1
    add r0, r6, #0x28
    bl SND_GetLfoValue
    mov r4, r0
    mov r12, r0, asr #31
    mov r3, #0
    cmp r12, r3
    cmpeq r0, r3
    beq .L_lfo_update
    ldrb r1, [r6, #0x28]
    cmp r1, #0
    beq .L_lfo_pitch_or_pan
    cmp r1, #1
    beq .L_lfo_volume
    cmp r1, #2
    moveq r12, r12, lsl #6
    orreq r12, r12, r0, lsr #26
    moveq r4, r0, lsl #6
    b .L_lfo_scale_result
.L_lfo_volume:
    mov r2, #0x3C
    umull r4, r1, r0, r2
    mla r1, r0, r3, r1
    mla r1, r12, r2, r1
    mov r12, r1
    b .L_lfo_scale_result
.L_lfo_pitch_or_pan:
    mov r12, r12, lsl #6
    orr r12, r12, r0, lsr #26
    mov r4, r0, lsl #6
.L_lfo_scale_result:
    mov r4, r4, lsr #14
    orr r4, r4, r12, lsl #18
.L_lfo_update:
    cmp r5, #0
    beq .L_lfo_result
    add r0, r6, #0x28
    bl SND_UpdateLfo
.L_lfo_result:
    mov r0, r4
    pop {r4, r5, r6, lr}
    bx lr
.size SNDi_GetExChannelLfoValue, . - SNDi_GetExChannelLfoValue

.global SNDi_GetExChannelSweep
.type SNDi_GetExChannelSweep, %function
SNDi_GetExChannelSweep:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    mov r5, r1
    ldrsh r3, [r4, #0x32]
    cmp r3, #0
    moveq r0, #0
    beq .L_sweep_done
    ldr r0, [r4, #0x14]
    ldr r2, [r4, #0x18]
    cmp r0, r2
    movge r0, #0
    bge .L_sweep_done
    sub r0, r2, r0
    smull r0, r1, r3, r0
    mov r3, r2, asr #31
    bl _ll_sdiv
    cmp r5, #0
    beq .L_sweep_done
    ldrb r1, [r4, #3]
    mov r1, r1, lsl #29
    movs r1, r1, lsr #31
    ldrne r1, [r4, #0x14]
    addne r1, r1, #1
    strne r1, [r4, #0x14]
.L_sweep_done:
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SNDi_GetExChannelSweep, . - SNDi_GetExChannelSweep

.global SNDi_CompareChannelPriority
.type SNDi_CompareChannelPriority, %function
SNDi_CompareChannelPriority:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldrh lr, [r0, #0x24]
    and r2, lr, #0xFF
    ldrh r12, [r1, #0x24]
    and r0, r12, #0xFF
    mov r2, r2, lsl #4
    mov r3, r0, lsl #4
    ldr r1, .L_volume_shift_table
    ldrb r0, [r1, lr, asr #8]
    mov r2, r2, asr r0
    ldrb r0, [r1, r12, asr #8]
    mov r0, r3, asr r0
    cmp r2, r0
    beq .L_priority_equal
    cmp r2, r0
    movlt r0, #1
    mvnge r0, #0
    b .L_priority_done
.L_priority_equal:
    mov r0, #0
.L_priority_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_volume_shift_table:
    .word SNDi_VolumeShiftTable
.size SNDi_CompareChannelPriority, . - SNDi_CompareChannelPriority

.global SNDi_StartExChannel
.type SNDi_StartExChannel, %function
SNDi_StartExChannel:
    push {r4, lr}
    mov r4, r0
    ldr r0, .L_envelope_start
    str r0, [r4, #0x10]
    mov r0, #0
    strb r0, [r4, #2]
    str r1, [r4, #0x34]
    add r0, r4, #0x28
    bl SND_StartLfo
    ldrb r0, [r4, #3]
    orr r0, r0, #2
    strb r0, [r4, #3]
    ldrb r0, [r4, #3]
    bic r0, r0, #1
    orr r0, r0, #1
    strb r0, [r4, #3]
    pop {r4, lr}
    bx lr
.L_envelope_start:
    .word 0xFFFE9680
.size SNDi_StartExChannel, . - SNDi_StartExChannel

.global SNDi_InitExChannel
.type SNDi_InitExChannel, %function
SNDi_InitExChannel:
    push {r4, lr}
    mov r4, r0
    mov r12, #0
    str r12, [r4, #0x50]
    str r1, [r4, #0x48]
    str r2, [r4, #0x4C]
    str r12, [r4, #0x34]
    strb r3, [r4, #0x22]
    mov r1, #0x7F
    strh r1, [r4, #0x24]
    ldrb r2, [r4, #3]
    bic r2, r2, #2
    strb r2, [r4, #3]
    ldrb r2, [r4, #3]
    orr r2, r2, #4
    strb r2, [r4, #3]
    mov r2, #0x3C
    strb r2, [r4, #8]
    strb r2, [r4, #5]
    strb r1, [r4, #9]
    strb r12, [r4, #0xA]
    strh r12, [r4, #0xC]
    strh r12, [r4, #6]
    strh r12, [r4, #0xE]
    strb r12, [r4, #0xB]
    strb r1, [r4, #4]
    strh r12, [r4, #0x32]
    str r12, [r4, #0x18]
    str r12, [r4, #0x14]
    bl SND_SetExChannelAttack
    mov r0, r4
    mov r1, #0x7F
    bl SND_SetExChannelDecay
    mov r0, r4
    mov r1, #0x7F
    bl SND_SetExChannelSustain
    mov r0, r4
    mov r1, #0x7F
    bl SND_SetExChannelRelease
    add r0, r4, #0x28
    bl SND_InitLfoParam
    pop {r4, lr}
    bx lr
.size SNDi_InitExChannel, . - SNDi_InitExChannel

.global SNDi_CalcDecay
.type SNDi_CalcDecay, %function
SNDi_CalcDecay:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r1, r0
    cmp r1, #0x7F
    ldreq r0, .L_max_decay
    beq .L_decay_done
    cmp r1, #0x7E
    moveq r0, #0x3C00
    beq .L_decay_done
    cmp r1, #0x32
    movlt r0, r1, lsl #1
    addlt r0, r0, #1
    movlt r0, r0, lsl #16
    movlt r0, r0, lsr #16
    blt .L_decay_done
    mov r0, #0x1E00
    rsb r1, r1, #0x7E
    bl _s32_div_f
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
.L_decay_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_max_decay:
    .word 0x0000FFFF
.size SNDi_CalcDecay, . - SNDi_CalcDecay
