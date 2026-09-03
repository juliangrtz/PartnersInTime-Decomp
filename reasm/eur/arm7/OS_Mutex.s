/* NitroSDK recursive mutex ownership and wait-queue operations. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Mutex, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SleepThread
.extern OS_WakeupThread
.extern OSi_ThreadInfo

.global OSi_RemoveMutexLinkFromQueue
.type OSi_RemoveMutexLinkFromQueue, %function
OSi_RemoveMutexLinkFromQueue:
    ldr r3, [r0, #0x60]
    ldr r2, [r3, #0x10]
    cmp r2, #0
    moveq r1, #0
    streq r1, [r0, #0x64]
    movne r1, #0
    strne r1, [r2, #0x0C]
    str r2, [r0, #0x60]
    mov r0, r3
    bx lr
.size OSi_RemoveMutexLinkFromQueue, . - OSi_RemoveMutexLinkFromQueue

.global OSi_DequeueItem
.type OSi_DequeueItem, %function
OSi_DequeueItem:
    ldr r2, [r1, #0x10]
    ldr r1, [r1, #0x0C]
    cmp r2, #0
    streq r1, [r0, #0x64]
    strne r1, [r2, #0x0C]
    cmp r1, #0
    streq r2, [r0, #0x60]
    strne r2, [r1, #0x10]
    bx lr
.size OSi_DequeueItem, . - OSi_DequeueItem

.global OSi_EnqueueTail
.type OSi_EnqueueTail, %function
OSi_EnqueueTail:
    ldr r2, [r0, #0x64]
    cmp r2, #0
    streq r1, [r0, #0x60]
    strne r1, [r2, #0x10]
    str r2, [r1, #0x0C]
    mov r2, #0
    str r2, [r1, #0x10]
    str r1, [r0, #0x64]
    bx lr
.size OSi_EnqueueTail, . - OSi_EnqueueTail

.global OSi_UnlockAllMutex
.type OSi_UnlockAllMutex, %function
OSi_UnlockAllMutex:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, #0
    b .L_unlock_all_test
.L_unlock_next_mutex:
    mov r0, r5
    bl OSi_RemoveMutexLinkFromQueue
    str r4, [r0, #8]
    str r4, [r0, #4]
    bl OS_WakeupThread
.L_unlock_all_test:
    ldr r0, [r5, #0x60]
    cmp r0, #0
    bne .L_unlock_next_mutex
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.size OSi_UnlockAllMutex, . - OSi_UnlockAllMutex

.global OS_UnlockMutex
.type OS_UnlockMutex, %function
OS_UnlockMutex:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r0, .L_thread_info_unlock
    ldr r0, [r0, #8]
    ldr r1, [r5, #4]
    cmp r1, r0
    bne .L_unlock_mutex_done
    ldr r1, [r5, #8]
    sub r1, r1, #1
    str r1, [r5, #8]
    ldr r1, [r5, #8]
    cmp r1, #0
    bne .L_unlock_mutex_done
    mov r1, r5
    bl OSi_DequeueItem
    mov r0, #0
    str r0, [r5, #4]
    mov r0, r5
    bl OS_WakeupThread
.L_unlock_mutex_done:
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_thread_info_unlock:
    .word OSi_ThreadInfo
.size OS_UnlockMutex, . - OS_UnlockMutex

.global OS_LockMutex
.type OS_LockMutex, %function
OS_LockMutex:
    stmdb sp!, {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r5, r0
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r0, .L_thread_info_lock
    ldr r7, [r0, #8]
    mov r6, #0
.L_try_lock_mutex:
    ldr r0, [r5, #4]
    cmp r0, #0
    bne .L_mutex_owned
    str r7, [r5, #4]
    ldr r0, [r5, #8]
    add r0, r0, #1
    str r0, [r5, #8]
    mov r0, r7
    mov r1, r5
    bl OSi_EnqueueTail
    b .L_lock_mutex_done
.L_mutex_owned:
    cmp r0, r7
    ldreq r0, [r5, #8]
    addeq r0, r0, #1
    streq r0, [r5, #8]
    beq .L_lock_mutex_done
    str r5, [r7, #0x5C]
    mov r0, r5
    bl OS_SleepThread
    str r6, [r7, #0x5C]
    b .L_try_lock_mutex
.L_lock_mutex_done:
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
.L_thread_info_lock:
    .word OSi_ThreadInfo
.size OS_LockMutex, . - OS_LockMutex

.global OS_InitMutex
.type OS_InitMutex, %function
OS_InitMutex:
    mov r1, #0
    strh r1, [r0]
    str r1, [r0, #4]
    str r1, [r0, #8]
    bx lr
.size OS_InitMutex, . - OS_InitMutex
