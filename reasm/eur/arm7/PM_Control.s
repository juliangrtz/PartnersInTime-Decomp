/* Initialize PM state, send PXI replies, and update power-control bits. */

.syntax unified
.arch armv4t
.arm
.section .pit_PM_Control, "ax", %progbits
.balign 4

.extern PMi_GetRegister
.extern PMi_Initialized
.extern PMi_SetRegister
.extern PMi_Work
.extern PXI_SendWordByFifo

.global PM_Init
.type PM_Init, %function
PM_Init:
    mov r1, #1
    ldr r0, .L_initialized
    str r1, [r0]
    mov r3, #0
    ldr r0, .L_pm_work
    str r3, [r0, #32]
    mov r2, r3
.L_clear_work:
    mov r1, r3, lsl #1
    strh r2, [r0, r1]
    add r3, r3, #1
    cmp r3, #16
    blt .L_clear_work
    bx lr
.L_initialized:
    .word PMi_Initialized
.L_pm_work:
    .word PMi_Work
.size PM_Init, . - PM_Init

.global PMi_SendPxiCommand
.type PMi_SendPxiCommand, %function
PMi_SendPxiCommand:
    ldr r3, .L_halfword_mask
    and r3, r2, r3
    and r0, r0, #0x03c00000
    mov r2, r0, lsl #22
    and r0, r1, #0x003f0000
    orr r0, r2, r0, lsl #16
    orr r0, r3, r0
    ldr ip, .L_send_pxi_data
    bx ip
.L_halfword_mask:
    .word 0x0000ffff
.L_send_pxi_data:
    .word PMi_SendPxiData
.size PMi_SendPxiCommand, . - PMi_SendPxiCommand

.global PMi_SendPxiData
.type PMi_SendPxiData, %function
PMi_SendPxiData:
    stmdb sp!, {r4-r6, lr}
    mov r6, r0
    mov r5, #8
    mov r4, #0
.L_retry_pxi_send:
    mov r0, r5
    mov r1, r6
    mov r2, r4
    bl PXI_SendWordByFifo
    cmp r0, #0
    bne .L_retry_pxi_send
    ldmia sp!, {r4-r6, lr}
    bx lr
.size PMi_SendPxiData, . - PMi_SendPxiData

.global PMi_ResetControl
.type PMi_ResetControl, %function
PMi_ResetControl:
    stmdb sp!, {r4, lr}
    mov r4, r0
    mov r0, #0
    bl PMi_GetRegister
    mvn r1, r4
    and r0, r0, r1
    and r1, r0, #0xff
    mov r0, #0
    bl PMi_SetRegister
    ldmia sp!, {r4, lr}
    bx lr
.size PMi_ResetControl, . - PMi_ResetControl

.global PMi_SetControl
.type PMi_SetControl, %function
PMi_SetControl:
    stmdb sp!, {r4, lr}
    mov r4, r0
    mov r0, #0
    bl PMi_GetRegister
    orr r1, r0, r4
    mov r0, #0
    bl PMi_SetRegister
    ldmia sp!, {r4, lr}
    bx lr
.size PMi_SetControl, . - PMi_SetControl
