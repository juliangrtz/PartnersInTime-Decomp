/* Detect which ARM7 WRAM alias is visible and publish the result. */

.syntax unified
.arch armv4t
.arm
.section .pit_ARM7_MemoryAliasTest, "ax", %progbits
.balign 4

.global ARM7_MemoryAliasTest
.type ARM7_MemoryAliasTest, %function
.extern HW_ARM7MemoryAliasProbe
ARM7_MemoryAliasTest:
    mov r0, #1
    mov r1, #0
    ldr r2, .L_alias_probe
    sub r3, r2, #0x400000
.Lprobe:
    strh r1, [r2]
    ldrh r12, [r3]
    cmp r1, r12
    movne r0, #2
    bne .Ldone
    add r1, r1, #1
    cmp r1, #2
    bne .Lprobe
.Ldone:
    strh r0, [r2]
    bx lr

.L_alias_probe:
    .word HW_ARM7MemoryAliasProbe

.size ARM7_MemoryAliasTest, . - ARM7_MemoryAliasTest
