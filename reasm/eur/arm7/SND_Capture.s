/* Configure and query the Nintendo DS sound-capture units. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_Capture, "ax", %progbits
.balign 4

.global SND_IsCaptureActive
.type SND_IsCaptureActive, %function
SND_IsCaptureActive:
    add r0, r0, #0x04000000
    ldrb r0, [r0, #0x508]
    ands r0, r0, #0x80
    movne r0, #1
    moveq r0, #0
    bx lr
.size SND_IsCaptureActive, . - SND_IsCaptureActive

.global SND_SetupCapture
.type SND_SetupCapture, %function
SND_SetupCapture:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0, lsl #3
    ldr r12, [sp, #0x10]
    cmp r12, #0
    movne r5, #0
    moveq r5, #1
    ldr lr, [sp, #0x18]
    ldr r12, [sp, #0x14]
    mov r1, r1, lsl #3
    orr r1, r1, r5, lsl #2
    orr r1, r1, r12, lsl #1
    orr r1, lr, r1
    add r0, r0, #0x04000000
    strb r1, [r0, #0x508]
    add r0, r4, #0x04000000
    str r2, [r0, #0x510]
    add r0, r0, #0x500
    strh r3, [r0, #0x14]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size SND_SetupCapture, . - SND_SetupCapture
