/* Initialize touch-panel state, VAlarms, and SPI control registers. */

.syntax unified
.arch armv4t
.arm
.section .pit_TP_Init, "ax", %progbits
.balign 4

.extern OS_CreateVAlarm
.extern OS_InitVAlarm
.extern OS_IsVAlarmAvailable
.extern OS_SetVAlarmTag
.extern TPi_VAlarmArray
.extern TPi_Work

.global TP_Init
.type TP_Init, %function
TP_Init:
    stmdb sp!, {r4-r8, lr}
    mov r3, #0
    ldr r0, .L_touch_work
    str r3, [r0, #32]
    mov r1, #20
    str r1, [r0, #36]
    str r1, [r0, #40]
    mov r2, r3
.L_clear_command_fragments:
    mov r1, r3, lsl #1
    strh r2, [r0, r1]
    add r3, r3, #1
    cmp r3, #16
    blt .L_clear_command_fragments
    bl OS_IsVAlarmAvailable
    cmp r0, #0
    bne .L_create_alarms
    bl OS_InitVAlarm
.L_create_alarms:
    mov r7, #0
    ldr r6, .L_valarm_array
    ldr r5, .L_valarm_tag
    mov r4, #40
.L_create_alarm:
    mla r8, r7, r4, r6
    mov r0, r8
    bl OS_CreateVAlarm
    mov r0, r8
    mov r1, r5
    bl OS_SetVAlarmTag
    add r7, r7, #1
    cmp r7, #4
    blt .L_create_alarm
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
.L_wait_for_enable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_for_enable
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
.L_wait_for_disable:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_wait_for_disable
    ldmia sp!, {r4-r8, lr}
    bx lr
.L_touch_work:
    .word TPi_Work
.L_valarm_array:
    .word TPi_VAlarmArray
.L_valarm_tag:
    .word 0x54505641
.L_spicnt:
    .word 0x040001c0
.L_touch_enable:
    .word 0x00008a01
.L_spidata:
    .word 0x040001c2
.L_touch_disable:
    .word 0x00008201
.size TP_Init, . - TP_Init
