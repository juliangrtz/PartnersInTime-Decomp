/* Initialize the ARM7 side of the NitroSDK PXI FIFO and sync handshake. */

.syntax unified
.arch armv4t
.arm
.section .pit_PXIi_InitFifo, "ax", %progbits
.balign 4

.global PXIi_InitFifo
.type PXIi_InitFifo, %function
.extern HW_MAIN_MEM_SYSTEM
.extern OS_DisableInterrupts
.extern OS_EnableIrqMask
.extern OS_ResetRequestIrqMask
.extern OS_RestoreInterrupts
.extern OS_SetIrqFunction
.extern OS_SpinWait
.extern PXIi_FifoInitialized
.extern PXIi_FifoRecvCallbackTable
.extern PXIi_HandlerRecvFifoNotEmpty
.extern REG_PXI_FIFO_CNT
.extern REG_PXI_SYNC
PXIi_InitFifo:
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    bl OS_DisableInterrupts
    mov r5, r0
    ldr r0, .L_fifo_initialized
    ldrh r1, [r0]
    cmp r1, #0
    bne .L_restore_interrupts
    mov r1, #1
    strh r1, [r0]
    mov r2, #0
    ldr r0, .L_system_work
    str r2, [r0, #0x38C]
    mov r1, r2
    ldr r0, .L_callback_table
.L_clear_callback_table:
    str r1, [r0, r2, lsl #2]
    add r2, r2, #1
    cmp r2, #0x20
    blt .L_clear_callback_table
    ldr r1, .L_fifo_control_initial
    ldr r0, .L_fifo_control
    strh r1, [r0]
    mov r0, #0x40000
    bl OS_ResetRequestIrqMask
    mov r0, #0x40000
    ldr r1, .L_receive_handler
    bl OS_SetIrqFunction
    mov r0, #0x40000
    bl OS_EnableIrqMask

    mov r4, #8
    mov r6, r4
    ldr r8, .L_sync
    mov r7, #0x3E8
    b .L_check_sync_attempt
.L_sync_attempt:
    mov r0, r4, lsl #8
    strh r0, [r8]
    mov r0, r7
    bl OS_SpinWait
    ldrh r0, [r8]
    and r0, r0, #0xF
    cmp r0, r4
    movne r4, r6
    sub r4, r4, #1
.L_check_sync_attempt:
    cmp r4, #0
    bge .L_sync_attempt

.L_restore_interrupts:
    mov r0, r5
    bl OS_RestoreInterrupts
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
.L_fifo_initialized:
    .word PXIi_FifoInitialized
.L_system_work:
    .word HW_MAIN_MEM_SYSTEM
.L_callback_table:
    .word PXIi_FifoRecvCallbackTable
.L_fifo_control_initial:
    .word 0x0000C408
.L_fifo_control:
    .word REG_PXI_FIFO_CNT
.L_receive_handler:
    .word PXIi_HandlerRecvFifoNotEmpty
.L_sync:
    .word REG_PXI_SYNC

.size PXIi_InitFifo, . - PXIi_InitFifo
