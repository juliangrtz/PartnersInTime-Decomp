/* Transfer backup-memory data in device-page-sized SPI chunks. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_BackupTransfer, "ax", %progbits
.balign 4

.extern CARDi_CommandBegin
.extern CARDi_CommandEnd
.extern CARDi_CommArray
.extern CARDi_CommReadCore
.extern CARDi_CommWriteCore
.extern CARDi_SendSpiAddressingCommand
.extern CARDi_WaitPrevCommand
.extern CARDi_Work
.extern CARDi_WriteEnable

.global CARDi_WriteBackupCore
.type CARDi_WriteBackupCore, %function
CARDi_WriteBackupCore:
    stmdb sp!, {r4-r11, lr}
    sub sp, sp, #4
    mov r10, r0
    mov r9, r1
    mov r8, r2
    ldr r0, .L_write_card_work
    ldr r0, [r0]
    ldr r7, [r0, #0x20]
    bl CARDi_WaitPrevCommand
    sub r5, r7, #1
    mov r4, #0xa
    mov r11, #0
    b .L_write_check
.L_write_next:
    and r0, r10, r5
    sub r6, r7, r0
    cmp r6, r8
    movhi r6, r8
    bl CARDi_WriteEnable
    ldr r0, .L_write_card_work
    ldr r0, [r0]
    ldr r0, [r0, #0x24]
    add r0, r0, #1
    add r0, r0, r6
    bl CARDi_CommandBegin
    mov r0, r10
    mov r1, r4
    bl CARDi_SendSpiAddressingCommand
    mov r0, r9
    mov r1, r11
    mov r2, r6
    ldr r3, .L_write_callback
    bl CARDi_CommArray
    ldr r0, .L_write_card_work
    ldr r1, [r0]
    ldr r0, [r1, #0x2c]
    ldr r1, [r1, #0x30]
    bl CARDi_CommandEnd
    ldr r0, .L_write_card_work
    ldr r0, [r0]
    ldr r0, [r0]
    cmp r0, #0
    bne .L_write_done
    add r9, r9, r6
    add r10, r10, r6
    sub r8, r8, r6
.L_write_check:
    cmp r8, #0
    bne .L_write_next
.L_write_done:
    add sp, sp, #4
    ldmia sp!, {r4-r11, lr}
    bx lr
.L_write_card_work:
    .word CARDi_Work
.L_write_callback:
    .word CARDi_CommWriteCore
.size CARDi_WriteBackupCore, . - CARDi_WriteBackupCore

.global CARDi_ProgramBackupCore
.type CARDi_ProgramBackupCore, %function
CARDi_ProgramBackupCore:
    stmdb sp!, {r4-r11, lr}
    sub sp, sp, #4
    mov r10, r0
    mov r9, r1
    mov r8, r2
    ldr r0, .L_program_card_work
    ldr r0, [r0]
    ldr r7, [r0, #0x20]
    bl CARDi_WaitPrevCommand
    sub r5, r7, #1
    mov r11, #2
    mov r4, #0
    b .L_program_check
.L_program_next:
    and r0, r10, r5
    sub r6, r7, r0
    cmp r6, r8
    movhi r6, r8
    bl CARDi_WriteEnable
    ldr r0, .L_program_card_work
    ldr r0, [r0]
    ldr r0, [r0, #0x24]
    add r0, r0, #1
    add r0, r0, r6
    bl CARDi_CommandBegin
    mov r0, r10
    mov r1, r11
    bl CARDi_SendSpiAddressingCommand
    mov r0, r9
    mov r1, r4
    mov r2, r6
    ldr r3, .L_program_callback
    bl CARDi_CommArray
    ldr r0, .L_program_card_work
    ldr r0, [r0]
    ldr r0, [r0, #0x28]
    mov r1, r4
    bl CARDi_CommandEnd
    ldr r0, .L_program_card_work
    ldr r0, [r0]
    ldr r0, [r0]
    cmp r0, #0
    bne .L_program_done
    add r9, r9, r6
    add r10, r10, r6
    sub r8, r8, r6
.L_program_check:
    cmp r8, #0
    bne .L_program_next
.L_program_done:
    add sp, sp, #4
    ldmia sp!, {r4-r11, lr}
    bx lr
.L_program_card_work:
    .word CARDi_Work
.L_program_callback:
    .word CARDi_CommWriteCore
.size CARDi_ProgramBackupCore, . - CARDi_ProgramBackupCore

.global CARDi_ReadBackupCore
.type CARDi_ReadBackupCore, %function
CARDi_ReadBackupCore:
    stmdb sp!, {r4-r6, lr}
    mov r6, r0
    mov r5, r1
    mov r4, r2
    bl CARDi_WaitPrevCommand
    ldr r0, .L_read_card_work
    ldr r0, [r0]
    ldr r0, [r0, #0x24]
    add r0, r0, #1
    add r0, r0, r4
    bl CARDi_CommandBegin
    mov r0, r6
    mov r1, #3
    bl CARDi_SendSpiAddressingCommand
    mov r0, #0
    mov r1, r5
    mov r2, r4
    ldr r3, .L_read_callback
    bl CARDi_CommArray
    mov r0, #0
    mov r1, r0
    bl CARDi_CommandEnd
    ldmia sp!, {r4-r6, lr}
    bx lr
.L_read_card_work:
    .word CARDi_Work
.L_read_callback:
    .word CARDi_CommReadCore
.size CARDi_ReadBackupCore, . - CARDi_ReadBackupCore
