/* Erase one backup-memory sector through the CARD SPI command engine. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_EraseSector, "ax", %progbits
.balign 4

.extern CARDi_CommandBegin
.extern CARDi_CommandEnd
.extern CARDi_SendSpiAddressingCommand
.extern CARDi_WaitPrevCommand
.extern CARDi_Work
.extern CARDi_WriteEnable

.global CARDi_EraseBackupSectorCore
.type CARDi_EraseBackupSectorCore, %function
CARDi_EraseBackupSectorCore:
    stmdb sp!, {r4, lr}
    mov r4, r0
    bl CARDi_WaitPrevCommand
    bl CARDi_WriteEnable
    ldr r0, .L_card_work
    ldr r0, [r0]
    ldr r0, [r0, #0x24]
    add r0, r0, #1
    bl CARDi_CommandBegin
    mov r0, r4
    mov r1, #0xd8
    bl CARDi_SendSpiAddressingCommand
    ldr r0, .L_card_work
    ldr r0, [r0]
    ldr r0, [r0, #0x38]
    mov r1, #0
    bl CARDi_CommandEnd
    ldmia sp!, {r4, lr}
    bx lr
.L_card_work:
    .word CARDi_Work
.size CARDi_EraseBackupSectorCore, . - CARDi_EraseBackupSectorCore
