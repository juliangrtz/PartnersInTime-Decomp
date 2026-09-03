/* ARM7 NitroSDK alarm queue and public alarm scheduling APIs. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Alarm, "ax", %progbits
.balign 4

.extern HW_INTR_CHECK_BUF
.extern OS_DisableInterrupts
.extern OS_DisableIrqMask
.extern OS_GetTick
.extern OS_RestoreInterrupts
.extern OS_Terminate
.extern OSi_AlarmQueue
.extern OSi_SetTimer
.extern REG_TM1CNT_H
.extern _ll_udiv

.global OSi_AlarmHandler
.type OSi_AlarmHandler, %function
OSi_AlarmHandler:
    stmdb sp!, {lr}
    bl OSi_ArrangeTimer
    ldmia sp!, {lr}
    bx lr
.size OSi_AlarmHandler, . - OSi_AlarmHandler

.global OSi_ArrangeTimer
.type OSi_ArrangeTimer, %function
OSi_ArrangeTimer:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r1, #0
    ldr r0, .L_timer1_control_arrange
    strh r1, [r0]
    mov r0, #0x10
    bl OS_DisableIrqMask
    ldr r1, .L_bios_irq_flags
    ldr r0, [r1]
    orr r0, r0, #0x10
    str r0, [r1]
    bl OS_GetTick
    ldr r2, .L_alarm_queue_arrange
    ldr r4, [r2]
    cmp r4, #0
    beq .L_arrange_done
    ldr r12, [r4, #0xC]
    ldr r3, [r4, #0x10]
    cmp r1, r3
    cmpeq r0, r12
    bhs .L_alarm_expired
    mov r0, r4
    bl OSi_SetTimer
    b .L_arrange_done

.L_alarm_expired:
    ldr r1, [r4, #0x18]
    str r1, [r2]
    cmp r1, #0
    moveq r0, #0
    streq r0, [r2, #4]
    movne r0, #0
    strne r0, [r1, #0x14]
    ldr r5, [r4]
    ldr r2, [r4, #0x1C]
    ldr r1, [r4, #0x20]
    mov r0, #0
    cmp r1, r0
    cmpeq r2, r0
    streq r0, [r4]
    cmp r5, #0
    beq .L_reinsert_periodic
    ldr r0, [r4, #4]
    mov lr, pc
    bx r5
.L_reinsert_periodic:
    ldr r2, [r4, #0x1C]
    ldr r0, [r4, #0x20]
    mov r1, #0
    cmp r0, r1
    cmpeq r2, r1
    beq .L_rearm_queue
    str r5, [r4]
    mov r0, r4
    mov r2, r1
    bl OSi_InsertAlarm
.L_rearm_queue:
    ldr r0, .L_alarm_queue_arrange
    ldr r0, [r0]
    cmp r0, #0
    beq .L_arrange_done
    bl OSi_SetTimer
.L_arrange_done:
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_timer1_control_arrange:
    .word REG_TM1CNT_H
.L_bios_irq_flags:
    .word HW_INTR_CHECK_BUF
.L_alarm_queue_arrange:
    .word OSi_AlarmQueue
.size OSi_ArrangeTimer, . - OSi_ArrangeTimer

.global OS_CancelAlarm
.type OS_CancelAlarm, %function
OS_CancelAlarm:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r1, [r5]
    cmp r1, #0
    bne .L_cancel_linked
    bl OS_RestoreInterrupts
    b .L_cancel_done
.L_cancel_linked:
    ldr r0, [r5, #0x18]
    cmp r0, #0
    ldreq r2, [r5, #0x14]
    ldreq r1, .L_alarm_queue_cancel
    streq r2, [r1, #4]
    ldrne r1, [r5, #0x14]
    strne r1, [r0, #0x14]
    ldr r1, [r5, #0x14]
    cmp r1, #0
    strne r0, [r1, #0x18]
    bne .L_cancel_clear
    ldr r1, .L_alarm_queue_cancel
    str r0, [r1]
    cmp r0, #0
    beq .L_cancel_clear
    bl OSi_SetTimer
.L_cancel_clear:
    mov r0, #0
    str r0, [r5]
    str r0, [r5, #0x1C]
    str r0, [r5, #0x20]
    mov r0, r4
    bl OS_RestoreInterrupts
.L_cancel_done:
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_alarm_queue_cancel:
    .word OSi_AlarmQueue
.size OS_CancelAlarm, . - OS_CancelAlarm

.global OS_SetPeriodicAlarm
.type OS_SetPeriodicAlarm, %function
OS_SetPeriodicAlarm:
    stmdb sp!, {r4-r9, lr}
    sub sp, sp, #4
    movs r4, r0
    mov r9, r1
    mov r8, r2
    mov r7, r3
    ldr r6, [sp, #0x20]
    beq .L_periodic_invalid
    ldr r0, [r4]
    cmp r0, #0
    beq .L_periodic_schedule
.L_periodic_invalid:
    bl OS_Terminate
.L_periodic_schedule:
    bl OS_DisableInterrupts
    mov r5, r0
    str r7, [r4, #0x1C]
    str r6, [r4, #0x20]
    str r9, [r4, #0x24]
    str r8, [r4, #0x28]
    ldr r0, [sp, #0x24]
    str r0, [r4]
    ldr r0, [sp, #0x28]
    str r0, [r4, #4]
    mov r0, r4
    mov r1, #0
    mov r2, r1
    bl OSi_InsertAlarm
    mov r0, r5
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmia sp!, {r4-r9, lr}
    bx lr
.size OS_SetPeriodicAlarm, . - OS_SetPeriodicAlarm

.global OS_SetAlarm
.type OS_SetAlarm, %function
OS_SetAlarm:
    stmdb sp!, {r4-r8, lr}
    movs r7, r0
    mov r6, r1
    mov r5, r2
    mov r8, r3
    beq .L_alarm_invalid
    ldr r0, [r7]
    cmp r0, #0
    beq .L_alarm_schedule
.L_alarm_invalid:
    bl OS_Terminate
.L_alarm_schedule:
    bl OS_DisableInterrupts
    mov r4, r0
    mov r0, #0
    str r0, [r7, #0x1C]
    str r0, [r7, #0x20]
    str r8, [r7]
    ldr r0, [sp, #0x18]
    str r0, [r7, #4]
    bl OS_GetTick
    mov r2, r0
    mov r0, r7
    adds r3, r6, r2
    adc r2, r5, r1
    mov r1, r3
    bl OSi_InsertAlarm
    mov r0, r4
    bl OS_RestoreInterrupts
    ldmia sp!, {r4-r8, lr}
    bx lr
.size OS_SetAlarm, . - OS_SetAlarm

.global OSi_InsertAlarm
.type OSi_InsertAlarm, %function
OSi_InsertAlarm:
    stmdb sp!, {r4-r8, lr}
    mov r8, r0
    mov r7, r1
    mov r6, r2
    ldr r2, [r8, #0x1C]
    ldr r1, [r8, #0x20]
    mov r0, #0
    cmp r1, r0
    cmpeq r2, r0
    beq .L_store_fire_tick
    bl OS_GetTick
    ldr r7, [r8, #0x24]
    ldr r6, [r8, #0x28]
    cmp r6, r1
    cmpeq r7, r0
    bhs .L_store_fire_tick
    ldr r5, [r8, #0x1C]
    ldr r4, [r8, #0x20]
    subs r0, r0, r7
    sbc r1, r1, r6
    mov r2, r5
    mov r3, r4
    bl _ll_udiv
    mov r2, #1
    adds r2, r0, r2
    adc r0, r1, #0
    umull r3, r1, r5, r2
    mla r1, r5, r0, r1
    mla r1, r4, r2, r1
    adds r7, r7, r3
    adc r6, r6, r1
.L_store_fire_tick:
    str r7, [r8, #0xC]
    str r6, [r8, #0x10]
    ldr r0, .L_alarm_queue_insert
    ldr r4, [r0]
    mov r1, #0
    b .L_insert_test
.L_insert_before:
    ldr r2, [r4, #0xC]
    ldr r0, [r4, #0x10]
    subs r3, r7, r2
    sbc r2, r6, r0
    subs r0, r3, r1
    sbcs r0, r2, r1
    bge .L_insert_advance
    ldr r0, [r4, #0x14]
    str r0, [r8, #0x14]
    str r8, [r4, #0x14]
    str r4, [r8, #0x18]
    ldr r0, [r8, #0x14]
    cmp r0, #0
    strne r8, [r0, #0x18]
    bne .L_insert_done
    ldr r0, .L_alarm_queue_insert
    str r8, [r0]
    mov r0, r8
    bl OSi_SetTimer
    b .L_insert_done
.L_insert_advance:
    ldr r4, [r4, #0x18]
.L_insert_test:
    cmp r4, #0
    bne .L_insert_before
    mov r0, #0
    str r0, [r8, #0x18]
    ldr r0, .L_alarm_queue_insert
    ldr r1, [r0, #4]
    str r8, [r0, #4]
    str r1, [r8, #0x14]
    cmp r1, #0
    strne r8, [r1, #0x18]
    bne .L_insert_done
    str r8, [r0, #4]
    str r8, [r0]
    mov r0, r8
    bl OSi_SetTimer
.L_insert_done:
    ldmia sp!, {r4-r8, lr}
    bx lr
.L_alarm_queue_insert:
    .word OSi_AlarmQueue
.size OSi_InsertAlarm, . - OSi_InsertAlarm
