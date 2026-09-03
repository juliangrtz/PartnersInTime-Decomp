/* NitroSDK V-count alarm scheduling and display-status IRQ integration. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_VAlarm, "ax", %progbits
.balign 4

.extern HW_INTR_CHECK_BUF
.extern OS_DisableInterrupts
.extern OS_DisableIrqMask
.extern OS_EnableIrqMask
.extern OS_ResetRequestIrqMask
.extern OS_RestoreInterrupts
.extern OS_SetIrqFunction
.extern OS_Terminate
.extern OSi_LastVCount
.extern OSi_VAlarmInitialized
.extern OSi_VAlarmQueue
.extern OSi_VFrameCount
.extern REG_DISPSTAT
.extern REG_VCOUNT

.global OSi_GetVFrame
.type OSi_GetVFrame, %function
OSi_GetVFrame:
    push {r4, lr}
    mov r4, r0
    bl OS_DisableInterrupts
    ldr r1, .L_last_vcount_get
    ldr r1, [r1]
    cmp r4, r1
    ldrlt r1, .L_vframe_count_get
    ldrlt r2, [r1]
    addlt r2, r2, #1
    strlt r2, [r1]
    ldr r1, .L_last_vcount_get
    str r4, [r1]
    bl OS_RestoreInterrupts
    ldr r0, .L_vframe_count_get
    ldr r0, [r0]
    pop {r4, lr}
    bx lr
.size OSi_GetVFrame, . - OSi_GetVFrame
.L_last_vcount_get:
    .word OSi_LastVCount
.L_vframe_count_get:
    .word OSi_VFrameCount

.global OSi_CompareVCount
.type OSi_CompareVCount, %function
OSi_CompareVCount:
    ldr r3, [r0, #0x0C]
    subs r3, r1, r3
    ldrsh r1, [r0, #0x10]
    sub r2, r2, r1
    bmi .L_compare_before
    cmp r3, #0
    bne .L_compare_window
    cmp r2, #0
    bge .L_compare_window
.L_compare_before:
    mov r0, #0
    bx lr
.L_compare_window:
    cmp r2, #0
    ldrlt r1, .L_scanline_count
    addlt r2, r2, r1
    ldrsh r0, [r0, #0x12]
    cmp r2, r0
    movle r0, #1
    movgt r0, #2
    bx lr
.size OSi_CompareVCount, . - OSi_CompareVCount
.L_scanline_count:
    .word 0x00000107

.global OSi_VAlarmHandler
.type OSi_VAlarmHandler, %function
OSi_VAlarmHandler:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    mov r0, #4
    bl OS_DisableIrqMask
    ldr r2, .L_dispstat_handler
    ldrh r0, [r2]
    bic r0, r0, #0x20
    strh r0, [r2]
    ldr r1, .L_irq_check_handler
    ldr r0, [r1]
    orr r0, r0, #4
    str r0, [r1]
    ldrh r2, [r2]
    mov r0, r2, asr #8
    and r1, r0, #0xFF
    mov r0, r2, lsl #1
    and r0, r0, #0x100
    orr r0, r1, r0
    sub r0, r0, #1
    bl OSi_GetVFrame
    ldr r9, .L_vcount_handler
    mov r6, #4
    ldr r5, .L_dispstat_handler
    mov r4, #0
    ldr r11, .L_vframe_count_handler
    b .L_handler_next
.L_handler_loop:
    ldrh r8, [r9]
    mov r0, r8
    bl OSi_GetVFrame
    mov r7, r0
    mov r0, r10
    mov r1, r7
    mov r2, r8
    bl OSi_CompareVCount
    cmp r0, #0
    beq .L_handler_before
    cmp r0, #1
    beq .L_handler_fire
    cmp r0, #2
    beq .L_handler_late
    b .L_handler_next
.L_handler_before:
    mov r0, r10
    bl OSi_SetNextVAlarm
    ldrh r1, [r9]
    ldrsh r0, [r10, #0x10]
    cmp r0, r1
    bne .L_handler_done
    ldr r0, [r10, #0x0C]
    cmp r0, r7
    bne .L_handler_done
    mov r0, r6
    bl OS_DisableIrqMask
    ldrh r0, [r5]
    bic r0, r0, #0x20
    strh r0, [r5]
    mov r0, r6
    bl OS_ResetRequestIrqMask
.L_handler_fire:
    ldr r7, [r10]
    mov r0, r10
    bl OSi_DetachVAlarm
    str r4, [r10]
    cmp r7, #0
    beq .L_handler_periodic
    ldr r0, [r10, #4]
    mov lr, pc
    bx r7
.L_handler_periodic:
    ldr r0, [r10, #0x1C]
    cmp r0, #0
    beq .L_handler_next
    ldr r0, [r10, #0x24]
    cmp r0, #0
    bne .L_handler_next
    str r7, [r10]
    ldr r0, [r11]
    add r0, r0, #1
    str r0, [r10, #0x0C]
    mov r0, r10
    bl OSi_AppendVAlarm
    b .L_handler_next
.L_handler_late:
    mov r0, r10
    bl OSi_DetachVAlarm
    mov r0, r10
    bl OSi_AppendVAlarm
    ldr r0, [r11]
    add r0, r0, #1
    str r0, [r10, #0x0C]
.L_handler_next:
    ldr r0, .L_valarm_queue_handler
    ldr r10, [r0]
    cmp r10, #0
    bne .L_handler_loop
.L_handler_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
.size OSi_VAlarmHandler, . - OSi_VAlarmHandler
.L_dispstat_handler:
    .word REG_DISPSTAT
.L_irq_check_handler:
    .word HW_INTR_CHECK_BUF
.L_vcount_handler:
    .word REG_VCOUNT
.L_vframe_count_handler:
    .word OSi_VFrameCount
.L_valarm_queue_handler:
    .word OSi_VAlarmQueue

.global OS_CancelVAlarms
.type OS_CancelVAlarms, %function
OS_CancelVAlarms:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r0
    bl OS_DisableInterrupts
    mov r5, r0
    cmp r7, #0
    bne .L_cancel_all_valid
    bl OS_Terminate
.L_cancel_all_valid:
    ldr r0, .L_valarm_queue_cancel_all
    ldr r0, [r0]
    cmp r0, #0
    ldrne r6, [r0, #0x18]
    moveq r6, #0
    mov r4, #0
    b .L_cancel_all_test
.L_cancel_all_loop:
    ldr r1, [r0, #8]
    cmp r1, r7
    bne .L_cancel_all_next
    bl OS_CancelVAlarm
.L_cancel_all_next:
    mov r0, r6
    cmp r6, #0
    ldrne r6, [r6, #0x18]
    moveq r6, r4
.L_cancel_all_test:
    cmp r0, #0
    bne .L_cancel_all_loop
    mov r0, r5
    bl OS_RestoreInterrupts
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size OS_CancelVAlarms, . - OS_CancelVAlarms
.L_valarm_queue_cancel_all:
    .word OSi_VAlarmQueue

.global OS_CancelVAlarm
.type OS_CancelVAlarm, %function
OS_CancelVAlarm:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    bl OS_DisableInterrupts
    mov r4, r0
    mov r1, #1
    str r1, [r5, #0x24]
    ldr r1, [r5]
    cmp r1, #0
    bne .L_cancel_attached
    bl OS_RestoreInterrupts
    b .L_cancel_return
.L_cancel_attached:
    mov r0, r5
    bl OSi_DetachVAlarm
    mov r0, #0
    str r0, [r5]
    mov r0, r4
    bl OS_RestoreInterrupts
.L_cancel_return:
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size OS_CancelVAlarm, . - OS_CancelVAlarm

.global OS_SetVAlarmTag
.type OS_SetVAlarmTag, %function
OS_SetVAlarmTag:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    movs r4, r1
    bne .L_tag_valid
    bl OS_Terminate
.L_tag_valid:
    cmp r5, #0
    strne r4, [r5, #8]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size OS_SetVAlarmTag, . - OS_SetVAlarmTag

.global OSi_SetNextVAlarm
.type OSi_SetNextVAlarm, %function
OSi_SetNextVAlarm:
    push {r4, lr}
    mov r4, r0
    mov r0, #4
    ldr r1, .L_valarm_handler_next
    bl OS_SetIrqFunction
    ldrsh r12, [r4, #0x10]
    and r3, r12, #0x100
    ldr r1, .L_dispstat_next
    ldrh r0, [r1]
    and r2, r0, #0x3F
    and r0, r12, #0xFF
    orr r0, r2, r0, lsl #8
    orr r0, r0, r3, asr #1
    strh r0, [r1]
    ldrh r0, [r1]
    orr r0, r0, #0x20
    strh r0, [r1]
    mov r0, #4
    bl OS_EnableIrqMask
    pop {r4, lr}
    bx lr
.size OSi_SetNextVAlarm, . - OSi_SetNextVAlarm
.L_valarm_handler_next:
    .word OSi_VAlarmHandler
.L_dispstat_next:
    .word REG_DISPSTAT

.global OS_SetPeriodicVAlarm
.type OS_SetPeriodicVAlarm, %function
OS_SetPeriodicVAlarm:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r8, r0
    mov r7, r1
    mov r6, r2
    mov r5, r3
    bl OS_DisableInterrupts
    mov r4, r0
    cmp r8, #0
    beq .L_periodic_invalid
    ldr r0, [r8]
    cmp r0, #0
    beq .L_periodic_valid
.L_periodic_invalid:
    bl OS_Terminate
.L_periodic_valid:
    ldr r0, .L_vcount_periodic
    ldrh r9, [r0]
    mov r0, r9
    bl OSi_GetVFrame
    mov r1, #1
    str r1, [r8, #0x1C]
    strh r7, [r8, #0x10]
    cmp r7, r9
    addle r0, r0, #1
    str r0, [r8, #0x0C]
    strh r6, [r8, #0x12]
    str r5, [r8]
    ldr r0, [sp, #0x20]
    str r0, [r8, #4]
    mov r0, #0
    str r0, [r8, #0x24]
    mov r0, r8
    bl OSi_InsertVAlarm
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.size OS_SetPeriodicVAlarm, . - OS_SetPeriodicVAlarm
.L_vcount_periodic:
    .word REG_VCOUNT

.global OS_SetVAlarm
.type OS_SetVAlarm, %function
OS_SetVAlarm:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r8, r0
    mov r7, r1
    mov r6, r2
    mov r5, r3
    bl OS_DisableInterrupts
    mov r4, r0
    cmp r8, #0
    beq .L_set_invalid
    ldr r0, [r8]
    cmp r0, #0
    beq .L_set_valid
.L_set_invalid:
    bl OS_Terminate
.L_set_valid:
    ldr r0, .L_vcount_set
    ldrh r9, [r0]
    mov r0, r9
    bl OSi_GetVFrame
    mov r1, #0
    str r1, [r8, #0x1C]
    strh r7, [r8, #0x10]
    cmp r7, r9
    addle r0, r0, #1
    str r0, [r8, #0x0C]
    strh r6, [r8, #0x12]
    str r5, [r8]
    ldr r0, [sp, #0x20]
    str r0, [r8, #4]
    mov r0, #0
    str r0, [r8, #0x24]
    mov r0, r8
    bl OSi_InsertVAlarm
    mov r0, r4
    bl OS_RestoreInterrupts
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.size OS_SetVAlarm, . - OS_SetVAlarm
.L_vcount_set:
    .word REG_VCOUNT

.global OS_CreateVAlarm
.type OS_CreateVAlarm, %function
OS_CreateVAlarm:
    mov r1, #0
    str r1, [r0]
    str r1, [r0, #8]
    str r1, [r0, #0x20]
    bx lr
.size OS_CreateVAlarm, . - OS_CreateVAlarm

.global OSi_DetachVAlarm
.type OSi_DetachVAlarm, %function
OSi_DetachVAlarm:
    cmp r0, #0
    bxeq lr
    ldr r2, [r0, #0x14]
    ldr r1, [r0, #0x18]
    cmp r1, #0
    strne r2, [r1, #0x14]
    ldreq r0, .L_valarm_queue_detach
    streq r2, [r0, #4]
    cmp r2, #0
    strne r1, [r2, #0x18]
    ldreq r0, .L_valarm_queue_detach
    streq r1, [r0]
    bx lr
.size OSi_DetachVAlarm, . - OSi_DetachVAlarm
.L_valarm_queue_detach:
    .word OSi_VAlarmQueue

.global OSi_AppendVAlarm
.type OSi_AppendVAlarm, %function
OSi_AppendVAlarm:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_valarm_queue_append
    ldr r3, [r1, #4]
    str r3, [r0, #0x14]
    mov r2, #0
    str r2, [r0, #0x18]
    str r0, [r1, #4]
    cmp r3, #0
    strne r0, [r3, #0x18]
    bne .L_append_return
    str r0, [r1]
    bl OSi_SetNextVAlarm
.L_append_return:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size OSi_AppendVAlarm, . - OSi_AppendVAlarm
.L_valarm_queue_append:
    .word OSi_VAlarmQueue

.global OSi_InsertVAlarm
.type OSi_InsertVAlarm, %function
OSi_InsertVAlarm:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_valarm_queue_insert
    ldr r3, [r1]
    b .L_insert_test
.L_insert_loop:
    ldr r2, [r0, #0x0C]
    ldr r1, [r3, #0x0C]
    cmp r1, r2
    blo .L_insert_next
    cmp r1, r2
    bne .L_insert_here
    ldrsh r2, [r3, #0x10]
    ldrsh r1, [r0, #0x10]
    cmp r2, r1
    ble .L_insert_next
.L_insert_here:
    ldr r1, [r3, #0x14]
    str r1, [r0, #0x14]
    str r3, [r0, #0x18]
    str r0, [r3, #0x14]
    cmp r1, #0
    strne r0, [r1, #0x18]
    bne .L_insert_return
    ldr r1, .L_valarm_queue_insert
    str r0, [r1]
    bl OSi_SetNextVAlarm
    b .L_insert_return
.L_insert_next:
    ldr r3, [r3, #0x18]
.L_insert_test:
    cmp r3, #0
    bne .L_insert_loop
    bl OSi_AppendVAlarm
.L_insert_return:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size OSi_InsertVAlarm, . - OSi_InsertVAlarm
.L_valarm_queue_insert:
    .word OSi_VAlarmQueue

.global OS_IsVAlarmAvailable
.type OS_IsVAlarmAvailable, %function
OS_IsVAlarmAvailable:
    ldr r0, .L_valarm_initialized_available
    ldrh r0, [r0]
    bx lr
.size OS_IsVAlarmAvailable, . - OS_IsVAlarmAvailable
.L_valarm_initialized_available:
    .word OSi_VAlarmInitialized

.global OS_InitVAlarm
.type OS_InitVAlarm, %function
OS_InitVAlarm:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_valarm_initialized_init
    ldrh r1, [r0]
    cmp r1, #0
    bne .L_init_return
    mov r1, #1
    strh r1, [r0]
    mov r1, #0
    ldr r0, .L_valarm_queue_init
    str r1, [r0]
    str r1, [r0, #4]
    mov r0, #4
    bl OS_DisableIrqMask
    mov r1, #0
    ldr r0, .L_vframe_count_init
    str r1, [r0]
    ldr r0, .L_last_vcount_init
    str r1, [r0]
.L_init_return:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size OS_InitVAlarm, . - OS_InitVAlarm
.L_valarm_initialized_init:
    .word OSi_VAlarmInitialized
.L_valarm_queue_init:
    .word OSi_VAlarmQueue
.L_vframe_count_init:
    .word OSi_VFrameCount
.L_last_vcount_init:
    .word OSi_LastVCount
