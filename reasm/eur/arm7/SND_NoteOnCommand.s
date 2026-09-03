/* Translate one sequence note command into an allocated extended channel. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_NoteOnCommand, "ax", %progbits
.balign 4

.extern SND_AllocExChannel
.extern SND_FreeExChannel
.extern SND_NoteOn
.extern SND_ReadInstData
.extern SND_SetExChannelAttack
.extern SND_SetExChannelDecay
.extern SND_SetExChannelRelease
.extern SND_SetExChannelSustain
.extern SNDi_ChannelCallback

.global SNDi_NoteOnCommandProc
.type SNDi_NoteOnCommandProc, %function
SNDi_NoteOnCommandProc:
    push {r4, r5, r6, r7, r8, lr}
    sub sp, sp, #0x18
    mov r8, r0
    mov r7, r1
    mov r6, r2
    mov r5, r3
    mov r4, #0
    ldrb r0, [r8]
    mov r0, r0, lsl #28
    movs r0, r0, lsr #31
    beq .L_allocate_note_channel
    ldr r4, [r8, #0x3C]
    cmp r4, #0
    strbne r6, [r4, #8]
    strbne r5, [r4, #9]
.L_allocate_note_channel:
    cmp r4, #0
    bne .L_configure_note_channel
    ldr r0, [r7, #0x20]
    ldrh r1, [r8, #2]
    mov r2, r6
    add r3, sp, #8
    bl SND_ReadInstData
    cmp r0, #0
    beq .L_note_done
    ldrb r0, [sp, #8]
    cmp r0, #4
    addls pc, pc, r0, lsl #2
    b .L_invalid_instrument
    b .L_invalid_instrument
    b .L_pcm_channel_mask
    b .L_psg_channel_mask
    b .L_noise_channel_mask
    b .L_pcm_channel_mask
.L_pcm_channel_mask:
    ldr r1, .L_pcm_mask
    b .L_allocate_extended_channel
.L_psg_channel_mask:
    mov r1, #0x3F00
    b .L_allocate_extended_channel
.L_noise_channel_mask:
    mov r1, #0xC000
    b .L_allocate_extended_channel
.L_invalid_instrument:
    b .L_note_done
.L_allocate_extended_channel:
    ldrh r0, [r8, #0x1E]
    and r0, r1, r0
    str r8, [sp]
    ldrb r2, [r7, #4]
    ldrb r1, [r8, #0x12]
    add r1, r2, r1
    ldrb r2, [r8]
    mov r2, r2, lsl #24
    mov r2, r2, lsr #31
    ldr r3, .L_channel_callback
    bl SND_AllocExChannel
    movs r4, r0
    beq .L_note_done
    ldrb r0, [r8]
    mov r0, r0, lsl #28
    movs r0, r0, lsr #31
    mvnne r3, #0
    ldreq r3, [sp, #0x30]
    ldr r0, [r7, #0x20]
    str r0, [sp]
    add r0, sp, #8
    str r0, [sp, #4]
    mov r0, r4
    mov r1, r6
    mov r2, r5
    bl SND_NoteOn
    cmp r0, #0
    bne .L_link_note_channel
    mov r0, #0
    strb r0, [r4, #0x22]
    mov r0, r4
    bl SND_FreeExChannel
    b .L_note_done
.L_link_note_channel:
    ldr r0, [r8, #0x3C]
    str r0, [r4, #0x50]
    str r4, [r8, #0x3C]
.L_configure_note_channel:
    ldrb r1, [r8, #0xE]
    cmp r1, #0xFF
    beq .L_skip_attack
    mov r0, r4
    bl SND_SetExChannelAttack
.L_skip_attack:
    ldrb r1, [r8, #0xF]
    cmp r1, #0xFF
    beq .L_skip_decay
    mov r0, r4
    bl SND_SetExChannelDecay
.L_skip_decay:
    ldrb r1, [r8, #0x10]
    cmp r1, #0xFF
    beq .L_skip_sustain
    mov r0, r4
    bl SND_SetExChannelSustain
.L_skip_sustain:
    ldrb r1, [r8, #0x11]
    cmp r1, #0xFF
    beq .L_skip_release
    mov r0, r4
    bl SND_SetExChannelRelease
.L_skip_release:
    ldrsh r0, [r8, #0x16]
    strh r0, [r4, #0x32]
    ldrb r0, [r8]
    mov r0, r0, lsl #26
    movs r0, r0, lsr #31
    beq .L_set_sweep_length
    ldrsh r1, [r4, #0x32]
    ldrb r0, [r8, #0x14]
    sub r0, r0, r6
    mov r0, r0, lsl #22
    add r0, r1, r0, asr #16
    strh r0, [r4, #0x32]
.L_set_sweep_length:
    ldrb r0, [r8, #0x15]
    cmp r0, #0
    bne .L_set_portamento_length
    ldr r0, [sp, #0x30]
    str r0, [r4, #0x18]
    ldrb r0, [r4, #3]
    bic r0, r0, #4
    strb r0, [r4, #3]
    b .L_reset_sweep_counter
.L_set_portamento_length:
    mul r1, r0, r0
    ldrsh r0, [r4, #0x32]
    cmp r0, #0
    rsblt r0, r0, #0
    mul r0, r1, r0
    mov r0, r0, asr #11
    str r0, [r4, #0x18]
.L_reset_sweep_counter:
    mov r0, #0
    str r0, [r4, #0x14]
.L_note_done:
    add sp, sp, #0x18
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.L_pcm_mask:
    .word 0x0000FFFF
.L_channel_callback:
    .word SNDi_ChannelCallback
.size SNDi_NoteOnCommandProc, . - SNDi_NoteOnCommandProc
