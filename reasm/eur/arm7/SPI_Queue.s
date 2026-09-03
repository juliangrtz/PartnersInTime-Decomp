/* Queue SPI requests and serialize subsystem ownership. */

.syntax unified
.arch armv4t
.arm
.section .pit_SPI_Queue, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_ReadMessage
.extern OS_RestoreInterrupts
.extern OS_SendMessage
.extern OS_WakeupThread
.extern SPIi_EntryCommands
.extern SPIi_EntryMessages
.extern SPIi_ExceptionThreadQueue
.extern SPIi_MessageQueue
.extern SPIi_Work

.global SPIi_CheckEntry
.type SPIi_CheckEntry, %function
SPIi_CheckEntry:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_check_message_queue
    add r1, sp, #0
    mov r2, #0
    bl OS_ReadMessage
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_check_message_queue:
    .word SPIi_MessageQueue
.size SPIi_CheckEntry, . - SPIi_CheckEntry

.global SPIi_SetEntry
.type SPIi_SetEntry, %function
SPIi_SetEntry:
    stmdb sp!, {r0-r3}
    stmdb sp!, {r4-r6, lr}
    mov r5, r0
    mov r4, r1
    ldrh r0, [sp, #24]
    cmp r0, #4
    movhi r0, #0
    bhi .L_set_done
    bl OS_DisableInterrupts
    ldr r2, .L_set_work
    ldr r3, [r2, #0x454]
    mov r1, #24
    mul r12, r3, r1
    ldr r3, .L_set_commands
    str r5, [r3, r12]
    ldr r3, [r2, #0x454]
    mul r5, r3, r1
    ldr r3, .L_set_messages
    str r4, [r3, r5]
    add r3, sp, #24
    bic r3, r3, #3
    add r6, r3, #4
    mov lr, #0
    ldrh r12, [sp, #24]
    b .L_set_test_argument
.L_set_copy_argument:
    add r6, r6, #4
    ldr r5, [r6, #-4]
    ldr r4, [r2, #0x454]
    mla r3, r4, r1, r2
    add r3, r3, lr, lsl #2
    str r5, [r3, #0x2dc]
    add lr, lr, #1
.L_set_test_argument:
    cmp lr, r12
    blt .L_set_copy_argument
    ldr r1, .L_set_work
    ldr r4, [r1, #0x454]
    add r2, r4, #1
    and r2, r2, #15
    str r2, [r1, #0x454]
    bl OS_RestoreInterrupts
    ldr r0, .L_set_message_queue
    ldr r2, .L_set_commands
    mov r1, #24
    mla r1, r4, r1, r2
    mov r2, #0
    bl OS_SendMessage
.L_set_done:
    ldmia sp!, {r4-r6, lr}
    add sp, sp, #16
    bx lr
.L_set_work:
    .word SPIi_Work
.L_set_commands:
    .word SPIi_EntryCommands
.L_set_messages:
    .word SPIi_EntryMessages
.L_set_message_queue:
    .word SPIi_MessageQueue
.size SPIi_SetEntry, . - SPIi_SetEntry

.global SPIi_ReleaseException
.type SPIi_ReleaseException, %function
SPIi_ReleaseException:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_release_work
    ldr r2, [r1, #4]
    cmp r2, r0
    bne .L_release_done
    mov r0, #5
    str r0, [r1, #4]
    mov r0, #0
    str r0, [r1]
    ldr r0, .L_release_queue
    bl OS_WakeupThread
.L_release_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_release_work:
    .word SPIi_Work
.L_release_queue:
    .word SPIi_ExceptionThreadQueue
.size SPIi_ReleaseException, . - SPIi_ReleaseException

.global SPIi_GetException
.type SPIi_GetException, %function
SPIi_GetException:
    mov r2, #1
    ldr r1, .L_get_work
    str r2, [r1]
    str r0, [r1, #4]
    bx lr
.L_get_work:
    .word SPIi_Work
.size SPIi_GetException, . - SPIi_GetException

.global SPIi_CheckException
.type SPIi_CheckException, %function
SPIi_CheckException:
    ldr r0, .L_exception_work
    ldr r0, [r0]
    cmp r0, #0
    moveq r0, #1
    movne r0, #0
    bx lr
.L_exception_work:
    .word SPIi_Work
.size SPIi_CheckException, . - SPIi_CheckException
