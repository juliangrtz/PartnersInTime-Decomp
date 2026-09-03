/* Atomically replace NitroSDK's thread-switch callback. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_SetSwitchThreadCallback, "ax", %progbits
.balign 4

.global OS_SetSwitchThreadCallback
.type OS_SetSwitchThreadCallback, %function
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OSi_ThreadInfo
OS_SetSwitchThreadCallback:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    bl OS_DisableInterrupts
    ldr r1, .L_thread_info
    ldr r4, [r1, #0x10]
    str r5, [r1, #0x10]
    bl OS_RestoreInterrupts
    mov r0, r4
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr

.L_thread_info:
    .word OSi_ThreadInfo

.size OS_SetSwitchThreadCallback, . - OS_SetSwitchThreadCallback
