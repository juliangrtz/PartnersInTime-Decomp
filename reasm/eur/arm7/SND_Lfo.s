/* Low-frequency oscillator state used by ARM7 extended sound channels. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_Lfo, "ax", %progbits
.balign 4

.extern SND_SinIdx

.global SND_GetLfoValue
.type SND_GetLfoValue, %function
SND_GetLfoValue:
    push {r4, lr}
    mov r4, r0
    ldrb r0, [r4, #2]
    cmp r0, #0
    moveq r0, #0
    beq .L_lfo_value_done
    ldrh r1, [r4, #6]
    ldrh r0, [r4, #4]
    cmp r1, r0
    movlo r0, #0
    blo .L_lfo_value_done
    ldrh r0, [r4, #8]
    mov r0, r0, lsr #8
    bl SND_SinIdx
    ldrb r2, [r4, #3]
    ldrb r1, [r4, #2]
    mul r0, r1, r0
    mul r0, r2, r0
.L_lfo_value_done:
    pop {r4, lr}
    bx lr
.size SND_GetLfoValue, . - SND_GetLfoValue

.global SND_UpdateLfo
.type SND_UpdateLfo, %function
SND_UpdateLfo:
    ldrh r2, [r0, #6]
    ldrh r1, [r0, #4]
    cmp r2, r1
    addlo r1, r2, #1
    strhlo r1, [r0, #6]
    bxlo lr
    ldrh r2, [r0, #8]
    ldrb r1, [r0, #1]
    mov r3, r1, lsl #6
    add r1, r2, r1, lsl #6
    mov r2, r1, lsr #8
    b .L_wrap_lfo_phase_test
.L_wrap_lfo_phase:
    sub r2, r2, #0x80
.L_wrap_lfo_phase_test:
    cmp r2, #0x80
    bhs .L_wrap_lfo_phase
    ldrh r1, [r0, #8]
    add r1, r1, r3
    strh r1, [r0, #8]
    ldrh r1, [r0, #8]
    and r1, r1, #0xFF
    strh r1, [r0, #8]
    ldrh r1, [r0, #8]
    orr r1, r1, r2, lsl #8
    strh r1, [r0, #8]
    bx lr
.size SND_UpdateLfo, . - SND_UpdateLfo

.global SND_StartLfo
.type SND_StartLfo, %function
SND_StartLfo:
    mov r1, #0
    strh r1, [r0, #8]
    strh r1, [r0, #6]
    bx lr
.size SND_StartLfo, . - SND_StartLfo

.global SND_InitLfoParam
.type SND_InitLfoParam, %function
SND_InitLfoParam:
    mov r2, #0
    strb r2, [r0]
    strb r2, [r0, #2]
    mov r1, #1
    strb r1, [r0, #3]
    mov r1, #0x10
    strb r1, [r0, #1]
    strh r2, [r0, #4]
    bx lr
.size SND_InitLfoParam, . - SND_InitLfoParam
