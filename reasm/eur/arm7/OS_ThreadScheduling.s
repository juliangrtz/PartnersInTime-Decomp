/* Thread priority, selection, and context-switch scheduling. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_ThreadScheduling, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_GetProcessorMode
.extern OS_LoadContext
.extern OS_RestoreInterrupts
.extern OS_SaveContext
.extern OSi_CurrentThreadPtr
.extern OSi_IdleThread
.extern OSi_InsertThreadToList
.extern OSi_RescheduleCount
.extern OSi_SystemCallbackInSwitchThread
.extern OSi_ThreadInfo

.global OS_SetThreadPriority
.type OS_SetThreadPriority, %function
OS_SetThreadPriority:
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    mov r6, r0
    mov r5, r1
    ldr r0, .L_thread_info_priority
    ldr r8, [r0, #0x0C]
    mov r7, #0
    bl OS_DisableInterrupts
    mov r4, r0
    b .L_find_thread
.L_next_thread:
    mov r7, r8
    ldr r8, [r8, #0x4C]
.L_find_thread:
    cmp r8, #0
    beq .L_priority_invalid
    cmp r8, r6
    bne .L_next_thread
.L_priority_invalid:
    cmp r8, #0
    beq .L_priority_failure
    ldr r0, .L_idle_thread
    cmp r8, r0
    bne .L_check_priority
.L_priority_failure:
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #0
    b .L_priority_return
.L_check_priority:
    ldr r0, [r8, #0x54]
    cmp r0, r5
    beq .L_priority_success
    cmp r7, #0
    ldreq r1, [r6, #0x4C]
    ldreq r0, .L_thread_info_priority
    streq r1, [r0, #0x0C]
    ldrne r0, [r6, #0x4C]
    strne r0, [r7, #0x4C]
    str r5, [r6, #0x54]
    mov r0, r6
    bl OSi_InsertThreadToList
    bl OSi_RescheduleThread
.L_priority_success:
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #1
.L_priority_return:
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
.L_thread_info_priority:
    .word OSi_ThreadInfo
.L_idle_thread:
    .word OSi_IdleThread
.size OS_SetThreadPriority, . - OS_SetThreadPriority

.global OSi_RescheduleThread
.type OSi_RescheduleThread, %function
OSi_RescheduleThread:
    stmdb sp!, {r4, r5, r6, lr}
    ldr r4, .L_thread_info_reschedule
    ldr r0, .L_reschedule_count
    ldr r0, [r0]
    cmp r0, #0
    bne .L_reschedule_return
    ldrh r0, [r4, #4]
    cmp r0, #0
    bne .L_defer_reschedule
    bl OS_GetProcessorMode
    cmp r0, #0x12
    bne .L_select_thread
.L_defer_reschedule:
    mov r0, #1
    strh r0, [r4]
    b .L_reschedule_return
.L_select_thread:
    ldr r0, .L_current_thread_reschedule
    ldr r0, [r0]
    ldr r6, [r0]
    bl OS_SelectThread
    mov r5, r0
    cmp r6, r5
    beq .L_reschedule_return
    cmp r5, #0
    beq .L_reschedule_return
    mov r0, r6
    bl OS_SaveContext
    cmp r0, #0
    bne .L_reschedule_return
    ldr r0, .L_system_switch_callback
    ldr r2, [r0]
    cmp r2, #0
    beq .L_call_user_switch_callback
    mov r0, r6
    mov r1, r5
    mov lr, pc
    bx r2
.L_call_user_switch_callback:
    ldr r2, [r4, #0x10]
    cmp r2, #0
    beq .L_load_next_context
    mov r0, r6
    mov r1, r5
    mov lr, pc
    bx r2
.L_load_next_context:
    ldr r0, .L_thread_info_reschedule
    str r5, [r0, #8]
    mov r0, r5
    bl OS_LoadContext
.L_reschedule_return:
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
.L_thread_info_reschedule:
    .word OSi_ThreadInfo
.L_reschedule_count:
    .word OSi_RescheduleCount
.L_current_thread_reschedule:
    .word OSi_CurrentThreadPtr
.L_system_switch_callback:
    .word OSi_SystemCallbackInSwitchThread
.size OSi_RescheduleThread, . - OSi_RescheduleThread

.global OS_SelectThread
.type OS_SelectThread, %function
OS_SelectThread:
    ldr r0, .L_thread_info_select
    ldr r0, [r0, #0x0C]
    b .L_test_selected_thread
.L_next_selected_thread:
    ldr r0, [r0, #0x4C]
.L_test_selected_thread:
    cmp r0, #0
    bxeq lr
    ldr r1, [r0, #0x48]
    cmp r1, #1
    bne .L_next_selected_thread
    bx lr
.L_thread_info_select:
    .word OSi_ThreadInfo
.size OS_SelectThread, . - OS_SelectThread
