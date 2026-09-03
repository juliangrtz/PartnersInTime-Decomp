/* Serialize ROM-ID and ROM-data requests and configure the cartridge bus. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_RomRequest, "ax", %progbits
.balign 4

.extern CARDi_ReadCard
.extern CARDi_ReadEnd
.extern CARDi_RomHeaderControl
.extern CARDi_RomStat
.extern CARDi_SetRomOp
.extern CARDi_Work
.extern MI_StopDma
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_SleepThread
.extern OSi_ThreadInfo
.extern REG_CARD_CNT
.extern REG_CARD_DATA

.global CARDi_ReadRomID
.type CARDi_ReadRomID, %function
CARDi_ReadRomID:
    stmdb sp!, {r4-r6, lr}
    ldr r4, .L_id_card_work
    bl OS_DisableInterrupts
    mov r6, r0
    add r5, r4, #0xbc
    b .L_id_lock_check
.L_id_lock_wait:
    mov r0, r5
    bl OS_SleepThread
.L_id_lock_check:
    ldr r0, [r4, #0x38]
    ands r0, r0, #4
    bne .L_id_lock_wait
    ldr r0, [r4, #0x38]
    orr r0, r0, #4
    str r0, [r4, #0x38]
    mov r0, r6
    bl OS_RestoreInterrupts
    mov r0, #0
    str r0, [r4, #0x2c]
    str r0, [r4, #0x30]
    bl CARDi_ReadRomIDCore
    mov r4, r0
    bl CARDi_ReadEnd
    mov r0, r4
    ldmia sp!, {r4-r6, lr}
    bx lr
.L_id_card_work:
    .word CARDi_Work
.size CARDi_ReadRomID, . - CARDi_ReadRomID

.global CARDi_ReadRom
.type CARDi_ReadRom, %function
CARDi_ReadRom:
    stmdb sp!, {r4, lr}
    ldr r4, .L_rom_stat
    ldr r12, .L_rom_card_work
    str r0, [r12, #0x28]
    str r1, [r12, #0x1c]
    str r2, [r12, #0x20]
    str r3, [r12, #0x24]
    mov r1, #0
    str r1, [r12, #0x2c]
    str r1, [r12, #0x30]
    cmp r0, #3
    bhi .L_rom_dma_stopped
    bl MI_StopDma
.L_rom_dma_stopped:
    ldr r0, .L_rom_header_control
    ldr r0, [r0]
    bic r0, r0, #0x07000000
    orr r0, r0, #0xa1000000
    str r0, [r4, #4]
    ldr r0, .L_thread_info
    ldr r1, [r0, #8]
    ldr r0, .L_rom_card_work
    str r1, [r0, #0xb8]
    mov r0, r4
    bl CARDi_ReadCard
    ldmia sp!, {r4, lr}
    bx lr
.L_rom_stat:
    .word CARDi_RomStat
.L_rom_card_work:
    .word CARDi_Work
.L_rom_header_control:
    .word CARDi_RomHeaderControl
.L_thread_info:
    .word OSi_ThreadInfo
.size CARDi_ReadRom, . - CARDi_ReadRom

.global CARDi_ReadRomIDCore
.type CARDi_ReadRomIDCore, %function
CARDi_ReadRomIDCore:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r0, #0xb8000000
    mov r1, #0
    bl CARDi_SetRomOp
    ldr r0, .L_id_header_control
    ldr r0, [r0]
    bic r0, r0, #0x07000000
    orr r1, r0, #0xa7000000
    mov r0, #0x2000
    rsb r0, r0, #0
    and r0, r1, r0
    ldr r1, .L_card_control
    str r0, [r1]
.L_wait_for_id:
    ldr r0, [r1]
    ands r0, r0, #0x00800000
    beq .L_wait_for_id
    ldr r0, .L_card_data
    ldr r0, [r0]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_id_header_control:
    .word CARDi_RomHeaderControl
.L_card_control:
    .word REG_CARD_CNT
.L_card_data:
    .word REG_CARD_DATA
.size CARDi_ReadRomIDCore, . - CARDi_ReadRomIDCore
