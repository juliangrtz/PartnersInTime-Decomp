/* Initialize the ARM7 CARD ROM and backup-memory service state. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_Init, "ax", %progbits
.balign 4

.extern CARD_InitPulledOutCallback
.extern CARDi_InitCommon
.extern CARDi_RomAccessor
.extern CARDi_RomBase
.extern CARDi_RomStat
.extern CARDi_Work

.global CARD_Init
.type CARD_Init, %function
CARD_Init:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r2, .L_card_work
    ldr r0, [r2, #0x38]
    cmp r0, #0
    bne .L_card_init_done
    mov r0, #1
    str r0, [r2, #0x38]
    mov r1, #0
    str r1, [r2, #0x24]
    ldr r0, [r2, #0x24]
    str r0, [r2, #0x20]
    ldr r0, [r2, #0x20]
    str r0, [r2, #0x1c]
    mvn r0, #0
    str r0, [r2, #0x28]
    str r1, [r2, #0x2c]
    str r1, [r2, #0x30]
    ldr r0, .L_rom_base
    str r1, [r0]
    bl CARDi_InitCommon
    ldr r1, .L_rom_accessor
    ldr r0, .L_rom_stat
    str r1, [r0]
    bl CARD_InitPulledOutCallback
.L_card_init_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_card_work:
    .word CARDi_Work
.L_rom_base:
    .word CARDi_RomBase
.L_rom_accessor:
    .word CARDi_RomAccessor
.L_rom_stat:
    .word CARDi_RomStat
.size CARD_Init, . - CARD_Init
