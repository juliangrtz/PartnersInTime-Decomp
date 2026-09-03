/* NitroSDK ARM7 CPU fill and copy primitives. */

.syntax unified
.arch armv4t
.arm
.section .pit_MI_Memory, "ax", %progbits
.balign 4

.global MIi_CpuClear16
.type MIi_CpuClear16, %function
MIi_CpuClear16:
    mov r3, #0
.L_clear16_loop:
    cmp r3, r2
    strhlt r0, [r1, r3]
    addlt r3, r3, #2
    blt .L_clear16_loop
    bx lr
.size MIi_CpuClear16, . - MIi_CpuClear16

.global MIi_CpuCopy16
.type MIi_CpuCopy16, %function
MIi_CpuCopy16:
    mov r12, #0
.L_copy16_loop:
    cmp r12, r2
    ldrhlt r3, [r0, r12]
    strhlt r3, [r1, r12]
    addlt r12, r12, #2
    blt .L_copy16_loop
    bx lr
.size MIi_CpuCopy16, . - MIi_CpuCopy16

.global MIi_CpuClear32
.type MIi_CpuClear32, %function
MIi_CpuClear32:
    add r12, r1, r2
.L_clear32_loop:
    cmp r1, r12
    stmialt r1!, {r0}
    blt .L_clear32_loop
    bx lr
.size MIi_CpuClear32, . - MIi_CpuClear32

.global MIi_CpuCopy32
.type MIi_CpuCopy32, %function
MIi_CpuCopy32:
    add r12, r1, r2
.L_copy32_loop:
    cmp r1, r12
    ldmialt r0!, {r2}
    stmialt r1!, {r2}
    blt .L_copy32_loop
    bx lr
.size MIi_CpuCopy32, . - MIi_CpuCopy32

.global MIi_CpuClearFast
.type MIi_CpuClearFast, %function
MIi_CpuClearFast:
    push {r4, r5, r6, r7, r8, r9}
    add r9, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
    mov r2, r0
    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
.L_clear_fast_blocks:
    cmp r1, r12
    stmialt r1!, {r0, r2, r3, r4, r5, r6, r7, r8}
    blt .L_clear_fast_blocks
.L_clear_fast_tail:
    cmp r1, r9
    stmialt r1!, {r0}
    blt .L_clear_fast_tail
    pop {r4, r5, r6, r7, r8, r9}
    bx lr
.size MIi_CpuClearFast, . - MIi_CpuClearFast

.global MI_CpuFill8
.type MI_CpuFill8, %function
MI_CpuFill8:
    cmp r2, #0
    bxeq lr
    tst r0, #1
    beq .L_fill8_halfword_aligned
    ldrh r12, [r0, #-1]
    and r12, r12, #0xFF
    orr r3, r12, r1, lsl #8
    strh r3, [r0, #-1]
    add r0, r0, #1
    subs r2, r2, #1
    bxeq lr
.L_fill8_halfword_aligned:
    cmp r2, #2
    blo .L_fill8_last_byte
    orr r1, r1, r1, lsl #8
    tst r0, #2
    beq .L_fill8_word_aligned
    strh r1, [r0], #2
    subs r2, r2, #2
    bxeq lr
.L_fill8_word_aligned:
    orr r1, r1, r1, lsl #16
    bics r3, r2, #3
    beq .L_fill8_last_halfword
    sub r2, r2, r3
    add r12, r3, r0
.L_fill8_words:
    str r1, [r0], #4
    cmp r0, r12
    blo .L_fill8_words
.L_fill8_last_halfword:
    tst r2, #2
    strhne r1, [r0], #2
.L_fill8_last_byte:
    tst r2, #1
    bxeq lr
    ldrh r3, [r0]
    and r3, r3, #0xFF00
    and r1, r1, #0xFF
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
.size MI_CpuFill8, . - MI_CpuFill8

.global MI_CpuCopy8
.type MI_CpuCopy8, %function
MI_CpuCopy8:
    cmp r2, #0
    bxeq lr
    tst r1, #1
    beq .L_copy8_destination_aligned
    ldrh r12, [r1, #-1]
    and r12, r12, #0xFF
    tst r0, #1
    ldrhne r3, [r0, #-1]
    movne r3, r3, lsr #8
    ldrheq r3, [r0]
    orr r3, r12, r3, lsl #8
    strh r3, [r1, #-1]
    add r0, r0, #1
    add r1, r1, #1
    subs r2, r2, #1
    bxeq lr
.L_copy8_destination_aligned:
    eor r12, r1, r0
    tst r12, #1
    beq .L_copy8_same_halfword_alignment
    bic r0, r0, #1
    ldrh r12, [r0], #2
    mov r3, r12, lsr #8
    subs r2, r2, #2
    blo .L_copy8_misaligned_tail
.L_copy8_misaligned_halfwords:
    ldrh r12, [r0], #2
    orr r12, r3, r12, lsl #8
    strh r12, [r1], #2
    mov r3, r12, lsr #16
    subs r2, r2, #2
    bhs .L_copy8_misaligned_halfwords
.L_copy8_misaligned_tail:
    tst r2, #1
    bxeq lr
    ldrh r12, [r1]
    and r12, r12, #0xFF00
    orr r12, r12, r3
    strh r12, [r1]
    bx lr
.L_copy8_same_halfword_alignment:
    tst r12, #2
    beq .L_copy8_same_word_alignment
    bics r3, r2, #1
    beq .L_copy8_last_byte
    sub r2, r2, r3
    add r12, r3, r1
.L_copy8_halfwords:
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    cmp r1, r12
    blo .L_copy8_halfwords
    b .L_copy8_last_byte
.L_copy8_same_word_alignment:
    cmp r2, #2
    blo .L_copy8_last_byte
    tst r1, #2
    beq .L_copy8_words_aligned
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    subs r2, r2, #2
    bxeq lr
.L_copy8_words_aligned:
    bics r3, r2, #3
    beq .L_copy8_last_halfword
    sub r2, r2, r3
    add r12, r3, r1
.L_copy8_words:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, r12
    blo .L_copy8_words
.L_copy8_last_halfword:
    tst r2, #2
    ldrhne r3, [r0], #2
    strhne r3, [r1], #2
.L_copy8_last_byte:
    tst r2, #1
    bxeq lr
    ldrh r2, [r1]
    ldrh r0, [r0]
    and r2, r2, #0xFF00
    and r0, r0, #0xFF
    orr r0, r2, r0
    strh r0, [r1]
    bx lr
.size MI_CpuCopy8, . - MI_CpuCopy8
