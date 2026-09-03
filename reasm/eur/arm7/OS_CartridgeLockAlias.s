/* Legacy spelling retained by NitroSDK for source compatibility. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_CartridgeLockAlias, "ax", %progbits
.balign 4

.extern OS_UnlockCartridge

.global OS_UnLockCartridge
.type OS_UnLockCartridge, %function
OS_UnLockCartridge:
    ldr r1, .L_unlock_cartridge
    bx r1
.L_unlock_cartridge:
    .word OS_UnlockCartridge
.size OS_UnLockCartridge, . - OS_UnLockCartridge
