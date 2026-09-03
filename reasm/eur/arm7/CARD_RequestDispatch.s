/* Adapt shared ARM9 CARD command records to the ARM7 operation cores. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_RequestDispatch, "ax", %progbits
.balign 4

.extern CARDi_EraseBackupSectorCore
.extern CARDi_ProgramBackupCore
.extern CARDi_ReadBackupCore
.extern CARDi_ReadRom
.extern CARDi_ReadRomIDCore
.extern CARDi_VerifyBackupCore
.extern CARDi_WriteBackupCore

.global CARDi_TaskEraseSector
.type CARDi_TaskEraseSector, %function
CARDi_TaskEraseSector:
    ldr r0, [r0]
    ldr r0, [r0, #0x10]
    ldr r12, .L_erase_sector_target
    bx r12
.L_erase_sector_target:
    .word CARDi_EraseBackupSectorCore
.size CARDi_TaskEraseSector, . - CARDi_TaskEraseSector

.global CARDi_TaskVerifyBackup
.type CARDi_TaskVerifyBackup, %function
CARDi_TaskVerifyBackup:
    ldr r2, [r0]
    ldr r0, [r2, #0x10]
    ldr r1, [r2, #0xc]
    ldr r2, [r2, #0x14]
    ldr r12, .L_verify_target
    bx r12
.L_verify_target:
    .word CARDi_VerifyBackupCore
.size CARDi_TaskVerifyBackup, . - CARDi_TaskVerifyBackup

.global CARDi_TaskWriteBackup
.type CARDi_TaskWriteBackup, %function
CARDi_TaskWriteBackup:
    ldr r2, [r0]
    ldr r0, [r2, #0x10]
    ldr r1, [r2, #0xc]
    ldr r2, [r2, #0x14]
    ldr r12, .L_write_target
    bx r12
.L_write_target:
    .word CARDi_WriteBackupCore
.size CARDi_TaskWriteBackup, . - CARDi_TaskWriteBackup

.global CARDi_TaskProgramBackup
.type CARDi_TaskProgramBackup, %function
CARDi_TaskProgramBackup:
    ldr r2, [r0]
    ldr r0, [r2, #0x10]
    ldr r1, [r2, #0xc]
    ldr r2, [r2, #0x14]
    ldr r12, .L_program_target
    bx r12
.L_program_target:
    .word CARDi_ProgramBackupCore
.size CARDi_TaskProgramBackup, . - CARDi_TaskProgramBackup

.global CARDi_TaskReadBackup
.type CARDi_TaskReadBackup, %function
CARDi_TaskReadBackup:
    ldr r2, [r0]
    ldr r0, [r2, #0xc]
    ldr r1, [r2, #0x10]
    ldr r2, [r2, #0x14]
    ldr r12, .L_read_backup_target
    bx r12
.L_read_backup_target:
    .word CARDi_ReadBackupCore
.size CARDi_TaskReadBackup, . - CARDi_TaskReadBackup

.global CARDi_TaskReadRom
.type CARDi_TaskReadRom, %function
CARDi_TaskReadRom:
    ldr r3, [r0]
    mov r0, #4
    ldr r1, [r3, #0xc]
    ldr r2, [r3, #0x10]
    ldr r3, [r3, #0x14]
    ldr r12, .L_read_rom_target
    bx r12
.L_read_rom_target:
    .word CARDi_ReadRom
.size CARDi_TaskReadRom, . - CARDi_TaskReadRom

.global CARDi_TaskReadRomID
.type CARDi_TaskReadRomID, %function
CARDi_TaskReadRomID:
    stmdb sp!, {r4, lr}
    mov r4, r0
    bl CARDi_ReadRomIDCore
    ldr r1, [r4]
    str r0, [r1, #8]
    ldmia sp!, {r4, lr}
    bx lr
.size CARDi_TaskReadRomID, . - CARDi_TaskReadRomID

.global CARDi_TaskIdentify
.type CARDi_TaskIdentify, %function
CARDi_TaskIdentify:
    bx lr
.size CARDi_TaskIdentify, . - CARDi_TaskIdentify

.global CARDi_TaskAck
.type CARDi_TaskAck, %function
CARDi_TaskAck:
    bx lr
.size CARDi_TaskAck, . - CARDi_TaskAck

.global CARDi_TaskInit
.type CARDi_TaskInit, %function
CARDi_TaskInit:
    bx lr
.size CARDi_TaskInit, . - CARDi_TaskInit

.global CARDi_TaskUnsupported
.type CARDi_TaskUnsupported, %function
CARDi_TaskUnsupported:
    mov r1, #3
    ldr r0, [r0]
    str r1, [r0]
    bx lr
.size CARDi_TaskUnsupported, . - CARDi_TaskUnsupported
