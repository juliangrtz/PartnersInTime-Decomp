/* Service queued CARD requests and dispatch their operation handlers. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_TaskThread, "ax", %progbits
.balign 4

.extern CARDi_CommandTable
.extern CARDi_Work
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SleepThread
.extern OS_WakeupThread
.extern OS_WakeupThreadDirect
.extern PXI_SendWordByFifo

.global CARDi_TaskThread
.type CARDi_TaskThread, %function
CARDi_TaskThread:
    stmdb sp!, {r4-r11, lr}
    sub sp, sp, #4
    ldr r10, .L_card_work
    add r5, r10, #0x40
    mov r8, #0
    mov r4, #0xb
    mov r6, #1
    add r7, r10, #0x38
    add r11, r10, #0xbc
.L_task_loop:
    mov r9, r8
    bl OS_DisableInterrupts
    str r0, [sp]
.L_wait_for_task:
    ldr r0, [r10, #0x38]
    ands r0, r0, #4
    bne .L_task_ready
    ldr r0, [r10, #0x38]
    ands r0, r0, #0x10
    beq .L_sleep
    ldr r0, [r7]
    orr r0, r0, #4
    str r0, [r7]
    ldr r0, [r7]
    bic r0, r0, #0x10
    str r0, [r7]
    mov r9, r6
    b .L_restore_interrupts
.L_task_ready:
    ldr r0, [r10, #0x38]
    ands r0, r0, #8
    bne .L_restore_interrupts
.L_sleep:
    str r5, [r10, #0xb8]
    mov r0, r8
    bl OS_SleepThread
    b .L_wait_for_task
.L_restore_interrupts:
    ldr r0, [sp]
    bl OS_RestoreInterrupts
    cmp r9, #0
    beq .L_run_task_callback
    ldr r0, [r10]
    str r8, [r0]
    mov r0, r10
    ldr r2, [r10, #4]
    ldr r1, .L_card_command_table
    ldr r1, [r1, r2, lsl #2]
    mov lr, pc
    bx r1
.L_notify_arm9:
    mov r0, r4
    mov r1, r6
    mov r2, r6
    bl PXI_SendWordByFifo
    cmp r0, #0
    blt .L_notify_arm9
    bl OS_DisableInterrupts
    mov r9, r0
    ldr r0, [r10, #0x38]
    bic r0, r0, #0xc
    str r0, [r10, #0x38]
    mov r0, r11
    bl OS_WakeupThread
    ldr r0, [r10, #0x38]
    ands r0, r0, #0x10
    beq .L_restore_after_notify
    mov r0, r5
    bl OS_WakeupThreadDirect
.L_restore_after_notify:
    mov r0, r9
    bl OS_RestoreInterrupts
    b .L_task_loop
.L_run_task_callback:
    mov r0, r10
    ldr r1, [r10, #0x34]
    mov lr, pc
    bx r1
    b .L_task_loop
.L_card_work:
    .word CARDi_Work
.L_card_command_table:
    .word CARDi_CommandTable
.size CARDi_TaskThread, . - CARDi_TaskThread
