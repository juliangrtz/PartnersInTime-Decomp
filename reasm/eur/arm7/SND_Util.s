/* NitroSDK sound random, sine, volume, and pitch/timer calculations. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_Util, "ax", %progbits
.balign 4

.extern SVC_GetPitchTable
.extern SVC_GetVolumeTable
.extern SNDi_SineTable
.extern SNDi_Random

.global SND_CalcRandom
.type SND_CalcRandom, %function
SND_CalcRandom:
    ldr r2, .L_random_state
    ldr r3, [r2]
    ldr r0, .L_random_multiplier
    ldr r1, .L_random_increment
    mla r0, r3, r0, r1
    str r0, [r2]
    mov r0, r0, lsr #16
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    bx lr
.size SND_CalcRandom, . - SND_CalcRandom
.L_random_state:
    .word SNDi_Random
.L_random_multiplier:
    .word 0x0019660D
.L_random_increment:
    .word 0x3C6EF35F

.global SND_SinIdx
.type SND_SinIdx, %function
SND_SinIdx:
    cmp r0, #0x20
    ldrlt r1, .L_sine_table
    ldrsblt r0, [r1, r0]
    bxlt lr
    cmp r0, #0x40
    rsblt r1, r0, #0x40
    ldrlt r0, .L_sine_table
    ldrsblt r0, [r0, r1]
    bxlt lr
    cmp r0, #0x60
    bge .L_sine_fourth_quadrant
    sub r1, r0, #0x40
    ldr r0, .L_sine_table
    ldrsb r0, [r0, r1]
    rsb r0, r0, #0
    mov r0, r0, lsl #24
    mov r0, r0, asr #24
    bx lr
.L_sine_fourth_quadrant:
    sub r0, r0, #0x60
    rsb r1, r0, #0x20
    ldr r0, .L_sine_table
    ldrsb r0, [r0, r1]
    rsb r0, r0, #0
    mov r0, r0, lsl #24
    mov r0, r0, asr #24
    bx lr
.size SND_SinIdx, . - SND_SinIdx
.L_sine_table:
    .word SNDi_SineTable

.global SND_CalcChannelVolume
.type SND_CalcChannelVolume, %function
SND_CalcChannelVolume:
    push {r4, lr}
    mov r4, r0
    ldr r0, .L_min_channel_volume
    cmp r4, r0
    movlt r4, r0
    blt .L_volume_clamped
    cmp r4, #0
    movgt r4, #0
.L_volume_clamped:
    ldr r0, .L_channel_volume_offset
    add r0, r4, r0
    bl SND_GetVolumeTable
    mvn r1, #0xEF
    cmp r4, r1
    movlt r1, #3
    blt .L_volume_shift_ready
    mvn r1, #0x77
    cmp r4, r1
    movlt r1, #2
    blt .L_volume_shift_ready
    mvn r1, #0x3B
    cmp r4, r1
    movlt r1, #1
    movge r1, #0
.L_volume_shift_ready:
    orr r0, r0, r1, lsl #8
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    pop {r4, lr}
    bx lr
.size SND_CalcChannelVolume, . - SND_CalcChannelVolume
.L_min_channel_volume:
    .word 0xFFFFFD2D
.L_channel_volume_offset:
    .word 0x000002D3

.global SND_GetVolumeTable
.type SND_GetVolumeTable, %function
SND_GetVolumeTable:
    ldr r12, .L_volume_table_svc
    bx r12
.size SND_GetVolumeTable, . - SND_GetVolumeTable
.L_volume_table_svc:
    /* ELF marks the now-defined .thumb_func symbol odd as a callable pointer. */
    .word SVC_GetVolumeTable

.global SND_CalcTimer
.type SND_CalcTimer, %function
SND_CalcTimer:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, #0
    rsb r0, r1, #0
    b .L_pitch_negative_test
.L_pitch_negative_loop:
    sub r4, r4, #1
    add r0, r0, #0x300
.L_pitch_negative_test:
    cmp r0, #0
    blt .L_pitch_negative_loop
    b .L_pitch_positive_test
.L_pitch_positive_loop:
    add r4, r4, #1
    sub r0, r0, #0x300
.L_pitch_positive_test:
    cmp r0, #0x300
    bge .L_pitch_positive_loop
    bl SND_GetPitchTable
    mov r2, #0
    mov r1, #0x10000
    adds lr, r0, r1
    adc r12, r2, #0
    mov r3, r5, asr #31
    umull r1, r0, lr, r5
    mla r0, lr, r3, r0
    mla r0, r12, r5, r0
    sub lr, r4, #0x10
    cmp lr, #0
    bgt .L_timer_shift_left
    rsb r2, lr, #0
    mov r3, r0, lsr r2
    mov r5, r1, lsr r2
    rsb r1, r2, #0x20
    orr r5, r5, r0, lsl r1
    sub r1, r2, #0x20
    orr r5, r5, r0, lsr r1
    b .L_timer_clamp
.L_timer_shift_left:
    cmp lr, #0x20
    bge .L_timer_overflow
    mvn r5, #0
    rsb r4, lr, #0x20
    mov r12, r5, lsl r4
    rsb r3, r4, #0x20
    orr r12, r12, r5, lsr r3
    sub r3, r4, #0x20
    orr r12, r12, r5, lsl r3
    and r3, r0, r12
    and r5, r1, r5, lsl r4
    cmp r3, r2
    cmpeq r5, r2
    ldrne r0, .L_timer_max
    bne .L_timer_return
    mov r5, r1, lsl lr
    mov r3, r0, lsl lr
    orr r3, r3, r1, lsr r4
    sub r0, lr, #0x20
    orr r3, r3, r1, lsl r0
    b .L_timer_clamp
.L_timer_overflow:
    ldr r0, .L_timer_max
    b .L_timer_return
.L_timer_clamp:
    mov r1, #0
    mov r0, #0x10
    cmp r3, r1
    cmpeq r5, r0
    movlo r5, r0
    blo .L_timer_pack
    ldr r0, .L_timer_max
    cmp r3, r1
    cmpeq r5, r0
    movhi r5, r0
.L_timer_pack:
    mov r0, r5, lsl #16
    mov r0, r0, lsr #16
.L_timer_return:
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SND_CalcTimer, . - SND_CalcTimer
.L_timer_max:
    .word 0x0000FFFF

.global SND_GetPitchTable
.type SND_GetPitchTable, %function
SND_GetPitchTable:
    ldr r12, .L_pitch_table_svc
    bx r12
.size SND_GetPitchTable, . - SND_GetPitchTable
.L_pitch_table_svc:
    /* ELF marks the now-defined .thumb_func symbol odd as a callable pointer. */
    .word SVC_GetPitchTable
