/* Start, poll, time, and finish backup-memory SPI commands. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_CommandControl, "ax", %progbits
.balign 4

.extern CARDi_CommArray
.extern CARDi_CommReadCore
.extern CARDi_CommWriteCore
.extern CARDi_ReadStatusCommand
.extern CARDi_SpiWork
.extern CARDi_Work
.extern OS_SpinWait
.extern REG_VCOUNT

.global CARDi_WaitPrevCommand
.type CARDi_WaitPrevCommand, %function
CARDi_WaitPrevCommand:
    mov r0, #0
    ldr r1, .L_previous_command_timeout
    ldr r12, .L_command_end_target
    bx r12
.L_previous_command_timeout:
    .word 0x00000316
.L_command_end_target:
    .word CARDi_CommandEnd
.size CARDi_WaitPrevCommand, . - CARDi_WaitPrevCommand

.global CARDi_CommandEnd
.type CARDi_CommandEnd, %function
CARDi_CommandEnd:
    stmdb sp!, {r4-r7, lr}
    sub sp, sp, #0x14
    mov r5, r0
    mov r4, r1
    mov r6, #1
    adds r0, r5, r4
    beq .L_command_complete
    cmp r5, #0
    beq .L_begin_busy_timeout
    ldr r0, .L_end_vcount
    ldrh r0, [r0]
    strh r0, [sp]
    str r5, [sp, #4]
    mov r7, #0
    add r6, sp, #0
    b .L_initial_delay_check
.L_initial_delay_wait:
    mov r0, r7
    bl OS_SpinWait
.L_initial_delay_check:
    mov r0, r6
    bl CARDi_CheckCommandTimeout
    cmp r0, #0
    bne .L_initial_delay_wait
.L_begin_busy_timeout:
    cmp r4, #0
    beq .L_check_busy_once
    ldr r0, .L_end_vcount
    ldrh r0, [r0]
    strh r0, [sp, #8]
    sub r0, r4, r5
    str r0, [sp, #0xc]
    mov r6, #0
    add r4, sp, #8
    mov r5, r6
    b .L_busy_timeout_check
.L_busy_timeout_wait:
    mov r0, r5
    bl OS_SpinWait
    bl CARDi_CommandCheckBusy
    mov r6, r0
.L_busy_timeout_check:
    cmp r6, #0
    bne .L_command_complete
    mov r0, r4
    bl CARDi_CheckCommandTimeout
    cmp r0, #0
    bne .L_busy_timeout_wait
    b .L_command_complete
.L_check_busy_once:
    bl CARDi_CommandCheckBusy
    mov r6, r0
.L_command_complete:
    cmp r6, #0
    moveq r1, #4
    ldreq r0, .L_end_card_work
    ldreq r0, [r0]
    streq r1, [r0]
    add sp, sp, #0x14
    ldmia sp!, {r4-r7, lr}
    bx lr
.L_end_vcount:
    .word REG_VCOUNT
.L_end_card_work:
    .word CARDi_Work
.size CARDi_CommandEnd, . - CARDi_CommandEnd

.global CARDi_CommandBegin
.type CARDi_CommandBegin, %function
CARDi_CommandBegin:
    ldr r1, .L_begin_spi_work
    str r0, [r1]
    bx lr
.L_begin_spi_work:
    .word CARDi_SpiWork
.size CARDi_CommandBegin, . - CARDi_CommandBegin

.global CARDi_CommandCheckBusy
.type CARDi_CommandCheckBusy, %function
CARDi_CommandCheckBusy:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r0, #2
    bl CARDi_CommandBegin
    ldr r0, .L_status_command
    mov r1, #0
    mov r2, #1
    ldr r3, .L_status_write_callback
    bl CARDi_CommArray
    mov r0, #0
    add r1, sp, #0
    mov r2, #1
    ldr r3, .L_status_read_callback
    bl CARDi_CommArray
    mov r0, #0
    mov r1, r0
    bl CARDi_CommandEnd
    ldrb r0, [sp]
    ands r0, r0, #1
    moveq r0, #1
    movne r0, #0
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_status_command:
    .word CARDi_ReadStatusCommand
.L_status_write_callback:
    .word CARDi_CommWriteCore
.L_status_read_callback:
    .word CARDi_CommReadCore
.size CARDi_CommandCheckBusy, . - CARDi_CommandCheckBusy

.global CARDi_CheckCommandTimeout
.type CARDi_CheckCommandTimeout, %function
CARDi_CheckCommandTimeout:
    ldr r1, .L_timeout_vcount
    ldrh r3, [r1]
    ldrh r1, [r0]
    subs r2, r3, r1
    beq .L_timeout_result
    strh r3, [r0]
    add r3, r0, #4
    ldr r1, [r0, #4]
    sub r1, r1, r2
    str r1, [r0, #4]
    cmp r2, #0
    ldrlt r2, [r3]
    ldrlt r1, .L_vcount_period
    sublt r1, r2, r1
    strlt r1, [r3]
.L_timeout_result:
    ldr r0, [r0, #4]
    cmp r0, #0
    movgt r0, #1
    movle r0, #0
    bx lr
.L_timeout_vcount:
    .word REG_VCOUNT
.L_vcount_period:
    .word 0x00000107
.size CARDi_CheckCommandTimeout, . - CARDi_CheckCommandTimeout
