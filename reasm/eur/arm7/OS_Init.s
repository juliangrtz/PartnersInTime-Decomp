/* Top-level NitroSDK ARM7 operating-system initialization. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Init, "ax", %progbits
.balign 4

.global OS_Init
.type OS_Init, %function
.extern OS_InitIrqTable
.extern func_037FBBD4
.extern OS_InitThread
.extern OS_InitArena
.extern func_037FD0F4
.extern func_037FD568
.extern OS_InitReset
.extern func_037FE1C4
.extern func_03806D04
OS_Init:
    stmdb sp!, {lr}
    sub sp, sp, #4
    bl OS_InitArena
    bl func_037FE1C4
    bl func_037FBBD4
    bl OS_InitIrqTable
    bl func_037FD0F4
    bl func_037FD568
    bl OS_InitThread
    bl OS_InitReset
    bl func_03806D04
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr

.size OS_Init, . - OS_Init
