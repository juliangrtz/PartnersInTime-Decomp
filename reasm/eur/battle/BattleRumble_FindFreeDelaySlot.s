/* Return the first of four unused deferred battle screen-effect slots. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleRumble_FindFreeDelaySlot, "ax", %progbits
.balign 4

.extern data_ov002_020c0660

.global BattleRumble_FindFreeDelaySlot
.type BattleRumble_FindFreeDelaySlot, %function
BattleRumble_FindFreeDelaySlot:
    ldr r1, .L_runtime_context
    mov r0, #0
    ldr r2, [r1]
.L_scan:
    add r1, r2, #0x11000
    ldr r1, [r1, #0x35c]
    cmp r1, #0
    bxeq lr
    add r0, r0, #1
    cmp r0, #4
    add r2, r2, #0xc
    blt .L_scan
    mvn r0, #0
    bx lr
.L_runtime_context:
    .word data_ov002_020c0660
.size BattleRumble_FindFreeDelaySlot, . - BattleRumble_FindFreeDelaySlot
