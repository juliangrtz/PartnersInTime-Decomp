/* Terminate the current thread and release its scheduler resources. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_ExitThread, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_Terminate
.extern OS_WakeupThread
.extern OSi_CurrentThreadPtr
.extern OSi_RemoveThreadFromList
.extern OSi_RescheduleThread
.extern OSi_ThreadTable
.extern OSi_UnlockAllMutex

.global OS_ExitThread
.type OS_ExitThread, %function
OS_ExitThread:
    stmdb sp!, {r4, lr}
    bl OS_DisableInterrupts
    ldr r0, .L_current_thread_exit
    ldr r0, [r0]
    ldr r4, [r0]
    mov r0, r4
    bl OSi_RemoveThreadFromList
    mov r0, r4
    bl OSi_UnlockAllMutex
    mov r2, #0
    ldr r1, [r4, #0x50]
    ldr r0, .L_thread_table
    str r2, [r0, r1, lsl #2]
    mov r0, #2
    str r0, [r4, #0x48]
    add r0, r4, #0x74
    bl OS_WakeupThread
    bl OSi_RescheduleThread
    bl OS_Terminate
    ldmia sp!, {r4, lr}
    bx lr
.L_current_thread_exit:
    .word OSi_CurrentThreadPtr
.L_thread_table:
    .word OSi_ThreadTable
.size OS_ExitThread, . - OS_ExitThread
