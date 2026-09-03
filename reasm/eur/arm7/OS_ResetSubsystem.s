/* NitroSDK ARM7 reset coordination, termination, and spin-wait helpers. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_ResetSubsystem, "ax", %progbits
.balign 4

.extern MI_StopDma
.extern OS_DisableInterrupts
.extern OS_ResetRequestIrqMask
.extern OS_SetIrqMask
.extern OSi_IsResetOccurred
.extern OSi_ResetInitialized
.extern PXI_SendWordByFifo
.extern PXI_SetFifoRecvCallback
.extern SVC_Halt_Thunk
.extern SVC_WaitByLoop
.extern _s32_div_f
.extern func_037FE70C
.extern func_03807604

.global OS_SpinWait
.type OS_SpinWait, %function
OS_SpinWait:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r1, #4
    bl _s32_div_f
    bl SVC_WaitByLoop_Thunk
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size OS_SpinWait, . - OS_SpinWait

.global SVC_WaitByLoop_Thunk
.type SVC_WaitByLoop_Thunk, %function
SVC_WaitByLoop_Thunk:
    ldr r12, .L_wait_by_loop
    bx r12
.L_wait_by_loop:
    /* ELF marks .thumb_func symbols odd when used as callable pointers. */
    .word SVC_WaitByLoop
.size SVC_WaitByLoop_Thunk, . - SVC_WaitByLoop_Thunk

.global OS_Terminate
.type OS_Terminate, %function
OS_Terminate:
    stmdb sp!, {lr}
    sub sp, sp, #4
.L_terminate_loop:
    bl OS_DisableInterrupts
    bl SVC_Halt_Thunk
    b .L_terminate_loop
.size OS_Terminate, . - OS_Terminate

.global OS_ResetSystem
.type OS_ResetSystem, %function
OS_ResetSystem:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r0, #0
    bl MI_StopDma
    mov r0, #1
    bl MI_StopDma
    mov r0, #2
    bl MI_StopDma
    mov r0, #3
    bl MI_StopDma
    mov r0, #0x40000
    bl OS_SetIrqMask
    mvn r0, #0
    bl OS_ResetRequestIrqMask
    bl func_037FE70C
    mov r0, #0x10
    bl OSi_SendResetCommand
    bl func_03807604
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size OS_ResetSystem, . - OS_ResetSystem

.global OSi_SendResetCommand
.type OSi_SendResetCommand, %function
OSi_SendResetCommand:
    stmdb sp!, {r4, r5, r6, lr}
    mov r6, r0, lsl #8
    mov r5, #12
    mov r4, #0
.L_send_reset_command:
    mov r0, r5
    mov r1, r6
    mov r2, r4
    bl PXI_SendWordByFifo
    cmp r0, #0
    bne .L_send_reset_command
    ldmia sp!, {r4, r5, r6, lr}
    bx lr
.size OSi_SendResetCommand, . - OSi_SendResetCommand

.global OSi_ResetCallback
.type OSi_ResetCallback, %function
OSi_ResetCallback:
    stmdb sp!, {lr}
    sub sp, sp, #4
    and r0, r1, #0x7F00
    mov r0, r0, lsl #8
    mov r0, r0, lsr #16
    cmp r0, #0x10
    moveq r1, #1
    ldreq r0, .L_reset_occurred
    strheq r1, [r0]
    beq .L_reset_callback_done
    bl OS_Terminate
.L_reset_callback_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_reset_occurred:
    .word OSi_IsResetOccurred
.size OSi_ResetCallback, . - OSi_ResetCallback

.global OS_IsResetOccurred
.type OS_IsResetOccurred, %function
OS_IsResetOccurred:
    ldr r0, .L_reset_occurred_2
    ldrh r0, [r0]
    bx lr
.L_reset_occurred_2:
    .word OSi_IsResetOccurred
.size OS_IsResetOccurred, . - OS_IsResetOccurred

.global OS_InitReset
.type OS_InitReset, %function
OS_InitReset:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_reset_initialized
    ldrh r1, [r0]
    cmp r1, #0
    bne .L_init_reset_done
    mov r1, #1
    strh r1, [r0]
    mov r0, #12
    ldr r1, .L_reset_callback
    bl PXI_SetFifoRecvCallback
.L_init_reset_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_reset_initialized:
    .word OSi_ResetInitialized
.L_reset_callback:
    .word OSi_ResetCallback
.size OS_InitReset, . - OS_InitReset
