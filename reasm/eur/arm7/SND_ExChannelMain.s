/* Advance envelopes/modulation and stage hardware updates for all voices. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_ExChannelMain, "ax", %progbits
.balign 4

.extern SND_CalcChannelVolume
.extern SND_CalcTimer
.extern SND_IsChannelActive
.extern SND_UpdateExChannelEnvelope
.extern SNDi_DecibelSquareTable
.extern SNDi_GetExChannelLfoValue
.extern SNDi_GetExChannelSweep
.extern SNDi_Work

.global SND_ExChannelMain
.type SND_ExChannelMain, %function
SND_ExChannelMain:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #0x1C
    mov r6, r0
    mov r4, #0
    mov r11, r4
    mov r7, #1
    str r4, [sp, #8]
    mov r0, #0x7F
    str r0, [sp, #0x14]
    str r4, [sp, #0x10]
    str r4, [sp, #0x0C]
    str r4, [sp, #4]
    str r4, [sp]
.L_ex_channel_main_loop:
    ldr r8, [sp]
    mov r10, r8
    mov r9, r8
    mov r1, #0x54
    ldr r0, .L_main_sound_work
    mla r5, r4, r1, r0
    ldrb r1, [r5, #3]
    mov r0, r1, lsl #31
    movs r0, r0, lsr #31
    beq .L_ex_channel_main_next
    mov r0, r1, lsl #30
    movs r0, r0, lsr #31
    beq .L_check_hardware_channel
    bic r0, r1, #0xF8
    mov r1, r1, lsl #24
    mov r1, r1, lsr #27
    orr r1, r1, #1
    and r1, r1, #0xFF
    and r1, r1, #0x1F
    orr r0, r0, r1, lsl #3
    strb r0, [r5, #3]
    ldrb r0, [r5, #3]
    bic r0, r0, #2
    strb r0, [r5, #3]
    b .L_compute_channel_state
.L_check_hardware_channel:
    mov r0, r4
    bl SND_IsChannelActive
    cmp r0, #0
    bne .L_compute_channel_state
    ldr r3, [r5, #0x48]
    cmp r3, #0
    strbeq r11, [r5, #0x22]
    beq .L_deactivate_finished_channel
    mov r0, r5
    mov r1, r7
    ldr r2, [r5, #0x4C]
    mov lr, pc
    bx r3
.L_deactivate_finished_channel:
    ldr r0, [sp, #4]
    strh r0, [r5, #0x24]
    ldrb r0, [r5, #3]
    bic r0, r0, #1
    strb r0, [r5, #3]
    b .L_ex_channel_main_next
.L_compute_channel_state:
    ldrb r0, [r5, #9]
    mov r1, r0, lsl #1
    ldr r0, .L_main_decibel_square_table
    ldrsh r0, [r0, r1]
    add r8, r8, r0
    ldrb r1, [r5, #8]
    ldrb r0, [r5, #5]
    sub r0, r1, r0
    add r10, r10, r0, lsl #6
    mov r0, r5
    mov r1, r6
    bl SND_UpdateExChannelEnvelope
    add r8, r8, r0
    mov r0, r5
    mov r1, r6
    bl SNDi_GetExChannelSweep
    add r10, r10, r0
    mov r0, r5
    mov r1, r6
    bl SNDi_GetExChannelLfoValue
    ldrb r1, [r5, #0x28]
    cmp r1, #0
    beq .L_add_lfo_to_pitch
    cmp r1, #1
    beq .L_add_lfo_to_volume
    cmp r1, #2
    addeq r9, r9, r0
    b .L_apply_channel_offsets
.L_add_lfo_to_volume:
    add r8, r8, r0
    b .L_apply_channel_offsets
.L_add_lfo_to_pitch:
    add r10, r10, r0
.L_apply_channel_offsets:
    ldrsb r0, [r5, #0x0A]
    add r9, r9, r0
    ldrb r0, [r5, #4]
    cmp r0, #0x7F
    mulne r0, r9, r0
    addne r0, r0, #0x40
    movne r9, r0, asr #7
    ldrsh r0, [r5, #0x0C]
    add r1, r8, r0
    ldrsh r0, [r5, #6]
    add r8, r1, r0
    ldrsh r0, [r5, #0x0E]
    add r10, r10, r0
    ldrsb r0, [r5, #0x0B]
    add r9, r9, r0
    ldrb r0, [r5, #2]
    cmp r0, #3
    bne .L_calculate_hardware_values
    ldr r0, .L_minimum_channel_volume
    cmp r8, r0
    bgt .L_calculate_hardware_values
    ldrb r0, [r5, #3]
    bic r0, r0, #0xF8
    orr r0, r0, #0x10
    strb r0, [r5, #3]
    ldr r3, [r5, #0x48]
    cmp r3, #0
    ldreq r0, [sp, #8]
    strbeq r0, [r5, #0x22]
    beq .L_deactivate_released_channel
    mov r0, r5
    mov r1, r7
    ldr r2, [r5, #0x4C]
    mov lr, pc
    bx r3
.L_deactivate_released_channel:
    ldr r0, [sp, #0x0C]
    strh r0, [r5, #0x24]
    ldrb r0, [r5, #3]
    bic r0, r0, #1
    strb r0, [r5, #3]
    b .L_ex_channel_main_next
.L_calculate_hardware_values:
    mov r0, r8
    bl SND_CalcChannelVolume
    mov r8, r0
    ldrh r0, [r5, #0x3C]
    mov r1, r10
    bl SND_CalcTimer
    ldrb r1, [r5, #1]
    cmp r1, #1
    ldreq r1, .L_psg_timer_mask
    andeq r0, r0, r1
    moveq r0, r0, lsl #16
    moveq r0, r0, lsr #16
    adds r9, r9, #0x40
    ldrmi r9, [sp, #0x10]
    bmi .L_stage_channel_values
    cmp r9, #0x7F
    ldrgt r9, [sp, #0x14]
.L_stage_channel_values:
    ldrh r1, [r5, #0x24]
    cmp r8, r1
    beq .L_stage_channel_timer
    strh r8, [r5, #0x24]
    ldrb r2, [r5, #3]
    bic r1, r2, #0xF8
    mov r2, r2, lsl #24
    mov r2, r2, lsr #27
    orr r2, r2, #8
    and r2, r2, #0xFF
    and r2, r2, #0x1F
    orr r1, r1, r2, lsl #3
    strb r1, [r5, #3]
.L_stage_channel_timer:
    ldrh r1, [r5, #0x26]
    cmp r0, r1
    beq .L_stage_channel_pan
    strh r0, [r5, #0x26]
    ldrb r0, [r5, #3]
    bic r1, r0, #0xF8
    mov r0, r0, lsl #24
    mov r0, r0, lsr #27
    orr r0, r0, #4
    and r0, r0, #0xFF
    and r0, r0, #0x1F
    orr r0, r1, r0, lsl #3
    strb r0, [r5, #3]
.L_stage_channel_pan:
    ldrb r0, [r5, #0x23]
    cmp r9, r0
    beq .L_ex_channel_main_next
    strb r9, [r5, #0x23]
    ldrb r0, [r5, #3]
    bic r1, r0, #0xF8
    mov r0, r0, lsl #24
    mov r0, r0, lsr #27
    orr r0, r0, #0x10
    and r0, r0, #0xFF
    and r0, r0, #0x1F
    orr r0, r1, r0, lsl #3
    strb r0, [r5, #3]
.L_ex_channel_main_next:
    add r4, r4, #1
    cmp r4, #0x10
    blt .L_ex_channel_main_loop
    add sp, sp, #0x1C
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.size SND_ExChannelMain, . - SND_ExChannelMain
.L_main_sound_work:
    .word SNDi_Work
.L_main_decibel_square_table:
    .word SNDi_DecibelSquareTable
.L_minimum_channel_volume:
    .word 0xFFFFFD2D
.L_psg_timer_mask:
    .word 0x0000FFFC
