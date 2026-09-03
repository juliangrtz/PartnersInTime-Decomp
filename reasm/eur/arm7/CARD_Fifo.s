/* Receive ARM9 CARD requests over PXI and wake the appropriate worker. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_Fifo, "ax", %progbits
.balign 4

.extern CARDi_Work
.extern OS_WakeupThreadDirect

.global CARDi_OnFifoRecv
.type CARDi_OnFifoRecv, %function
CARDi_OnFifoRecv:
    stmdb sp!, {lr}
    sub sp, sp, #4
    cmp r0, #0xb
    bne .L_fifo_done
    cmp r2, #0
    beq .L_fifo_done
    ldr r0, .L_card_work
    ldr r2, [r0, #8]
    cmp r2, #0
    streq r1, [r0, #4]
    ldr r2, [r0, #4]
    cmp r2, #0xc
    addls pc, pc, r2, lsl #2
    b .L_update_counter
    b .L_receive_init_word
    b .L_update_counter
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
    b .L_mark_request_ready
.L_receive_init_word:
    ldr r2, [r0, #8]
    cmp r2, #0
    beq .L_update_counter
    cmp r2, #1
    streq r1, [r0]
    ldreq r1, [r0, #0x38]
    orreq r1, r1, #0x10
    streq r1, [r0, #0x38]
    b .L_update_counter
.L_mark_request_ready:
    ldr r1, [r0, #0x38]
    orr r1, r1, #0x10
    str r1, [r0, #0x38]
.L_update_counter:
    ldr r1, [r0, #0x38]
    ands r1, r1, #0x10
    ldreq r1, [r0, #8]
    addeq r1, r1, #1
    streq r1, [r0, #8]
    beq .L_fifo_done
    mov r1, #0
    str r1, [r0, #8]
    ldr r1, [r0, #0x38]
    ands r1, r1, #4
    beq .L_wake_worker
    ldr r0, [r0, #0xb8]
    bl OS_WakeupThreadDirect
    b .L_fifo_done
.L_wake_worker:
    add r0, r0, #0x40
    bl OS_WakeupThreadDirect
.L_fifo_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_card_work:
    .word CARDi_Work
.size CARDi_OnFifoRecv, . - CARDi_OnFifoRecv
