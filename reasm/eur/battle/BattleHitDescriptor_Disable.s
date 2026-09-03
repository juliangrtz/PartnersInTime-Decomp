/* Disable one per-actor hit descriptor by clearing its dispatch callback. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleHitDescriptor_Disable, "ax", %progbits
.balign 4

.equ HIT_DESCRIPTOR_CALLBACK, 0x04

.global BattleHitDescriptor_Disable
.type BattleHitDescriptor_Disable, %function
BattleHitDescriptor_Disable:
    mov r1, #0
    str r1, [r0, #HIT_DESCRIPTOR_CALLBACK]
    bx lr
.size BattleHitDescriptor_Disable, . - BattleHitDescriptor_Disable
