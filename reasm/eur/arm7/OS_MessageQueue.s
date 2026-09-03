/* NitroSDK fixed-capacity message queue operations. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_MessageQueue, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SleepThread
.extern OS_WakeupThread
.extern _s32_div_f

.global OS_ReadMessage
.type OS_ReadMessage, %function
OS_ReadMessage:
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    mov r6, r0
    mov r5, r1
    mov r7, r2
    bl OS_DisableInterrupts
    mov r4, r0
    and r8, r7, #1
    add r7, r6, #2
    b .L_read_test
.L_read_empty:
    cmp r8, #0
    bne .L_read_wait
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #0
    b .L_read_return
.L_read_wait:
    mov r0, r7
    bl OS_SleepThread
.L_read_test:
    ldr r0, [r6, #0x10]
    cmp r0, #0
    beq .L_read_empty
    cmp r5, #0
    ldrne r1, [r6, #4]
    ldrne r0, [r6, #0x0C]
    ldrne r0, [r1, r0, lsl #2]
    strne r0, [r5]
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #1
.L_read_return:
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
.size OS_ReadMessage, . - OS_ReadMessage

.global OS_ReceiveMessage
.type OS_ReceiveMessage, %function
OS_ReceiveMessage:
    stmdb sp!, {r4, r5, r6, r7, r8, lr}
    mov r6, r0
    mov r5, r1
    mov r7, r2
    bl OS_DisableInterrupts
    mov r4, r0
    and r8, r7, #1
    add r7, r6, #2
    b .L_receive_test
.L_receive_empty:
    cmp r8, #0
    bne .L_receive_wait
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #0
    b .L_receive_return
.L_receive_wait:
    mov r0, r7
    bl OS_SleepThread
.L_receive_test:
    ldr r0, [r6, #0x10]
    cmp r0, #0
    beq .L_receive_empty
    cmp r5, #0
    ldrne r1, [r6, #4]
    ldrne r0, [r6, #0x0C]
    ldrne r0, [r1, r0, lsl #2]
    strne r0, [r5]
    ldr r0, [r6, #0x0C]
    add r0, r0, #1
    ldr r1, [r6, #8]
    bl _s32_div_f
    str r1, [r6, #0x0C]
    ldr r0, [r6, #0x10]
    sub r0, r0, #1
    str r0, [r6, #0x10]
    mov r0, r6
    bl OS_WakeupThread
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #1
.L_receive_return:
    ldmia sp!, {r4, r5, r6, r7, r8, lr}
    bx lr
.size OS_ReceiveMessage, . - OS_ReceiveMessage

.global OS_SendMessage
.type OS_SendMessage, %function
OS_SendMessage:
    stmdb sp!, {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r6, r0
    mov r5, r1
    mov r7, r2
    bl OS_DisableInterrupts
    mov r4, r0
    and r7, r7, #1
    b .L_send_test
.L_send_full:
    cmp r7, #0
    bne .L_send_wait
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #0
    b .L_send_return
.L_send_wait:
    mov r0, r6
    bl OS_SleepThread
.L_send_test:
    ldr r2, [r6, #0x10]
    ldr r1, [r6, #8]
    cmp r1, r2
    ble .L_send_full
    ldr r0, [r6, #0x0C]
    add r0, r0, r2
    bl _s32_div_f
    ldr r0, [r6, #4]
    str r5, [r0, r1, lsl #2]
    ldr r0, [r6, #0x10]
    add r0, r0, #1
    str r0, [r6, #0x10]
    add r0, r6, #2
    bl OS_WakeupThread
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #1
.L_send_return:
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, lr}
    bx lr
.size OS_SendMessage, . - OS_SendMessage

.global OS_InitMessageQueue
.type OS_InitMessageQueue, %function
OS_InitMessageQueue:
    mov r3, #0
    strh r3, [r0]
    strh r3, [r0, #2]
    str r1, [r0, #4]
    str r2, [r0, #8]
    str r3, [r0, #0x0C]
    str r3, [r0, #0x10]
    bx lr
.size OS_InitMessageQueue, . - OS_InitMessageQueue
