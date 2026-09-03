/* Spawn into the first free one of 64 tracked sprite-effect slots. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSpriteEffect_SpawnInFreeSlot, "ax", %progbits
.balign 4

.extern BattleSpriteEffect_SpawnAttached
.extern gBattleContext

.global BattleSpriteEffect_SpawnInFreeSlot
.type BattleSpriteEffect_SpawnInFreeSlot, %function
BattleSpriteEffect_SpawnInFreeSlot:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #0x0C
    ldr r4, .L_battle_context
    mov lr, r1
    ldr r5, [r4]
    mov ip, r2
    mov r4, #0
.L_find_slot:
    cmp r4, #0x40
    addeq sp, sp, #0x0C
    mvneq r0, #0
    popeq {r4, r5, r6, r7, lr}
    bxeq lr
    add r1, r5, r4, lsl #2
    add r1, r1, #0xC000
    ldr r1, [r1, #0xBF8]
    mov r7, r4, lsl #2
    cmp r1, #0
    addne r4, r4, #1
    bne .L_find_slot
    ldr r1, .L_slot_table_offset
    str r3, [sp]
    add r5, r5, r1
    ldr r6, [sp, #0x20]
    mov r1, r0
    mov r2, lr
    mov r3, ip
    add r0, r5, r7
    str r6, [sp, #4]
    bl BattleSpriteEffect_SpawnAttached
    mov r0, r4
    add sp, sp, #0x0C
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_slot_table_offset:
    .word 0xCBF8
.size BattleSpriteEffect_SpawnInFreeSlot, . - BattleSpriteEffect_SpawnInFreeSlot
