/* Format and execute bytewise backup-memory SPI transfers. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_SpiTransfer, "ax", %progbits
.balign 4

.extern CARDi_CommandBegin
.extern CARDi_CommandEnd
.extern CARDi_CommArray
.extern CARDi_CommReadCore
.extern CARDi_CommWriteCore
.extern CARDi_SpiWork
.extern CARDi_Work
.extern CARDi_WriteEnableCommand
.extern REG_AUXSPICNT
.extern REG_AUXSPIDATA

.global CARDi_SendSpiAddressingCommand
.type CARDi_SendSpiAddressingCommand, %function
CARDi_SendSpiAddressingCommand:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r2, .L_address_card_work
    ldr r2, [r2]
    ldr r2, [r2, #0x24]
    cmp r2, #1
    beq .L_one_address_byte
    cmp r2, #2
    beq .L_two_address_bytes
    cmp r2, #3
    beq .L_three_address_bytes
    b .L_send_address
.L_one_address_byte:
    and r3, r0, #0xff
    lsr r0, r0, #5
    and r0, r0, #8
    orr r0, r1, r0
    orr r0, r0, r3, lsl #8
    str r0, [sp]
    b .L_send_address
.L_two_address_bytes:
    and r3, r0, #0xff
    and r0, r0, #0xff00
    orr r0, r1, r0
    orr r0, r0, r3, lsl #16
    str r0, [sp]
    b .L_send_address
.L_three_address_bytes:
    and r12, r0, #0xff
    and r3, r0, #0xff00
    lsr r0, r0, #8
    and r0, r0, #0xff00
    orr r0, r1, r0
    orr r0, r0, r3, lsl #8
    orr r0, r0, r12, lsl #24
    str r0, [sp]
.L_send_address:
    add r0, sp, #0
    mov r1, #0
    add r2, r2, #1
    ldr r3, .L_address_write_callback
    bl CARDi_CommArray
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_address_card_work:
    .word CARDi_Work
.L_address_write_callback:
    .word CARDi_CommWriteCore
.size CARDi_SendSpiAddressingCommand, . - CARDi_SendSpiAddressingCommand

.global CARDi_WriteEnable
.type CARDi_WriteEnable, %function
CARDi_WriteEnable:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r0, #1
    bl CARDi_CommandBegin
    ldr r0, .L_write_enable_command
    mov r1, #0
    mov r2, #1
    ldr r3, .L_enable_write_callback
    bl CARDi_CommArray
    mov r0, #0
    mov r1, r0
    bl CARDi_CommandEnd
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_write_enable_command:
    .word CARDi_WriteEnableCommand
.L_enable_write_callback:
    .word CARDi_CommWriteCore
.size CARDi_WriteEnable, . - CARDi_WriteEnable

.global CARDi_CommVerifyCore
.type CARDi_CommVerifyCore, %function
CARDi_CommVerifyCore:
    mov r2, #0
    ldr r1, .L_verify_spi_data
    strh r2, [r1]
    ldr r2, .L_verify_spi_control
.L_verify_wait:
    ldrh r1, [r2]
    ands r1, r1, #0x80
    bne .L_verify_wait
    ldr r1, .L_verify_spi_data
    ldrh r1, [r1]
    and r2, r1, #0xff
    ldr r1, [r0, #4]
    ldrb r1, [r1]
    cmp r2, r1
    beq .L_verify_advance
    mov r1, #0
    str r1, [r0, #0xc]
    ldr r1, [r0]
    cmp r1, #1
    movhi r1, #1
    strhi r1, [r0]
.L_verify_advance:
    ldr r1, [r0, #4]
    add r1, r1, #1
    str r1, [r0, #4]
    bx lr
.L_verify_spi_data:
    .word REG_AUXSPIDATA
.L_verify_spi_control:
    .word REG_AUXSPICNT
.size CARDi_CommVerifyCore, . - CARDi_CommVerifyCore

.global CARDi_CommWriteCore
.type CARDi_CommWriteCore, %function
CARDi_CommWriteCore:
    sub sp, sp, #8
    ldr r1, [r0, #4]
    ldrb r2, [r1]
    ldr r1, .L_write_spi_data
    strh r2, [r1]
    ldr r1, [r0, #4]
    add r1, r1, #1
    str r1, [r0, #4]
    ldr r1, .L_write_spi_control
.L_write_wait:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_write_wait
    ldr r0, .L_write_spi_data
    ldrh r0, [r0]
    strh r0, [sp]
    add sp, sp, #8
    bx lr
.L_write_spi_data:
    .word REG_AUXSPIDATA
.L_write_spi_control:
    .word REG_AUXSPICNT
.size CARDi_CommWriteCore, . - CARDi_CommWriteCore

.global CARDi_CommReadCore
.type CARDi_CommReadCore, %function
CARDi_CommReadCore:
    mov r2, #0
    ldr r1, .L_read_spi_data
    strh r2, [r1]
    ldr r2, .L_read_spi_control
.L_read_wait:
    ldrh r1, [r2]
    ands r1, r1, #0x80
    bne .L_read_wait
    ldr r1, .L_read_spi_data
    ldrh r2, [r1]
    ldr r1, [r0, #8]
    strb r2, [r1]
    ldr r1, [r0, #8]
    add r1, r1, #1
    str r1, [r0, #8]
    bx lr
.L_read_spi_data:
    .word REG_AUXSPIDATA
.L_read_spi_control:
    .word REG_AUXSPICNT
.size CARDi_CommReadCore, . - CARDi_CommReadCore

.global CARDi_CommArray
.type CARDi_CommArray, %function
CARDi_CommArray:
    stmdb sp!, {r4-r8, lr}
    mov r7, r2
    mov r6, r3
    ldr r5, .L_spi_work
    str r0, [r5, #4]
    str r1, [r5, #8]
    ldr r0, .L_spi_control_start
    ldr r4, .L_array_spi_control
    strh r0, [r4]
    mov r8, #0xa000
    b .L_array_check
.L_array_next:
    ldr r0, [r5]
    sub r0, r0, #1
    str r0, [r5]
    ldr r0, [r5]
    cmp r0, #0
    strheq r8, [r4]
.L_array_wait:
    ldrh r0, [r4]
    ands r0, r0, #0x80
    bne .L_array_wait
    mov r0, r5
    mov lr, pc
    bx r6
    sub r7, r7, #1
.L_array_check:
    cmp r7, #0
    bne .L_array_next
    ldr r0, [r5]
    cmp r0, #0
    moveq r1, #0
    ldreq r0, .L_array_spi_control
    strheq r1, [r0]
    ldmia sp!, {r4-r8, lr}
    bx lr
.L_spi_work:
    .word CARDi_SpiWork
.L_spi_control_start:
    .word 0x0000a040
.L_array_spi_control:
    .word REG_AUXSPICNT
.size CARDi_CommArray, . - CARDi_CommArray
