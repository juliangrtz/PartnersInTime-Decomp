/* Decode a packed object ID and return its 44-byte battle descriptor slot. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleObjectData_ResolveSlot, "ax", %progbits
.balign 4

.equ BATTLE_OBJECT_SLOT_SIZE, 0x2C
.equ BATTLE_OBJECT_SLOT_BASE, 0x284
.equ BATTLE_OBJECT_SLOT_ID_BASE, 0x200
.equ BATTLE_OBJECT_SLOT_ID_OFFSET, 0xA8
.equ BATTLE_OBJECT_ID_MASK, 0x0FFF

.extern gBattleContext

.global BattleObjectData_ResolveSlot
.type BattleObjectData_ResolveSlot, %function
BattleObjectData_ResolveSlot:
    mov r1, r0, asr #12
    sub r2, r1, #1
    mov r1, #BATTLE_OBJECT_SLOT_SIZE
    mul r12, r2, r1
    ldr r2, .L_battle_context
    ldr r1, .L_object_id_mask
    ldr r3, [r2]
    and r1, r0, r1
    add r0, r3, r12
    add r0, r0, #BATTLE_OBJECT_SLOT_ID_BASE
    strh r1, [r0, #BATTLE_OBJECT_SLOT_ID_OFFSET]
    ldr r0, [r2]
    add r0, r0, #BATTLE_OBJECT_SLOT_BASE
    add r0, r0, r12
    bx lr

.L_battle_context:
    .word gBattleContext
.L_object_id_mask:
    .word BATTLE_OBJECT_ID_MASK
.size BattleObjectData_ResolveSlot, . - BattleObjectData_ResolveSlot
