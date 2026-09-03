/* Install and enable one callback-backed ARM7 timer IRQ handler. */

.syntax unified
.arch armv4t
.arm
.section .pit_OSi_SetTimerCallback, "ax", %progbits
.balign 4

.global OSi_SetTimerCallback
.type OSi_SetTimerCallback, %function
.extern OS_EnableIrqMask
.extern OSi_TimerCallbackInfo
OSi_SetTimerCallback:
    stmdb sp!, {r4, lr}
    mov r3, #12
    mul r4, r0, r3
    ldr r3, .L_timer_callback
    str r1, [r3, r4]
    ldr r1, .L_timer_argument
    str r2, [r1, r4]
    mov r1, #1
    add r0, r0, #3
    mov r0, r1, lsl r0
    bl OS_EnableIrqMask
    mov r1, #1
    ldr r0, .L_timer_enabled
    str r1, [r0, r4]
    ldmia sp!, {r4, lr}
    bx lr
.L_timer_callback:
    .word OSi_TimerCallbackInfo
.L_timer_argument:
    .word OSi_TimerCallbackInfo + 8
.L_timer_enabled:
    .word OSi_TimerCallbackInfo + 4

.size OSi_SetTimerCallback, . - OSi_SetTimerCallback
