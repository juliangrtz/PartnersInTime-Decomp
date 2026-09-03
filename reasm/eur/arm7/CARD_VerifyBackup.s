/* Verify backup-memory contents through the shared CARD SPI transfer engine. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_VerifyBackup, "ax", %progbits
.balign 4

.extern CARDi_CommandBegin
.extern CARDi_CommandEnd
.extern CARDi_CommArray
.extern CARDi_CommVerifyCore
.extern CARDi_SendSpiAddressingCommand
.extern CARDi_SpiWork
.extern CARDi_WaitPrevCommand
.extern CARDi_Work

.global CARDi_VerifyBackupCore
.type CARDi_VerifyBackupCore, %function
CARDi_VerifyBackupCore:
    stmdb sp!, {r4-r6, lr}
    mov r6, r0
    mov r5, r1
    mov r4, r2
    bl CARDi_WaitPrevCommand
    mov r1, #1
    ldr r0, .L_spi_work
    str r1, [r0, #0xc]
    ldr r0, .L_card_work
    ldr r0, [r0]
    ldr r0, [r0, #0x24]
    add r0, r0, #1
    add r0, r0, r4
    bl CARDi_CommandBegin
    mov r0, r6
    mov r1, #3
    bl CARDi_SendSpiAddressingCommand
    mov r0, r5
    mov r1, #0
    mov r2, r4
    ldr r3, .L_verify_callback
    bl CARDi_CommArray
    mov r0, #0
    mov r1, r0
    bl CARDi_CommandEnd
    ldr r0, .L_card_work
    ldr r1, [r0]
    ldr r0, [r1]
    cmp r0, #0
    bne .L_verify_done
    ldr r0, .L_spi_work
    ldr r0, [r0, #0xc]
    cmp r0, #0
    moveq r0, #1
    streq r0, [r1]
.L_verify_done:
    ldmia sp!, {r4-r6, lr}
    bx lr
.L_spi_work:
    .word CARDi_SpiWork
.L_card_work:
    .word CARDi_Work
.L_verify_callback:
    .word CARDi_CommVerifyCore
.size CARDi_VerifyBackupCore, . - CARDi_VerifyBackupCore
