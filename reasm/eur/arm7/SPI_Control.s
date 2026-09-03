/* Return SPI results and serialize direct hardware users. */

.syntax unified
.arch armv4t
.arm
.section .pit_SPI_Control, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SleepThread
.extern OS_WakeupThread
.extern PXI_SendWordByFifo
.extern SPIi_ExceptionThreadQueue
.extern SPIi_GetException
.extern SPIi_Work

.global SPIi_ReturnResult
.type SPIi_ReturnResult, %function
SPIi_ReturnResult:
    stmdb sp!, {r4-r6, lr}
    and r2, r0, #0x70
    cmp r2, #0x30
    bgt .L_result_over_30
    cmp r2, #0x30
    bge .L_result_tag_4
    cmp r2, #0x10
    bgt .L_result_over_10
    cmp r2, #0x10
    bge .L_result_tag_6
    cmp r2, #0
    beq .L_result_tag_6
    b .L_result_send
.L_result_over_10:
    cmp r2, #0x20
    beq .L_result_tag_4
    b .L_result_send
.L_result_over_30:
    cmp r2, #0x50
    bgt .L_result_over_50
    cmp r2, #0x50
    bge .L_result_tag_9
    cmp r2, #0x40
    beq .L_result_tag_9
    b .L_result_send
.L_result_over_50:
    cmp r2, #0x60
    bgt .L_result_over_60
    cmp r2, #0x60
    beq .L_result_tag_8
    b .L_result_send
.L_result_over_60:
    cmp r2, #0x70
    beq .L_result_tag_8
    b .L_result_send
.L_result_tag_6:
    mov r4, #6
    b .L_result_send
.L_result_tag_9:
    mov r4, #9
    b .L_result_send
.L_result_tag_8:
    mov r4, #8
    b .L_result_send
.L_result_tag_4:
    mov r4, #4
.L_result_send:
    and r0, r0, #0xff
    orr r0, r0, #0x80
    mov r0, r0, lsl #8
    orr r2, r0, #0x03000000
    and r0, r1, #0xff
    orr r6, r2, r0
    mov r5, #0
.L_result_retry:
    mov r0, r4
    mov r1, r6
    mov r2, r5
    bl PXI_SendWordByFifo
    cmp r0, #0
    blt .L_result_retry
    ldmia sp!, {r4-r6, lr}
    bx lr
.size SPIi_ReturnResult, . - SPIi_ReturnResult

.global SPI_Unlock
.type SPI_Unlock, %function
SPI_Unlock:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_unlock_work
    ldr r2, [r1]
    cmp r2, #0
    beq .L_unlock_done
    ldr r2, [r1, #4]
    cmp r2, #4
    bne .L_unlock_done
    ldr r1, [r1, #0x45c]
    cmp r1, r0
    bne .L_unlock_done
    bl OS_DisableInterrupts
    mov r2, #5
    ldr r1, .L_unlock_work
    str r2, [r1, #4]
    mov r2, #0
    str r2, [r1]
    str r2, [r1, #0x45c]
    bl OS_RestoreInterrupts
    ldr r0, .L_unlock_queue
    bl OS_WakeupThread
.L_unlock_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_unlock_work:
    .word SPIi_Work
.L_unlock_queue:
    .word SPIi_ExceptionThreadQueue
.size SPI_Unlock, . - SPI_Unlock

.global SPI_Lock
.type SPI_Lock, %function
SPI_Lock:
    stmdb sp!, {r4-r7, lr}
    sub sp, sp, #4
    mov r7, r0
    ldr r5, .L_lock_queue
    ldr r4, .L_lock_work
.L_lock_retry:
    bl OS_DisableInterrupts
    mov r6, r0
    ldr r1, [r4]
    cmp r1, #0
    beq .L_lock_acquire
    bl OS_RestoreInterrupts
    mov r0, r5
    bl OS_SleepThread
    b .L_lock_retry
.L_lock_acquire:
    mov r0, #4
    bl SPIi_GetException
    ldr r0, .L_lock_work
    str r7, [r0, #0x45c]
    mov r0, r6
    bl OS_RestoreInterrupts
    add sp, sp, #4
    ldmia sp!, {r4-r7, lr}
    bx lr
.L_lock_queue:
    .word SPIi_ExceptionThreadQueue
.L_lock_work:
    .word SPIi_Work
.size SPI_Lock, . - SPI_Lock
