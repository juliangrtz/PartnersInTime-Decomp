/* Top-level NitroSDK ARM7 operating-system initialization. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Init, "ax", %progbits
.balign 4

.global OS_Init
.type OS_Init, %function
.extern OS_InitIrqTable
.extern OS_InitLock
.extern OS_InitThread
.extern OS_InitArena
.extern OS_InitTick
.extern OS_InitAlarm
.extern OS_InitReset
.extern PXI_Init
.extern CTRDG_Init
OS_Init:
    stmdb sp!, {lr}
    sub sp, sp, #4
    bl OS_InitArena
    bl PXI_Init
    bl OS_InitLock
    bl OS_InitIrqTable
    bl OS_InitTick
    bl OS_InitAlarm
    bl OS_InitThread
    bl OS_InitReset
    bl CTRDG_Init
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr

.size OS_Init, . - OS_Init
