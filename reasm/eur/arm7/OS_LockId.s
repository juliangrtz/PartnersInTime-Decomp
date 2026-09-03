/* ARM7 lock-ID allocation and release. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_LockId, "ax", %progbits
.balign 4

.extern HW_LOCK_ID_FLAG_SUBP

.global OS_GetLockID
.type OS_GetLockID, %function
OS_GetLockID:
    ldr r3, .L_lock_id_flags_get
    ldr r1, [r3]
    mov r2, #0
    mov r0, #0x80000000
.L_scan_low_word:
    tst r1, r0
    bne .L_low_scan_done
    add r2, r2, #1
    cmp r2, #32
    beq .L_low_scan_done
    mov r0, r0, lsr #1
    b .L_scan_low_word
.L_low_scan_done:
    cmp r2, #32
    movne r0, #0x80
    bne .L_allocate_id

    add r3, r3, #4
    ldr r1, [r3]
    mov r2, #0
    mov r0, #0x80000000
.L_scan_high_word:
    tst r1, r0
    bne .L_high_scan_done
    add r2, r2, #1
    cmp r2, #32
    beq .L_high_scan_done
    mov r0, r0, lsr #1
    b .L_scan_high_word
.L_high_scan_done:
    cmp r2, #32
    ldr r0, .L_lock_id_error
    bxeq lr
    mov r0, #0xA0
.L_allocate_id:
    add r0, r0, r2
    mov r1, #0x80000000
    mov r1, r1, lsr r2
    ldr r2, [r3]
    bic r2, r2, r1
    str r2, [r3]
    bx lr
.L_lock_id_flags_get:
    .word HW_LOCK_ID_FLAG_SUBP
.L_lock_id_error:
    .word -3
.size OS_GetLockID, . - OS_GetLockID

.global OS_ReleaseLockID
.type OS_ReleaseLockID, %function
OS_ReleaseLockID:
    ldr r3, .L_lock_id_flags_release
    cmp r0, #0xA0
    addpl r3, r3, #4
    subpl r0, r0, #0xA0
    submi r0, r0, #0x80
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3]
    orr r2, r2, r1
    str r2, [r3]
    bx lr
.L_lock_id_flags_release:
    .word HW_LOCK_ID_FLAG_SUBP
.size OS_ReleaseLockID, . - OS_ReleaseLockID
