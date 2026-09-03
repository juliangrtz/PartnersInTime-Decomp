/* Direct and wait-queue thread sleep/wakeup primitives. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_ThreadSleepWake, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OSi_CurrentThreadPtr
.extern OSi_RescheduleThread
.extern OSi_ThreadInfo

.global OS_WakeupThreadDirect
.type OS_WakeupThreadDirect, %function
OS_WakeupThreadDirect:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    bl OS_DisableInterrupts
    mov r4, r0
    mov r0, #1
    str r0, [r5, #0x48]
    bl OSi_RescheduleThread
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.size OS_WakeupThreadDirect, . - OS_WakeupThreadDirect

.global OS_WakeupThread
.type OS_WakeupThread, %function
OS_WakeupThread:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    bl OS_DisableInterrupts
    mov r4, r0
    ldrh r3, [r5]
    cmp r3, #0
    beq .L_wakeup_done
    ldr r0, .L_thread_info_wakeup
    ldr r2, [r0, #0x0C]
    mov r1, #1
    b .L_test_wakeup_thread
.L_wakeup_next:
    ldr r0, [r2, #0x50]
    mov r0, r1, lsl r0
    ands r0, r3, r0
    strne r1, [r2, #0x48]
    ldr r2, [r2, #0x4C]
.L_test_wakeup_thread:
    cmp r2, #0
    bne .L_wakeup_next
    mov r0, #0
    strh r0, [r5]
    bl OSi_RescheduleThread
.L_wakeup_done:
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_thread_info_wakeup:
    .word OSi_ThreadInfo
.size OS_WakeupThread, . - OS_WakeupThread

.global OS_SleepThread
.type OS_SleepThread, %function
OS_SleepThread:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r0, .L_current_thread_sleep
    ldr r0, [r0]
    ldr r3, [r0]
    cmp r5, #0
    beq .L_suspend_current_thread
    ldrh r2, [r5]
    mov r1, #1
    ldr r0, [r3, #0x50]
    mov r0, r1, lsl r0
    mov r0, r0, lsl #16
    orr r0, r2, r0, lsr #16
    strh r0, [r5]
.L_suspend_current_thread:
    mov r0, #0
    str r0, [r3, #0x48]
    bl OSi_RescheduleThread
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_current_thread_sleep:
    .word OSi_CurrentThreadPtr
.size OS_SleepThread, . - OS_SleepThread
