/* ARM7 Game Pak spin-lock wrappers and shared lock-word primitives. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_CartridgeLock, "ax", %progbits
.balign 4

.extern HW_LOCK_CARTRIDGE
.extern MI_SwapWord
.extern OS_DisableInterrupts
.extern OS_DisableInterrupts_IrqAndFiq
.extern OS_RestoreInterrupts
.extern OS_RestoreInterrupts_IrqAndFiq
.extern OS_SpinWait

.type OSi_LockWordSpinWait, %function
OSi_LockWordSpinWait:
    mov r0, #0x1000
    ldr r12, .L_spin_wait
    bx r12
.L_spin_wait:
    .word OS_SpinWait
.size OSi_LockWordSpinWait, . - OSi_LockWordSpinWait

.global OS_ReadOwnerOfLockWord
.type OS_ReadOwnerOfLockWord, %function
OS_ReadOwnerOfLockWord:
    ldrh r0, [r0, #4]
    bx lr
.size OS_ReadOwnerOfLockWord, . - OS_ReadOwnerOfLockWord

.type OSi_UnlockCartridgeCallback, %function
OSi_UnlockCartridgeCallback:
    bx lr
.size OSi_UnlockCartridgeCallback, . - OSi_UnlockCartridgeCallback

.type OSi_LockCartridgeCallback, %function
OSi_LockCartridgeCallback:
    bx lr
.size OSi_LockCartridgeCallback, . - OSi_LockCartridgeCallback

.global OS_TryLockCartridge
.type OS_TryLockCartridge, %function
OS_TryLockCartridge:
    ldr r1, .L_try_lock_word
    ldr r2, .L_try_lock_callback
    mov r3, #1
    ldr r12, .L_try_lock_core
    bx r12
.L_try_lock_word:
    .word HW_LOCK_CARTRIDGE
.L_try_lock_callback:
    .word OSi_LockCartridgeCallback
.L_try_lock_core:
    .word OSi_DoTryLockByWord
.size OS_TryLockCartridge, . - OS_TryLockCartridge

.global OS_UnlockCartridge
.type OS_UnlockCartridge, %function
OS_UnlockCartridge:
    ldr r1, .L_unlock_word
    ldr r2, .L_unlock_callback
    mov r3, #1
    ldr r12, .L_unlock_core
    bx r12
.L_unlock_word:
    .word HW_LOCK_CARTRIDGE
.L_unlock_callback:
    .word OSi_UnlockCartridgeCallback
.L_unlock_core:
    .word OSi_DoUnlockByWord
.size OS_UnlockCartridge, . - OS_UnlockCartridge

.global OS_LockCartridge
.type OS_LockCartridge, %function
OS_LockCartridge:
    ldr r1, .L_lock_word
    ldr r2, .L_lock_callback
    mov r3, #1
    ldr r12, .L_lock_core
    bx r12
.L_lock_word:
    .word HW_LOCK_CARTRIDGE
.L_lock_callback:
    .word OSi_LockCartridgeCallback
.L_lock_core:
    .word OSi_DoLockByWord
.size OS_LockCartridge, . - OS_LockCartridge

.global OSi_DoTryLockByWord
.type OSi_DoTryLockByWord, %function
OSi_DoTryLockByWord:
    stmdb sp!, {r4-r9, lr}
    sub sp, sp, #4
    mov r9, r0
    mov r8, r1
    mov r7, r2
    movs r6, r3
    beq .L_try_disable_irq
    bl OS_DisableInterrupts_IrqAndFiq
    mov r5, r0
    b .L_try_swap
.L_try_disable_irq:
    bl OS_DisableInterrupts
    mov r5, r0
.L_try_swap:
    mov r0, r9
    mov r1, r8
    bl MI_SwapWord
    movs r4, r0
    bne .L_try_restore
    cmp r7, #0
    beq .L_try_set_owner
    mov lr, pc
    bx r7
.L_try_set_owner:
    strh r9, [r8, #4]
.L_try_restore:
    cmp r6, #0
    beq .L_try_restore_irq
    mov r0, r5
    bl OS_RestoreInterrupts_IrqAndFiq
    b .L_try_return
.L_try_restore_irq:
    mov r0, r5
    bl OS_RestoreInterrupts
.L_try_return:
    mov r0, r4
    add sp, sp, #4
    ldmia sp!, {r4-r9, lr}
    bx lr
.size OSi_DoTryLockByWord, . - OSi_DoTryLockByWord

.global OSi_DoUnlockByWord
.type OSi_DoUnlockByWord, %function
OSi_DoUnlockByWord:
    stmdb sp!, {r4-r7, lr}
    sub sp, sp, #4
    mov r7, r1
    mov r6, r2
    mov r5, r3
    ldrh r1, [r7, #4]
    cmp r0, r1
    mvnne r0, #1
    bne .L_unlock_return
    cmp r5, #0
    beq .L_unlock_disable_irq
    bl OS_DisableInterrupts_IrqAndFiq
    mov r4, r0
    b .L_unlock_clear_owner
.L_unlock_disable_irq:
    bl OS_DisableInterrupts
    mov r4, r0
.L_unlock_clear_owner:
    mov r0, #0
    strh r0, [r7, #4]
    cmp r6, #0
    beq .L_unlock_clear_word
    mov lr, pc
    bx r6
.L_unlock_clear_word:
    mov r0, #0
    str r0, [r7]
    cmp r5, #0
    beq .L_unlock_restore_irq
    mov r0, r4
    bl OS_RestoreInterrupts_IrqAndFiq
    b .L_unlock_success
.L_unlock_restore_irq:
    mov r0, r4
    bl OS_RestoreInterrupts
.L_unlock_success:
    mov r0, #0
.L_unlock_return:
    add sp, sp, #4
    ldmia sp!, {r4-r7, lr}
    bx lr
.size OSi_DoUnlockByWord, . - OSi_DoUnlockByWord

.global OSi_DoLockByWord
.type OSi_DoLockByWord, %function
OSi_DoLockByWord:
    stmdb sp!, {r4-r7, lr}
    sub sp, sp, #4
    mov r7, r0
    mov r6, r1
    mov r5, r2
    mov r4, r3
    b .L_lock_try
.L_lock_wait:
    bl OSi_LockWordSpinWait
.L_lock_try:
    mov r0, r7
    mov r1, r6
    mov r2, r5
    mov r3, r4
    bl OSi_DoTryLockByWord
    cmp r0, #0
    bgt .L_lock_wait
    add sp, sp, #4
    ldmia sp!, {r4-r7, lr}
    bx lr
.size OSi_DoLockByWord, . - OSi_DoLockByWord
