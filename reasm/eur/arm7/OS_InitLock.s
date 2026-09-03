/* Initialize ARM7 NitroSDK lock-ID allocation and the shared init lock. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_InitLock, "ax", %progbits
.balign 4

.global OS_InitLock
.type OS_InitLock, %function
.extern HW_LOCK_ID_FLAG_SUBP
.extern HW_LOCK_INIT
.extern OSi_LockInitialized
.extern func_037FB9FC
OS_InitLock:
    stmdb sp!, {r4, lr}
    ldr r0, .L_lock_initialized
    ldr r1, [r0]
    cmp r1, #0
    bne .L_init_lock_done
    mov r1, #1
    str r1, [r0]
    mov r0, #0
    ldr r4, .L_init_lock
    strh r0, [r4, #6]
    b .L_wait_for_main
.L_wait:
    bl func_037FB9FC
.L_wait_for_main:
    ldrh r0, [r4, #4]
    cmp r0, #0x7F
    bne .L_wait
    mvn r1, #0
    ldr r0, .L_lock_id_flags_low
    str r1, [r0]
    mov r0, #0x10000
    rsb r1, r0, #0
    ldr r0, .L_lock_id_flags_high
    str r1, [r0]
    mov r0, #0xBF
    strh r0, [r4, #6]
.L_init_lock_done:
    ldmia sp!, {r4, lr}
    bx lr
.L_lock_initialized:
    .word OSi_LockInitialized
.L_init_lock:
    .word HW_LOCK_INIT
.L_lock_id_flags_low:
    .word HW_LOCK_ID_FLAG_SUBP
.L_lock_id_flags_high:
    .word HW_LOCK_ID_FLAG_SUBP + 4

.size OS_InitLock, . - OS_InitLock
