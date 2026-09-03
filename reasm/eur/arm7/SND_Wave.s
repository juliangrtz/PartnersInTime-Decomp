/* Invalidate active PCM channels whose wave data lies in a freed range. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_Wave, "ax", %progbits
.balign 4

.extern SND_StopChannel
.extern SNDi_Work

.global SND_InvalidateWave
.type SND_InvalidateWave, %function
SND_InvalidateWave:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r7, r0
    mov r6, r1
    mov r5, #0
    mov r9, r5
    ldr r4, .L_sound_work
    mov r8, #0x54
.L_invalidate_wave_loop:
    mla r2, r5, r8, r4
    ldrb r1, [r2, #3]
    mov r0, r1, lsl #31
    movs r0, r0, lsr #31
    beq .L_invalidate_wave_next
    ldrb r0, [r2, #1]
    cmp r0, #0
    bne .L_invalidate_wave_next
    ldr r0, [r2, #0x44]
    cmp r7, r0
    bhi .L_invalidate_wave_next
    cmp r0, r6
    bhi .L_invalidate_wave_next
    bic r0, r1, #2
    strb r0, [r2, #3]
    mov r0, r5
    mov r1, r9
    bl SND_StopChannel
.L_invalidate_wave_next:
    add r0, r5, #1
    and r5, r0, #0xFF
    cmp r5, #0x10
    blo .L_invalidate_wave_loop
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.size SND_InvalidateWave, . - SND_InvalidateWave
.L_sound_work:
    .word SNDi_Work
