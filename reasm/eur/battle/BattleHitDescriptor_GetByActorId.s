/* Map party/enemy object IDs onto the battle context's 16-byte hit descriptors. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleHitDescriptor_GetByActorId, "ax", %progbits
.balign 4

.equ BATTLE_CONTEXT_HIT_DESCRIPTORS, 0xC8F4
.equ HIT_DESCRIPTOR_SIZE_SHIFT, 4

.extern gBattleContext

.global BattleHitDescriptor_GetByActorId
.type BattleHitDescriptor_GetByActorId, %function
BattleHitDescriptor_GetByActorId:
    cmp r0, #0x0A
    addlo r0, r0, #0x14
    movlo r0, r0, lsl #16
    movlo r0, r0, lsr #16
    blo .L_index_ready
    cmp r0, #0x38
    sublo r0, r0, #0x28
    movlo r0, r0, lsl #16
    movlo r0, r0, lsr #16
    subhs r0, r0, #0x28
    movhs r0, r0, lsl #16
    movhs r0, r0, lsr #16
.L_index_ready:
    ldr r2, .L_battle_context
    ldr r1, .L_descriptor_offset
    ldr r2, [r2]
    add r1, r2, r1
    add r0, r1, r0, lsl #HIT_DESCRIPTOR_SIZE_SHIFT
    bx lr
.L_battle_context:
    .word gBattleContext
.L_descriptor_offset:
    .word BATTLE_CONTEXT_HIT_DESCRIPTORS
.size BattleHitDescriptor_GetByActorId, . - BattleHitDescriptor_GetByActorId
