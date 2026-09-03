/* Initialize the ARM7 serial-peripheral services and worker thread. */

.syntax unified
.arch armv4t
.arm
.section .pit_SPI_Init, "ax", %progbits
.balign 4

.extern HW_TOUCHPANEL_X
.extern HW_TOUCHPANEL_Y
.extern MIC_Init
.extern MI_CpuFill8
.extern NVRAM_Init
.extern OS_CreateThread
.extern OS_InitMessageQueue
.extern OS_WakeupThreadDirect
.extern PM_Init
.extern PXI_Init
.extern PXI_SetFifoRecvCallback
.extern SPIi_EntryCommands
.extern SPIi_FifoRecvCallback
.extern SPIi_Initialized
.extern SPIi_MessageBuffer
.extern SPIi_MessageQueue
.extern SPIi_ReturnResult
.extern SPIi_SetEntry
.extern SPIi_TaskThread
.extern SPIi_Thread
.extern SPIi_ThreadStackTop
.extern SPIi_Work
.extern TP_Init
.extern TPi_Work

.global SPI_Init
.type SPI_Init, %function
SPI_Init:
    stmdb sp!, {r4-r8, lr}
    sub sp, sp, #8
    mov r4, r0
    ldr r0, .L_initialized
    ldrh r1, [r0]
    cmp r1, #0
    bne .L_init_done
    mov r1, #1
    strh r1, [r0]
    mov r1, #0
    ldr r0, .L_work
    str r1, [r0]
    mov r1, #5
    str r1, [r0, #4]
    bl TP_Init
    bl NVRAM_Init
    bl MIC_Init
    bl PM_Init
    bl PXI_Init
    mov r0, #6
    ldr r1, .L_fifo_callback
    bl PXI_SetFifoRecvCallback
    mov r0, #9
    ldr r1, .L_fifo_callback
    bl PXI_SetFifoRecvCallback
    mov r0, #8
    ldr r1, .L_fifo_callback
    bl PXI_SetFifoRecvCallback
    mov r0, #4
    ldr r1, .L_fifo_callback
    bl PXI_SetFifoRecvCallback
    ldr r0, .L_message_queue
    ldr r1, .L_message_buffer
    mov r2, #16
    bl OS_InitMessageQueue
    mov r8, #0
    ldr r7, .L_entry_commands
    mov r6, r8
    mov r5, #24
.L_clear_entries:
    mla r0, r8, r5, r7
    mov r1, r6
    mov r2, r5
    bl MI_CpuFill8
    add r8, r8, #1
    cmp r8, #16
    blt .L_clear_entries
    mov r2, #0
    ldr r0, .L_work
    str r2, [r0, #0x454]
    ldr r0, .L_thread_stack_top
    strh r2, [r0, #0x58]
    mov r0, #0x200
    str r0, [sp]
    str r4, [sp, #4]
    ldr r0, .L_thread
    ldr r1, .L_task_thread
    ldr r3, .L_message_queue
    bl OS_CreateThread
    ldr r0, .L_thread
    bl OS_WakeupThreadDirect
.L_init_done:
    add sp, sp, #8
    ldmia sp!, {r4-r8, lr}
    bx lr
.L_initialized:
    .word SPIi_Initialized
.L_work:
    .word SPIi_Work
.L_fifo_callback:
    .word SPIi_FifoRecvCallback
.L_message_queue:
    .word SPIi_MessageQueue
.L_message_buffer:
    .word SPIi_MessageBuffer
.L_entry_commands:
    .word SPIi_EntryCommands
.L_thread_stack_top:
    .word SPIi_ThreadStackTop
.L_thread:
    .word SPIi_Thread
.L_task_thread:
    .word SPIi_TaskThread
.size SPI_Init, . - SPI_Init

.global SPIi_SetTouchPanelCallbackValue
.type SPIi_SetTouchPanelCallbackValue, %function
SPIi_SetTouchPanelCallbackValue:
    stmdb sp!, {lr}
    sub sp, sp, #4
    cmp r0, #0
    bne .L_touch_value_valid
    mov r0, #3
    mov r1, #2
    bl SPIi_ReturnResult
    b .L_touch_value_done
.L_touch_value_valid:
    ldr r1, .L_touch_work
    str r0, [r1, #0x24]
    str r0, [r1, #0x28]
    mov r0, #3
    mov r1, #0
    bl SPIi_ReturnResult
.L_touch_value_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_touch_work:
    .word TPi_Work
.size SPIi_SetTouchPanelCallbackValue, . - SPIi_SetTouchPanelCallbackValue

.global TPi_VAlarmCallback
.type TPi_VAlarmCallback, %function
TPi_VAlarmCallback:
    stmdb sp!, {r4, lr}
    sub sp, sp, #8
    mov r4, r0
    mov r0, #0
    mov r1, #16
    mov r2, #1
    mov r3, r4
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_valarm_done
    ldr r0, [sp]
    bic r0, r0, #0x06000000
    orr r0, r0, #0x06000000
    str r0, [sp]
    ldrh r1, [sp]
    ldr r0, .L_touch_x
    strh r1, [r0]
    ldrh r1, [sp, #2]
    ldr r0, .L_touch_y
    strh r1, [r0]
    mov r0, #16
    and r1, r4, #0xff
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    bl SPIi_ReturnResult
.L_valarm_done:
    add sp, sp, #8
    ldmia sp!, {r4, lr}
    bx lr
.L_touch_x:
    .word HW_TOUCHPANEL_X
.L_touch_y:
    .word HW_TOUCHPANEL_Y
.size TPi_VAlarmCallback, . - TPi_VAlarmCallback
