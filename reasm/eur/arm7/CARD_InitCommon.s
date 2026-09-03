/* Enable the CARD service and construct its asynchronous worker thread. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_InitCommon, "ax", %progbits
.balign 4

.extern CARDi_BootMode
.extern CARDi_Enabled
.extern CARDi_Thread
.extern CARDi_ThreadStackTop
.extern CARDi_Work
.extern OS_CreateThread
.extern OS_WakeupThreadDirect
.extern PXI_SetFifoRecvCallback

.global CARD_Enable
.type CARD_Enable, %function
CARD_Enable:
    ldr r1, .L_card_enabled
    str r0, [r1]
    bx lr
.L_card_enabled:
    .word CARDi_Enabled
.size CARD_Enable, . - CARD_Enable

.global CARDi_InitCommon
.type CARDi_InitCommon, %function
CARDi_InitCommon:
    stmdb sp!, {lr}
    sub sp, sp, #0xc
    mvn r1, #2
    ldr r0, .L_card_work
    str r1, [r0, #0xc]
    mov r2, #0
    str r2, [r0, #0x10]
    str r2, [r0, #0x18]
    str r2, [r0]
    str r2, [r0, #8]
    strh r2, [r0, #0x14]
    strh r2, [r0, #0xbc]
    mov r1, #4
    str r1, [r0, #0x3c]
    mov r1, #0x400
    str r1, [sp]
    ldr r0, [r0, #0x3c]
    str r0, [sp, #4]
    ldr r0, .L_card_thread
    ldr r1, .L_card_task_thread
    ldr r3, .L_card_thread_stack
    bl OS_CreateThread
    ldr r0, .L_card_thread
    bl OS_WakeupThreadDirect
    mov r0, #0xb
    ldr r1, .L_card_fifo_callback
    bl PXI_SetFifoRecvCallback
    ldr r0, .L_card_boot_mode
    ldrh r0, [r0]
    cmp r0, #2
    beq .L_card_common_done
    mov r0, #1
    bl CARD_Enable
.L_card_common_done:
    add sp, sp, #0xc
    ldmia sp!, {lr}
    bx lr
.L_card_work:
    .word CARDi_Work
.L_card_thread:
    .word CARDi_Thread
.L_card_task_thread:
    .word CARDi_TaskThread
.L_card_thread_stack:
    .word CARDi_ThreadStackTop
.L_card_fifo_callback:
    .word CARDi_OnFifoRecv
.L_card_boot_mode:
    .word CARDi_BootMode
.size CARDi_InitCommon, . - CARDi_InitCommon
