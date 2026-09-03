/* Query the touch-controller and hinge register for pen state. */

.syntax unified
.arch armv4t
.arm
.section .pit_TP_CheckTouch, "ax", %progbits
.balign 4

.extern EXIi_SelectRcnt
.extern TPi_PenState

.global TPi_CheckTouch
.type TPi_CheckTouch, %function
TPi_CheckTouch:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r0, #0x8000
    bl EXIi_SelectRcnt
    ldr r1, .L_spicnt
.L_wait_before_enable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_before_enable
    ldr r0, .L_touch_enable
    strh r0, [r1]
    mov r1, #0x84
    ldr r0, .L_spidata
    strh r1, [r0]
    ldr r1, .L_spicnt
.L_wait_enable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_enable
    mov r1, #0
    ldr r0, .L_spidata
    strh r1, [r0]
    ldr r1, .L_spicnt
.L_wait_before_disable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_before_disable
    ldr r0, .L_touch_disable
    strh r0, [r1]
    mov r1, #0
    ldr r0, .L_spidata
    strh r1, [r0]
    ldr r1, .L_spicnt
.L_wait_disable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_disable
    ldr r0, .L_pen_state
    ldrh r0, [r0]
    cmp r0, #0
    bne .L_check_previous_pen
    ldr r0, .L_rcnt
    ldrh r0, [r0]
    ands r0, r0, #0x40
    moveq r0, #1
    movne r0, #0
    b .L_check_done
.L_check_previous_pen:
    ldr r0, .L_rcnt
    ldrh r0, [r0]
    ands r0, r0, #0x40
    moveq r0, #1
    beq .L_check_done
    ldr r0, .L_touch_enable
    strh r0, [r1]
    mov r1, #0x84
    ldr r0, .L_spidata
    strh r1, [r0]
    ldr r1, .L_spicnt
.L_wait_second_enable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_second_enable
    mov r1, #0
    ldr r0, .L_spidata
    strh r1, [r0]
    ldr r1, .L_spicnt
.L_wait_second_disable_command:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_second_disable_command
    ldr r0, .L_touch_disable
    strh r0, [r1]
    mov r1, #0
    ldr r0, .L_spidata
    strh r1, [r0]
    ldr r1, .L_spicnt
.L_wait_second_disable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_second_disable
    ldr r0, .L_rcnt
    ldrh r0, [r0]
    ands r0, r0, #0x40
    movne r0, #0
    moveq r0, #2
.L_check_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_spicnt:
    .word 0x040001c0
.L_touch_enable:
    .word 0x00008a01
.L_spidata:
    .word 0x040001c2
.L_touch_disable:
    .word 0x00008201
.L_pen_state:
    .word TPi_PenState
.L_rcnt:
    .word 0x04000136
.size TPi_CheckTouch, . - TPi_CheckTouch
