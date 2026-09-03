/* Read cartridge pages, maintain the ROM cache, and finish CARD requests. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_RomTransfer, "ax", %progbits
.balign 4

.extern CARDi_Work
.extern MI_CpuCopy8
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OS_WakeupThread
.extern OS_WakeupThreadDirect
.extern REG_CARD_CMD
.extern REG_CARD_CNT
.extern REG_CARD_DATA
.extern REG_CARD_MASTERCNT

.global CARDi_ReadCard
.type CARDi_ReadCard, %function
CARDi_ReadCard:
    stmdb sp!, {r4-r11, lr}
    sub sp, sp, #4
    mov r10, r0
    ldr r9, .L_read_card_work
    add r7, r10, #0x20
    ldr r11, .L_read_card_data
    ldr r5, .L_read_card_control
    mov r6, #0
    mov r0, #0x200
    rsb r4, r0, #0
.L_read_page:
    ldr r0, [r9, #0x1c]
    and r1, r0, r4
    cmp r1, r0
    bne .L_use_cache
    ldr r8, [r9, #0x20]
    ands r0, r8, #3
    bne .L_use_cache
    ldr r0, [r9, #0x24]
    cmp r0, #0x200
    bhs .L_destination_ready
.L_use_cache:
    mov r8, r7
    str r1, [r10, #8]
.L_destination_ready:
    lsr r0, r1, #8
    orr r0, r0, #0xb7000000
    lsl r1, r1, #24
    bl CARDi_SetRomOp
    mov r0, r6
    ldr r1, [r10, #4]
    str r1, [r5]
.L_read_fifo:
    ldr r2, [r5]
    ands r1, r2, #0x00800000
    beq .L_check_transfer_end
    ldr r1, [r11]
    cmp r0, #0x200
    strlo r1, [r8, r0, lsl #2]
    addlo r0, r0, #1
.L_check_transfer_end:
    ands r1, r2, #0x80000000
    bne .L_read_fifo
    ldr r0, [r9, #0x20]
    cmp r8, r0
    bne .L_copy_from_cache
    ldr r0, [r9, #0x1c]
    add r0, r0, #0x200
    str r0, [r9, #0x1c]
    ldr r0, [r9, #0x20]
    add r0, r0, #0x200
    str r0, [r9, #0x20]
    ldr r0, [r9, #0x24]
    subs r0, r0, #0x200
    str r0, [r9, #0x24]
    bne .L_read_page
    b .L_read_done
.L_copy_from_cache:
    mov r0, r10
    bl CARDi_ReadFromCache
    cmp r0, #0
    bne .L_read_page
.L_read_done:
    add sp, sp, #4
    ldmia sp!, {r4-r11, lr}
    bx lr
.L_read_card_work:
    .word CARDi_Work
.L_read_card_data:
    .word REG_CARD_DATA
.L_read_card_control:
    .word REG_CARD_CNT
.size CARDi_ReadCard, . - CARDi_ReadCard

.global CARDi_SetRomOp
.type CARDi_SetRomOp, %function
CARDi_SetRomOp:
    ldr r3, .L_setop_card_control
.L_wait_for_previous_rom_op:
    ldr r2, [r3]
    ands r2, r2, #0x80000000
    bne .L_wait_for_previous_rom_op
    mov r3, #0xc0
    ldr r2, .L_card_master_control
    strb r3, [r2]
    lsr r3, r0, #24
    ldr r2, .L_card_command_0
    strb r3, [r2]
    lsr r3, r0, #16
    ldr r2, .L_card_command_1
    strb r3, [r2]
    lsr r3, r0, #8
    ldr r2, .L_card_command_2
    strb r3, [r2]
    ldr r2, .L_card_command_3
    strb r0, [r2]
    lsr r2, r1, #24
    ldr r0, .L_card_command_4
    strb r2, [r0]
    lsr r2, r1, #16
    ldr r0, .L_card_command_5
    strb r2, [r0]
    lsr r2, r1, #8
    ldr r0, .L_card_command_6
    strb r2, [r0]
    ldr r0, .L_card_command_7
    strb r1, [r0]
    bx lr
.L_setop_card_control:
    .word REG_CARD_CNT
.L_card_master_control:
    .word REG_CARD_MASTERCNT
.L_card_command_0:
    .word REG_CARD_CMD
.L_card_command_1:
    .word REG_CARD_CMD + 1
.L_card_command_2:
    .word REG_CARD_CMD + 2
.L_card_command_3:
    .word REG_CARD_CMD + 3
.L_card_command_4:
    .word REG_CARD_CMD + 4
.L_card_command_5:
    .word REG_CARD_CMD + 5
.L_card_command_6:
    .word REG_CARD_CMD + 6
.L_card_command_7:
    .word REG_CARD_CMD + 7
.size CARDi_SetRomOp, . - CARDi_SetRomOp

.global CARDi_ReadEnd
.type CARDi_ReadEnd, %function
CARDi_ReadEnd:
    stmdb sp!, {r4-r7, lr}
    sub sp, sp, #4
    ldr r7, .L_end_card_work
    ldr r6, [r7, #0x2c]
    ldr r5, [r7, #0x30]
    mov r1, #0
    ldr r0, [r7]
    str r1, [r0]
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r0, [r7, #0x38]
    bic r0, r0, #0xc
    str r0, [r7, #0x38]
    add r0, r7, #0xbc
    bl OS_WakeupThread
    ldr r0, [r7, #0x38]
    ands r0, r0, #0x10
    beq .L_end_restore
    add r0, r7, #0x40
    bl OS_WakeupThreadDirect
.L_end_restore:
    mov r0, r4
    bl OS_RestoreInterrupts
    cmp r6, #0
    beq .L_end_done
    mov r0, r5
    mov lr, pc
    bx r6
.L_end_done:
    add sp, sp, #4
    ldmia sp!, {r4-r7, lr}
    bx lr
.L_end_card_work:
    .word CARDi_Work
.size CARDi_ReadEnd, . - CARDi_ReadEnd

.global CARDi_ReadFromCache
.type CARDi_ReadFromCache, %function
CARDi_ReadFromCache:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    ldr r5, .L_cache_card_work
    ldr r2, [r5, #0x1c]
    mov r1, #0x200
    rsb r1, r1, #0
    and r2, r2, r1
    ldr r1, [r0, #8]
    cmp r2, r1
    bne .L_cache_result
    ldr r1, [r5, #0x1c]
    sub r2, r1, r2
    rsb r4, r2, #0x200
    ldr r1, [r5, #0x24]
    cmp r4, r1
    movhi r4, r1
    add r0, r0, #0x20
    add r0, r0, r2
    ldr r1, [r5, #0x20]
    mov r2, r4
    bl MI_CpuCopy8
    ldr r0, [r5, #0x1c]
    add r0, r0, r4
    str r0, [r5, #0x1c]
    ldr r0, [r5, #0x20]
    add r0, r0, r4
    str r0, [r5, #0x20]
    ldr r0, [r5, #0x24]
    sub r0, r0, r4
    str r0, [r5, #0x24]
.L_cache_result:
    ldr r0, [r5, #0x24]
    cmp r0, #0
    movne r0, #1
    moveq r0, #0
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_cache_card_work:
    .word CARDi_Work
.size CARDi_ReadFromCache, . - CARDi_ReadFromCache
