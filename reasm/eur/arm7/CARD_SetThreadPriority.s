/* Atomically update the CARD worker thread's scheduling priority. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_SetThreadPriority, "ax", %progbits
.balign 4

.extern CARDi_Work
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SetThreadPriority

.global CARD_SetThreadPriority
.type CARD_SetThreadPriority, %function
CARD_SetThreadPriority:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r0
    ldr r6, .L_card_work
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r5, [r6, #0x3c]
    str r7, [r6, #0x3c]
    add r0, r6, #0x40
    ldr r1, [r6, #0x3c]
    bl OS_SetThreadPriority
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, r5
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_card_work:
    .word CARDi_Work
.size CARD_SetThreadPriority, . - CARD_SetThreadPriority
