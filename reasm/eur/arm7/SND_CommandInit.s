/* Initialize the ARM7 sound command queue and its PXI receive callback. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_CommandInit, "ax", %progbits
.balign 4

.extern OS_InitMessageQueue
.extern SNDi_CommandMesgBuffer
.extern SNDi_CommandMesgQueue
.extern SNDi_InitPxi
.extern SNDi_SharedWork

.global SND_CommandInit
.type SND_CommandInit, %function
SND_CommandInit:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_command_queue
    ldr r1, .L_command_buffer
    mov r2, #8
    bl OS_InitMessageQueue
    bl SNDi_InitPxi
    mov r1, #0
    ldr r0, .L_shared_work
    str r1, [r0]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_command_queue:
    .word SNDi_CommandMesgQueue
.L_command_buffer:
    .word SNDi_CommandMesgBuffer
.L_shared_work:
    .word SNDi_SharedWork
.size SND_CommandInit, . - SND_CommandInit
