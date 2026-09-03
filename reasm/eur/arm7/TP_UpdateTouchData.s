/* Update pen-state debounce counters from a raw touch sample. */

.syntax unified
.arch armv4t
.arm
.section .pit_TP_UpdateTouchData, "ax", %progbits
.balign 4

.extern TPi_PenDownCounter
.extern TPi_PenUpCounter
.extern TPi_Work

.global TPi_UpdateTouchData
.type TPi_UpdateTouchData, %function
TPi_UpdateTouchData:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, [r0]
    mov r2, r0, lsl #7
    movs r2, r2, lsr #31
    bne .L_sample_present
    mov r1, #0
    ldr r0, .L_pen_down_counter
    strb r1, [r0]
    ldr r0, .L_pen_up_counter
    strb r1, [r0]
    b .L_update_done
.L_sample_present:
    mov r0, r0, lsl #5
    movs r0, r0, lsr #30
    beq .L_good_sample
    mov r2, #0
    ldr r0, .L_pen_up_counter
    strb r2, [r0]
    ldr r0, .L_pen_down_counter
    ldrb r1, [r0]
    add r1, r1, #1
    strb r1, [r0]
    ldrb r1, [r0]
    cmp r1, #4
    bcc .L_update_done
    strb r2, [r0]
    ldr r0, .L_touch_work
    ldr r1, [r0, #36]
    cmp r1, #35
    addlt r1, r1, #1
    strlt r1, [r0, #36]
    b .L_update_done
.L_good_sample:
    mov lr, #0
    ldr r3, .L_pen_down_counter
    strb lr, [r3]
    ldr r2, .L_touch_work
    ldr r12, [r2, #36]
    cmp r1, r12, asr #1
    ldrge r0, .L_pen_up_counter
    strbge lr, [r0]
    bge .L_update_done
    ldr r0, .L_pen_up_counter
    ldrb r1, [r0]
    add r1, r1, #1
    strb r1, [r0]
    ldrb r1, [r0]
    cmp r1, #4
    bcc .L_update_done
    strb lr, [r0]
    ldr r0, [r2, #40]
    cmp r12, r0
    subgt r0, r12, #1
    strgt r0, [r2, #36]
    movgt r0, #3
    strbgt r0, [r3]
.L_update_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_pen_down_counter:
    .word TPi_PenDownCounter
.L_pen_up_counter:
    .word TPi_PenUpCounter
.L_touch_work:
    .word TPi_Work
.size TPi_UpdateTouchData, . - TPi_UpdateTouchData
