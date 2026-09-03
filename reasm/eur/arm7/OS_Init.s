/* Top-level NitroSDK ARM7 operating-system initialization. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Init, "ax", %progbits
.balign 4

.global OS_Init
.type OS_Init, %function
.extern func_037FB908
.extern func_037FBBD4
.extern func_037FC188
.extern func_037FC938
.extern func_037FD0F4
.extern func_037FD568
.extern func_037FDE18
.extern func_037FE1C4
.extern func_03806D04
OS_Init:
    stmdb sp!, {lr}
    sub sp, sp, #4
    bl func_037FC938
    bl func_037FE1C4
    bl func_037FBBD4
    bl func_037FB908
    bl func_037FD0F4
    bl func_037FD568
    bl func_037FC188
    bl func_037FDE18
    bl func_03806D04
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr

.size OS_Init, . - OS_Init
