/* Perform a complete touch-panel sample and encode its status. */

.syntax unified
.arch armv4t
.arm
.section .pit_TP_ExecSampling, "ax", %progbits
.balign 4

.extern OS_Terminate
.extern TPi_CheckTouch
.extern TPi_PenState
.extern TPi_ReadChannel

.global TP_ExecSampling
.type TP_ExecSampling, %function
TP_ExecSampling:
    stmdb sp!, {r4-r7, lr}
    sub sp, sp, #12
    mov r7, r0
    movs r6, r1
    mov r5, r2
    mov r0, #0
    strh r0, [r5]
    rsbmi r6, r6, #0
    bl TPi_CheckTouch
    movs r4, r0
    bne .L_touch_present
    ldr r1, [r7]
    mov r0, #0x1000
    rsb r0, r0, #0
    and r0, r1, r0
    str r0, [r7]
    ldr r1, [r7]
    ldr r0, .L_coordinate_mask
    and r0, r1, r0
    str r0, [r7]
    ldr r0, [r7]
    bic r0, r0, #0x01000000
    str r0, [r7]
    ldr r0, [r7]
    bic r0, r0, #0x06000000
    orr r0, r0, #0x06000000
    str r0, [r7]
    mov r1, #0
    ldr r0, .L_pen_state
    strh r1, [r0]
    b .L_sample_done
.L_touch_present:
    add r0, sp, #0
    mov r1, r6
    mov r2, #0
    add r3, sp, #2
    bl TPi_ReadChannel
    ldr r1, [r7]
    bic r1, r1, #0x06000000
    and r0, r0, #3
    orr r0, r1, r0, lsl #25
    str r0, [r7]
    ldr r1, [r7]
    mov r0, #0x1000
    rsb r0, r0, #0
    and r2, r1, r0
    ldrh r1, [sp]
    ldr r0, .L_twelve_bit_mask
    and r0, r1, r0
    orr r0, r2, r0
    str r0, [r7]
    add r0, sp, #0
    mov r1, r6
    mov r2, #1
    add r3, sp, #4
    bl TPi_ReadChannel
    cmp r0, #2
    bne .L_store_y
    ldr r0, [r7]
    bic r1, r0, #0x06000000
    mov r0, r0, lsl #5
    mov r0, r0, lsr #30
    orr r0, r0, #2
    and r0, r0, #3
    orr r0, r1, r0, lsl #25
    str r0, [r7]
.L_store_y:
    ldr r1, [r7]
    ldr r0, .L_coordinate_mask
    and r2, r1, r0
    ldrh r1, [sp]
    ldr r0, .L_twelve_bit_mask
    and r0, r1, r0
    orr r0, r2, r0, lsl #12
    str r0, [r7]
    ldr r0, .L_touch_enable
    ldr r3, .L_spicnt
    strh r0, [r3]
    mov r6, #0
    mov r2, r6
    ldr r1, .L_spidata
.L_send_idle_byte:
    strh r2, [r1]
.L_wait_idle_byte:
    ldrh r0, [r3]
    ands r0, r0, #0x80
    bne .L_wait_idle_byte
    add r6, r6, #1
    cmp r6, #12
    blt .L_send_idle_byte
    ldr r0, .L_touch_disable
    strh r0, [r3]
    mov r1, #0
    ldr r0, .L_spidata
    strh r1, [r0]
    ldr r1, .L_spicnt
.L_wait_touch_disable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_touch_disable
    cmp r4, #2
    ldreq r0, [r7]
    biceq r0, r0, #0x06000000
    orreq r0, r0, #0x06000000
    streq r0, [r7]
    bl TPi_CheckTouch
    cmp r0, #0
    beq .L_touch_released
    cmp r0, #1
    beq .L_touch_held
    cmp r0, #2
    bne .L_invalid_touch_state
    ldr r0, [r7]
    orr r0, r0, #0x01000000
    str r0, [r7]
    ldr r0, [r7]
    bic r0, r0, #0x06000000
    orr r0, r0, #0x06000000
    str r0, [r7]
    mov r1, #0
    ldr r0, .L_pen_state
    strh r1, [r0]
    b .L_sample_done
.L_touch_held:
    ldr r0, [r7]
    orr r0, r0, #0x01000000
    str r0, [r7]
    mov r1, #1
    ldr r0, .L_pen_state
    strh r1, [r0]
    ldrh r0, [sp, #4]
    ldrh r1, [sp, #2]
    cmp r1, r0
    movcc r1, r0
    strh r1, [r5]
    b .L_sample_done
.L_touch_released:
    ldr r0, [r7]
    bic r0, r0, #0x01000000
    str r0, [r7]
    mov r1, #0
    ldr r0, .L_pen_state
    strh r1, [r0]
    b .L_sample_done
.L_invalid_touch_state:
    bl OS_Terminate
.L_sample_done:
    add sp, sp, #12
    ldmia sp!, {r4-r7, lr}
    bx lr
.L_coordinate_mask:
    .word 0xff000fff
.L_pen_state:
    .word TPi_PenState
.L_twelve_bit_mask:
    .word 0x00000fff
.L_touch_enable:
    .word 0x00008a01
.L_spicnt:
    .word 0x040001c0
.L_spidata:
    .word 0x040001c2
.L_touch_disable:
    .word 0x00008201
.size TP_ExecSampling, . - TP_ExecSampling
