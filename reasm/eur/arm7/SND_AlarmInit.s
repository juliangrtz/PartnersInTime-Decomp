/* Reset the eight sound-alarm state records inside the sound work area. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_AlarmInit, "ax", %progbits
.balign 4

.extern SNDi_Work

.global SND_AlarmInit
.type SND_AlarmInit, %function
SND_AlarmInit:
    mov r3, #0
    mov r2, r3
    ldr r1, .L_sound_work
.L_clear_alarm_state:
    add r0, r1, r3, lsl #6
    strb r2, [r0, #0xf80]
    strb r2, [r0, #0xf81]
    add r3, r3, #1
    cmp r3, #8
    blt .L_clear_alarm_state
    bx lr
.L_sound_work:
    .word SNDi_Work
.size SND_AlarmInit, . - SND_AlarmInit
