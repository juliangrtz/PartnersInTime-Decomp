/* Read five touch-controller samples and choose a stable channel value. */

.syntax unified
.arch armv4t
.arm
.section .pit_TP_ReadChannel, "ax", %progbits
.balign 4

.global TPi_ReadChannel
.type TPi_ReadChannel, %function
TPi_ReadChannel:
    stmdb sp!, {r4-r10, lr}
    sub sp, sp, #24
    cmp r2, #0
    moveq r5, #0xd1
    moveq r6, #1
    movne r5, #0x91
    movne r6, #2
.L_wait_initial:
    ldr r4, .L_spicnt
    ldrh r2, [r4]
    ands r2, r2, #0x80
    bne .L_wait_initial
    ldr r2, .L_touch_enable
    strh r2, [r4]
    and r5, r5, #0xff
    ldr r2, .L_spidata
    strh r5, [r2]
.L_wait_command:
    ldrh r2, [r4]
    ands r2, r2, #0x80
    bne .L_wait_command
    mov r7, #0
    ldr r12, .L_spicnt
    ldr lr, .L_spidata
    add r2, sp, #0
    mov r4, r7
    ldr r9, .L_sample_mask
.L_read_sample:
    strh r4, [lr]
.L_wait_high_byte:
    ldrh r8, [r12]
    ands r8, r8, #0x80
    bne .L_wait_high_byte
    ldrh r8, [lr]
    and r8, r8, #0xff
    mov r8, r8, lsl #16
    mov r8, r8, lsr #8
    str r8, [r2, r7, lsl #2]
    strh r5, [lr]
.L_wait_low_byte:
    ldrh r8, [r12]
    ands r8, r8, #0x80
    bne .L_wait_low_byte
    ldrh r8, [lr]
    and r8, r8, #0xff
    mov r8, r8, lsl #16
    ldr r10, [r2, r7, lsl #2]
    orr r8, r10, r8, lsr #16
    str r8, [r2, r7, lsl #2]
    ldr r8, [r2, r7, lsl #2]
    and r8, r8, r9
    mov r8, r8, asr #3
    str r8, [r2, r7, lsl #2]
    add r7, r7, #1
    cmp r7, #5
    blt .L_read_sample
    ldr r2, .L_touch_disable
    strh r2, [r12]
    mov r2, #0
    strh r2, [lr]
    ldr r4, .L_spicnt
.L_wait_disable:
    ldrh r2, [r4]
    ands r2, r2, #0x80
    bne .L_wait_disable
    mov r8, #0
    mov r7, r8
    add r5, sp, #0
.L_max_outer:
    add r9, r7, #1
    ldr r4, [r5, r7, lsl #2]
    b .L_max_test
.L_max_inner:
    ldr r2, [r5, r9, lsl #2]
    subs r2, r4, r2
    rsbmi r2, r2, #0
    cmp r2, r8
    movgt r8, r2
    add r9, r9, #1
.L_max_test:
    cmp r9, #5
    blt .L_max_inner
    add r7, r7, #1
    cmp r7, #4
    blt .L_max_outer
    strh r8, [r3]
    mov r4, #0
    add r2, sp, #0
    b .L_stable_outer_test
.L_stable_outer:
    add r3, r4, #1
    ldr r7, [r2, r4, lsl #2]
    b .L_stable_middle_test
.L_stable_middle:
    ldr r8, [r2, r3, lsl #2]
    subs r5, r7, r8
    rsbmi r5, r5, #0
    cmp r5, r1
    bgt .L_next_middle
    add r9, r3, #1
    b .L_stable_inner_test
.L_stable_inner:
    ldr r5, [r2, r9, lsl #2]
    subs r10, r7, r5
    rsbmi r10, r10, #0
    cmp r10, r1
    bgt .L_next_inner
    add r1, r8, r7, lsl #1
    add r1, r5, r1
    mov r1, r1, asr #2
    bic r1, r1, #7
    strh r1, [r0]
    mov r0, #0
    b .L_channel_done
.L_next_inner:
    add r9, r9, #1
.L_stable_inner_test:
    cmp r9, #5
    blt .L_stable_inner
.L_next_middle:
    add r3, r3, #1
.L_stable_middle_test:
    cmp r3, #4
    blt .L_stable_middle
    add r4, r4, #1
.L_stable_outer_test:
    cmp r4, #3
    blt .L_stable_outer
    ldr r2, [sp]
    ldr r1, [sp, #16]
    add r1, r2, r1
    mov r1, r1, asr #1
    bic r1, r1, #7
    strh r1, [r0]
    mov r0, r6
.L_channel_done:
    add sp, sp, #24
    ldmia sp!, {r4-r10, lr}
    bx lr
.L_spicnt:
    .word 0x040001c0
.L_touch_enable:
    .word 0x00008a01
.L_spidata:
    .word 0x040001c2
.L_sample_mask:
    .word 0x00007ff8
.L_touch_disable:
    .word 0x00008201
.size TPi_ReadChannel, . - TPi_ReadChannel
