/* Reserve, release, and stop ARM7 sound channels by channel bit mask. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_LockChannel, "ax", %progbits
.balign 4

.extern SND_FreeExChannel
.extern SND_StopChannel
.extern SNDi_Work
.extern sLockedChannelMask
.extern sWeakLockedChannelMask

.global SND_GetLockedChannel
.type SND_GetLockedChannel, %function
SND_GetLockedChannel:
    ands r0, r0, #1
    ldrne r0, .L_locked_mask_get
    ldrne r0, [r0]
    ldreq r0, .L_weak_locked_mask_get
    ldreq r0, [r0]
    bx lr
.size SND_GetLockedChannel, . - SND_GetLockedChannel
.L_locked_mask_get:
    .word sLockedChannelMask
.L_weak_locked_mask_get:
    .word sWeakLockedChannelMask

.global SND_UnlockChannel
.type SND_UnlockChannel, %function
SND_UnlockChannel:
    ands r1, r1, #1
    ldreq r1, .L_weak_locked_mask_unlock
    ldreq r2, [r1]
    mvneq r0, r0
    andeq r0, r2, r0
    streq r0, [r1]
    bxeq lr
    ldr r1, .L_locked_mask_unlock
    ldr r2, [r1]
    mvn r0, r0
    and r0, r2, r0
    str r0, [r1]
    bx lr
.size SND_UnlockChannel, . - SND_UnlockChannel
.L_weak_locked_mask_unlock:
    .word sWeakLockedChannelMask
.L_locked_mask_unlock:
    .word sLockedChannelMask

.global SND_LockChannel
.type SND_LockChannel, %function
SND_LockChannel:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r10, r0
    mov r9, r1
    mov r7, r10
    mov r6, #0
    mov r11, r6
    str r6, [sp]
    mov r4, r6
    mov r5, #1
    b .L_lock_channel_test
.L_lock_channel_loop:
    ands r0, r7, #1
    beq .L_lock_channel_next
    mov r1, #0x54
    ldr r0, .L_lock_sound_work
    mla r8, r6, r1, r0
    ldr r0, .L_weak_locked_mask_lock
    ldr r1, [r0]
    mov r0, r5, lsl r6
    ands r0, r1, r0
    bne .L_lock_channel_next
    ldr r3, [r8, #0x48]
    cmp r3, #0
    beq .L_lock_channel_stop
    mov r0, r8
    mov r1, r11
    ldr r2, [r8, #0x4C]
    mov lr, pc
    bx r3
.L_lock_channel_stop:
    mov r0, r6
    ldr r1, [sp]
    bl SND_StopChannel
    strb r4, [r8, #0x22]
    mov r0, r8
    bl SND_FreeExChannel
    ldrb r0, [r8, #3]
    bic r0, r0, #0xF8
    strb r0, [r8, #3]
    ldrb r0, [r8, #3]
    bic r0, r0, #1
    strb r0, [r8, #3]
.L_lock_channel_next:
    add r6, r6, #1
    mov r7, r7, lsr #1
.L_lock_channel_test:
    cmp r6, #0x10
    bge .L_lock_channel_record
    cmp r7, #0
    bne .L_lock_channel_loop
.L_lock_channel_record:
    ands r0, r9, #1
    ldrne r0, .L_locked_mask_lock
    ldrne r1, [r0]
    orrne r1, r1, r10
    strne r1, [r0]
    ldreq r0, .L_weak_locked_mask_lock
    ldreq r1, [r0]
    orreq r1, r1, r10
    streq r1, [r0]
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.size SND_LockChannel, . - SND_LockChannel
.L_lock_sound_work:
    .word SNDi_Work
.L_weak_locked_mask_lock:
    .word sWeakLockedChannelMask
.L_locked_mask_lock:
    .word sLockedChannelMask

.global SND_StopUnlockedChannel
.type SND_StopUnlockedChannel, %function
SND_StopUnlockedChannel:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r10, r0
    mov r8, #0
    mov r6, r8
    mov r5, r8
    mov r4, r8
    ldr r11, .L_stop_sound_work
    mov r7, #1
    b .L_stop_unlocked_test
.L_stop_unlocked_loop:
    ands r0, r10, #1
    beq .L_stop_unlocked_next
    mov r0, #0x54
    mla r9, r8, r0, r11
    ldr r0, .L_weak_locked_mask_stop
    ldr r1, [r0]
    mov r0, r7, lsl r8
    ands r0, r1, r0
    bne .L_stop_unlocked_next
    ldr r3, [r9, #0x48]
    cmp r3, #0
    beq .L_stop_unlocked_channel
    mov r0, r9
    mov r1, r6
    ldr r2, [r9, #0x4C]
    mov lr, pc
    bx r3
.L_stop_unlocked_channel:
    mov r0, r8
    mov r1, r5
    bl SND_StopChannel
    strb r4, [r9, #0x22]
    mov r0, r9
    bl SND_FreeExChannel
    ldrb r0, [r9, #3]
    bic r0, r0, #0xF8
    strb r0, [r9, #3]
    ldrb r0, [r9, #3]
    bic r0, r0, #1
    strb r0, [r9, #3]
.L_stop_unlocked_next:
    add r8, r8, #1
    mov r10, r10, lsr #1
.L_stop_unlocked_test:
    cmp r8, #0x10
    bge .L_stop_unlocked_done
    cmp r10, #0
    bne .L_stop_unlocked_loop
.L_stop_unlocked_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.size SND_StopUnlockedChannel, . - SND_StopUnlockedChannel
.L_stop_sound_work:
    .word SNDi_Work
.L_weak_locked_mask_stop:
    .word sWeakLockedChannelMask
