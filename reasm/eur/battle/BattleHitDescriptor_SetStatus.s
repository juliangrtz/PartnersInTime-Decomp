/* Store the optional status payload packed into a hit descriptor. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleHitDescriptor_SetStatus, "ax", %progbits
.balign 4

.equ HIT_DESCRIPTOR_FLAGS, 0x0C
.equ HIT_DESCRIPTOR_STATUS_CHANCE, 0x0E
.equ HIT_DESCRIPTOR_STATUS_MAGNITUDE, 0x0F

.global BattleHitDescriptor_SetStatus
.type BattleHitDescriptor_SetStatus, %function
BattleHitDescriptor_SetStatus:
    ldrh ip, [r0, #HIT_DESCRIPTOR_FLAGS]
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    bic ip, ip, #0xFE00
    and r1, r1, #0x7F
    orr r1, ip, r1, lsl #9
    strh r1, [r0, #HIT_DESCRIPTOR_FLAGS]
    strb r2, [r0, #HIT_DESCRIPTOR_STATUS_CHANCE]
    strb r3, [r0, #HIT_DESCRIPTOR_STATUS_MAGNITUDE]
    bx lr
.size BattleHitDescriptor_SetStatus, . - BattleHitDescriptor_SetStatus
