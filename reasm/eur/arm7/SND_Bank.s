/* Resolve sequence instruments and start their extended sound channels. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_Bank, "ax", %progbits
.balign 4

.extern SND_SetExChannelAttack
.extern SND_SetExChannelDecay
.extern SND_SetExChannelRelease
.extern SND_SetExChannelSustain
.extern SND_StartExChannelNoise
.extern SND_StartExChannelPcm
.extern SND_StartExChannelPsg
.extern SNDi_GetWaveData
.extern SNDi_LockMutex
.extern SNDi_UnlockMutex

.global SND_NoteOn
.type SND_NoteOn, %function
SND_NoteOn:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r9, r0
    mov r8, r1
    mov r7, r2
    mov r6, r3
    ldr r5, [sp, #0x24]
    ldrb r4, [r5, #0xa]
    cmp r4, #0xff
    mvneq r6, #0
    moveq r4, #0
    ldrb r0, [r5]
    cmp r0, #4
    addls pc, pc, r0, lsl #2
    b .L_note_fail
    b .L_note_fail
    b .L_note_pcm
    b .L_note_psg
    b .L_note_noise
    b .L_note_pcm
.L_note_pcm:
    cmp r0, #1
    ldrhne r1, [r5, #4]
    ldrhne r0, [r5, #2]
    orrne r1, r0, r1, lsl #16
    bne .L_note_have_wave
    ldr r0, [sp, #0x20]
    ldrh r1, [r5, #4]
    ldrh r2, [r5, #2]
    bl SNDi_GetWaveData
    mov r1, r0
.L_note_have_wave:
    cmp r1, #0
    moveq r0, #0
    beq .L_note_started
    mov r0, r9
    add r2, r1, #0xc
    mov r3, r6
    bl SND_StartExChannelPcm
    b .L_note_started
.L_note_psg:
    mov r0, r9
    ldrh r1, [r5, #2]
    mov r2, r6
    bl SND_StartExChannelPsg
    b .L_note_started
.L_note_noise:
    mov r0, r9
    mov r1, r6
    bl SND_StartExChannelNoise
    b .L_note_started
.L_note_fail:
    mov r0, #0
.L_note_started:
    cmp r0, #0
    moveq r0, #0
    beq .L_note_done
    strb r8, [r9, #8]
    ldrb r0, [r5, #6]
    strb r0, [r9, #5]
    strb r7, [r9, #9]
    mov r0, r9
    ldrb r1, [r5, #7]
    bl SND_SetExChannelAttack
    mov r0, r9
    ldrb r1, [r5, #8]
    bl SND_SetExChannelDecay
    mov r0, r9
    ldrb r1, [r5, #9]
    bl SND_SetExChannelSustain
    mov r0, r9
    mov r1, r4
    bl SND_SetExChannelRelease
    ldrb r0, [r5, #0xb]
    sub r0, r0, #0x40
    strb r0, [r9, #0xa]
    mov r0, #1
.L_note_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.size SND_NoteOn, . - SND_NoteOn

.global SND_GetWaveDataAddress
.type SND_GetWaveDataAddress, %function
SND_GetWaveDataAddress:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, r1
    bl SNDi_LockMutex
    add r0, r5, r4, lsl #2
    ldr r4, [r0, #0x3c]
    cmp r4, #0
    beq .L_wave_address_null
    cmp r4, #0x02000000
    addlo r4, r5, r4
    b .L_wave_address_unlock
.L_wave_address_null:
    mov r4, #0
.L_wave_address_unlock:
    bl SNDi_UnlockMutex
    mov r0, r4
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SND_GetWaveDataAddress, . - SND_GetWaveDataAddress

.global SND_ReadInstData
.type SND_ReadInstData, %function
SND_ReadInstData:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r6, r0
    movs r7, r1
    mov r5, r2
    mov r4, r3
    movmi r0, #0
    bmi .L_read_inst_done
    bl SNDi_LockMutex
    ldr r0, [r6, #0x38]
    cmp r7, r0
    blo .L_read_inst_index_valid
    bl SNDi_UnlockMutex
    mov r0, #0
    b .L_read_inst_done
.L_read_inst_index_valid:
    add r0, r6, r7, lsl #2
    ldr r3, [r0, #0x3c]
    strb r3, [r4]
    ldrb r0, [r4]
    cmp r0, #0x11
    addls pc, pc, r0, lsl #2
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_direct
    b .L_read_inst_direct
    b .L_read_inst_direct
    b .L_read_inst_direct
    b .L_read_inst_direct
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_invalid
    b .L_read_inst_drum_set
    b .L_read_inst_key_split
.L_read_inst_direct:
    add r3, r6, r3, lsr #8
    add r2, r4, #2
    mov r1, #5
.L_copy_direct_params:
    ldrh r0, [r3], #2
    strh r0, [r2], #2
    subs r1, r1, #1
    bne .L_copy_direct_params
    b .L_read_inst_success
.L_read_inst_drum_set:
    add r2, r6, r3, lsr #8
    ldrb r1, [r2, #1]
    ldrb r0, [r6, r3, lsr #8]
    cmp r5, r0
    blt .L_read_drum_invalid
    cmp r5, r1
    ble .L_read_drum_valid
.L_read_drum_invalid:
    bl SNDi_UnlockMutex
    mov r0, #0
    b .L_read_inst_done
.L_read_drum_valid:
    sub r1, r5, r0
    mov r0, #0xc
    mla r0, r1, r0, r2
    add r2, r0, #2
    mov r1, #6
.L_copy_drum_params:
    ldrh r0, [r2], #2
    strh r0, [r4], #2
    subs r1, r1, #1
    bne .L_copy_drum_params
    b .L_read_inst_success
.L_read_inst_key_split:
    mov r2, #0
    add r1, r6, r3, lsr #8
    b .L_check_key_split
.L_next_key_split:
    add r2, r2, #1
    cmp r2, #8
    blt .L_check_key_split
    bl SNDi_UnlockMutex
    mov r0, #0
    b .L_read_inst_done
.L_check_key_split:
    ldrb r0, [r1, r2]
    cmp r5, r0
    bgt .L_next_key_split
    mov r0, #0xc
    mla r0, r2, r0, r1
    add r2, r0, #8
    mov r1, #6
.L_copy_key_split_params:
    ldrh r0, [r2], #2
    strh r0, [r4], #2
    subs r1, r1, #1
    bne .L_copy_key_split_params
    b .L_read_inst_success
.L_read_inst_invalid:
    bl SNDi_UnlockMutex
    mov r0, #0
    b .L_read_inst_done
.L_read_inst_success:
    bl SNDi_UnlockMutex
    mov r0, #1
.L_read_inst_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size SND_ReadInstData, . - SND_ReadInstData
