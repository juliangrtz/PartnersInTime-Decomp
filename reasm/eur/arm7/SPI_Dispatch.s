/* Dispatch SPI PXI commands and worker-thread requests. */

.syntax unified
.arch armv4t
.arm
.section .pit_SPI_Dispatch, "ax", %progbits
.balign 4

.extern MIC_AnalyzeCommand
.extern MIC_ExecuteProcess
.extern NVRAM_AnalyzeCommand
.extern NVRAM_ExecuteProcess
.extern OS_ReceiveMessage
.extern PM_AnalyzeCommand
.extern PM_ExecuteProcess
.extern SPIi_MessageQueue
.extern TP_AnalyzeCommand
.extern TP_ExecuteProcess

.global SPIi_FifoRecvCallback
.type SPIi_FifoRecvCallback, %function
SPIi_FifoRecvCallback:
    stmdb sp!, {lr}
    sub sp, sp, #4
    cmp r2, #0
    bne .L_fifo_done
    sub r0, r0, #4
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_fifo_done
    b .L_fifo_nvram
    b .L_fifo_done
    b .L_fifo_touch
    b .L_fifo_done
    b .L_fifo_power
    b .L_fifo_microphone
.L_fifo_touch:
    mov r0, r1
    bl TP_AnalyzeCommand
    b .L_fifo_done
.L_fifo_microphone:
    mov r0, r1
    bl MIC_AnalyzeCommand
    b .L_fifo_done
.L_fifo_power:
    mov r0, r1
    bl PM_AnalyzeCommand
    b .L_fifo_done
.L_fifo_nvram:
    mov r0, r1
    bl NVRAM_AnalyzeCommand
.L_fifo_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size SPIi_FifoRecvCallback, . - SPIi_FifoRecvCallback

.global SPIi_TaskThread
.type SPIi_TaskThread, %function
SPIi_TaskThread:
    stmdb sp!, {r4-r6, lr}
    sub sp, sp, #8
    ldr r6, .L_message_queue
    add r5, sp, #0
    mov r4, #1
.L_receive:
    mov r0, r6
    mov r1, r5
    mov r2, r4
    bl OS_ReceiveMessage
    ldr r0, [sp]
    ldr r1, [r0]
    cmp r1, #3
    addls pc, pc, r1, lsl #2
    b .L_receive
    b .L_execute_touch
    b .L_execute_nvram
    b .L_execute_microphone
    b .L_execute_power
.L_execute_touch:
    bl TP_ExecuteProcess
    b .L_receive
.L_execute_microphone:
    bl MIC_ExecuteProcess
    b .L_receive
.L_execute_power:
    bl PM_ExecuteProcess
    b .L_receive
.L_execute_nvram:
    bl NVRAM_ExecuteProcess
    b .L_receive
.L_message_queue:
    .word SPIi_MessageQueue
.size SPIi_TaskThread, . - SPIi_TaskThread
