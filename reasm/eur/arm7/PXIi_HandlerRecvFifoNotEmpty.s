/* ARM7 PXI receive-FIFO IRQ dispatcher. */

.syntax unified
.arch armv4t
.arm
.section .pit_PXIi_HandlerRecvFifoNotEmpty, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern PXIi_FifoRecvCallbackTable
.extern REG_PXI_FIFO_CNT
.extern REG_PXI_SEND_FIFO

.global PXIi_HandlerRecvFifoNotEmpty
.type PXIi_HandlerRecvFifoNotEmpty, %function
PXIi_HandlerRecvFifoNotEmpty:
    stmdb sp!, {r4-r10, lr}
    sub sp, sp, #8
    ldr r10, .L_fifo_control
    ldr r5, .L_callback_table
    mov r7, #0x04100000
    mov r6, #0
    mvn r8, #3
    mvn r9, #2
    ldr r4, .L_send_fifo

.L_receive_next:
    ldrh r0, [r10]
    ands r0, r0, #0x4000
    ldrhne r0, [r10]
    orrne r0, r0, #0xC000
    strhne r0, [r10]
    movne r1, r9
    bne .L_receive_result

    bl OS_DisableInterrupts
    ldrh r1, [r10]
    ands r1, r1, #0x100
    beq .L_read_word
    bl OS_RestoreInterrupts
    mov r1, r8
    b .L_receive_result
.L_read_word:
    ldr r1, [r7]
    str r1, [sp]
    bl OS_RestoreInterrupts
    mov r1, r6

.L_receive_result:
    cmp r1, r8
    beq .L_receive_done
    mvn r0, #2
    cmp r1, r0
    beq .L_receive_next

    ldr r2, [sp]
    mov r0, r2, lsl #27
    movs r0, r0, lsr #27
    beq .L_receive_next
    ldr r3, [r5, r0, lsl #2]
    cmp r3, #0
    beq .L_reply_unhandled
    mov r1, r2, lsr #6
    mov r2, r2, lsl #26
    mov r2, r2, lsr #31
    mov lr, pc
    bx r3
    b .L_receive_next

.L_reply_unhandled:
    mov r0, r2, lsl #26
    movs r0, r0, lsr #31
    bne .L_receive_next
    orr r0, r2, #0x20
    str r0, [sp]
    ldrh r0, [r10]
    ands r0, r0, #0x4000
    ldrhne r0, [r10]
    orrne r0, r0, #0xC000
    strhne r0, [r10]
    bne .L_receive_next

    bl OS_DisableInterrupts
    ldrh r1, [r10]
    ands r1, r1, #2
    beq .L_write_reply
    bl OS_RestoreInterrupts
    b .L_receive_next
.L_write_reply:
    ldr r1, [sp]
    str r1, [r4]
    bl OS_RestoreInterrupts
    b .L_receive_next

.L_receive_done:
    add sp, sp, #8
    ldmia sp!, {r4-r10, lr}
    bx lr
.L_fifo_control:
    .word REG_PXI_FIFO_CNT
.L_callback_table:
    .word PXIi_FifoRecvCallbackTable
.L_send_fifo:
    .word REG_PXI_SEND_FIFO
.size PXIi_HandlerRecvFifoNotEmpty, . - PXIi_HandlerRecvFifoNotEmpty
