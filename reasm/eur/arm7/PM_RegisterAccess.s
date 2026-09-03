/* Transfer power-management register commands over the ARM7 SPI port. */

.syntax unified
.arch armv4t
.arm
.section .pit_PM_RegisterAccess, "ax", %progbits
.balign 4

.global PMi_GetRegister
.type PMi_GetRegister, %function
PMi_GetRegister:
    stmdb sp!, {r4, lr}
    mov r4, r0
    ldr r1, .L_get_spicnt
.L_get_wait_command:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_get_wait_command
    mov r0, #1
    bl PMi_SetSpiControl
    orr r0, r4, #0x80
    and r0, r0, #0xff
    and r1, r0, #0xff
    ldr r0, .L_get_spidata
    strh r1, [r0]
    ldr r1, .L_get_spicnt
.L_get_wait_address:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_get_wait_address
    mov r0, #0
    bl PMi_SetSpiControl
    mov r1, #0
    ldr r0, .L_get_spidata
    strh r1, [r0]
    ldr r1, .L_get_spicnt
.L_get_wait_value:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_get_wait_value
    ldr r0, .L_get_spidata
    ldrh r0, [r0]
    and r0, r0, #0xff
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    and r0, r0, #0xff
    ldmia sp!, {r4, lr}
    bx lr
.L_get_spicnt:
    .word 0x040001c0
.L_get_spidata:
    .word 0x040001c2
.size PMi_GetRegister, . - PMi_GetRegister

.global PMi_SetRegister
.type PMi_SetRegister, %function
PMi_SetRegister:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, r1
    ldr r1, .L_set_spicnt
.L_set_wait_command:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_set_wait_command
    mov r0, #1
    bl PMi_SetSpiControl
    and r0, r5, #0xff
    and r1, r0, #0xff
    ldr r0, .L_set_spidata
    strh r1, [r0]
    ldr r1, .L_set_spicnt
.L_set_wait_address:
    ldrh r0, [r1]
    ands r0, r0, #0x80
    bne .L_set_wait_address
    mov r0, #0
    bl PMi_SetSpiControl
    and r1, r4, #0xff
    ldr r0, .L_set_spidata
    strh r1, [r0]
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_set_spicnt:
    .word 0x040001c0
.L_set_spidata:
    .word 0x040001c2
.size PMi_SetRegister, . - PMi_SetRegister

.global PMi_SetSpiControl
.type PMi_SetSpiControl, %function
PMi_SetSpiControl:
    ldr r1, .L_spi_control_base
    orr r1, r1, r0, lsl #11
    ldr r0, .L_spicnt
    strh r1, [r0]
    bx lr
.L_spi_control_base:
    .word 0x00008002
.L_spicnt:
    .word 0x040001c0
.size PMi_SetSpiControl, . - PMi_SetSpiControl
