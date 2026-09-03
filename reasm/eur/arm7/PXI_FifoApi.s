/* Public ARM7 NitroSDK PXI FIFO send and callback APIs. */

.syntax unified
.arch armv4t
.arm
.section .pit_PXI_FifoApi, "ax", %progbits
.balign 4

.extern HW_MAIN_MEM_SYSTEM
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern PXIi_FifoRecvCallbackTable
.extern REG_PXI_FIFO_CNT
.extern REG_PXI_SEND_FIFO

.global PXI_SendWordByFifo
.type PXI_SendWordByFifo, %function
PXI_SendWordByFifo:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r3, [sp]
    bic r3, r3, #0x1F
    and r0, r0, #0x1F
    orr r0, r3, r0
    str r0, [sp]
    bic r3, r0, #0x20
    and r0, r2, #1
    orr r0, r3, r0, lsl #5
    str r0, [sp]
    and r2, r0, #0x3F
    bic r0, r1, #0xFC000000
    orr r0, r2, r0, lsl #6
    str r0, [sp]

    ldr r1, .L_fifo_control_send
    ldrh r0, [r1]
    ands r0, r0, #0x4000
    ldrhne r0, [r1]
    orrne r0, r0, #0xC000
    strhne r0, [r1]
    mvnne r0, #0
    bne .L_send_done

    bl OS_DisableInterrupts
    ldr r1, .L_fifo_control_send
    ldrh r1, [r1]
    ands r1, r1, #2
    beq .L_write_fifo
    bl OS_RestoreInterrupts
    mvn r0, #1
    b .L_send_done
.L_write_fifo:
    ldr r2, [sp]
    ldr r1, .L_send_fifo
    str r2, [r1]
    bl OS_RestoreInterrupts
    mov r0, #0
.L_send_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_fifo_control_send:
    .word REG_PXI_FIFO_CNT
.L_send_fifo:
    .word REG_PXI_SEND_FIFO
.size PXI_SendWordByFifo, . - PXI_SendWordByFifo

.global PXI_IsCallbackReady
.type PXI_IsCallbackReady, %function
PXI_IsCallbackReady:
    mov r3, #1
    mov r2, r3, lsl r0
    ldr r0, .L_system_work_ready
    add r0, r0, r1, lsl #2
    ldr r0, [r0, #0x388]
    ands r0, r2, r0
    moveq r3, #0
    mov r0, r3
    bx lr
.L_system_work_ready:
    .word HW_MAIN_MEM_SYSTEM
.size PXI_IsCallbackReady, . - PXI_IsCallbackReady

.global PXI_SetFifoRecvCallback
.type PXI_SetFifoRecvCallback, %function
PXI_SetFifoRecvCallback:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    mov r5, r1
    bl OS_DisableInterrupts
    ldr r1, .L_callback_table_set
    str r5, [r1, r4, lsl #2]
    cmp r5, #0
    beq .L_clear_ready
    ldr r3, .L_system_work_set
    ldr r2, [r3, #0x38C]
    mov r1, #1
    orr r1, r2, r1, lsl r4
    str r1, [r3, #0x38C]
    b .L_restore_set_callback
.L_clear_ready:
    ldr r3, .L_system_work_set
    ldr r2, [r3, #0x38C]
    mov r1, #1
    mvn r1, r1, lsl r4
    and r1, r2, r1
    str r1, [r3, #0x38C]
.L_restore_set_callback:
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_callback_table_set:
    .word PXIi_FifoRecvCallbackTable
.L_system_work_set:
    .word HW_MAIN_MEM_SYSTEM
.size PXI_SetFifoRecvCallback, . - PXI_SetFifoRecvCallback
