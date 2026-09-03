/* Store a battle position relative to the selected view offset. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattlePosition_StoreViewRelative, "ax", %progbits
.balign 4

.equ BATTLE_CONTEXT_VIEW0_X, 0xCB9C
.equ BATTLE_CONTEXT_VIEW0_Y, 0xCB9E
.equ BATTLE_CONTEXT_VIEW1_X, 0xCBA0
.equ BATTLE_CONTEXT_VIEW1_Y, 0xCBA2

.extern gBattleContext

.global BattlePosition_StoreViewRelative
.type BattlePosition_StoreViewRelative, %function
BattlePosition_StoreViewRelative:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr ip, [sp, #8]
    cmp ip, #1
    strheq r1, [r0]
    strheq r2, [r0, #2]
    beq .L_store_z

    ldr ip, [sp, #0x0C]
    cmp ip, #0
    bne .L_view1

    ldr lr, .L_battle_context
    ldr ip, [lr]
    add ip, ip, #0xCB00
    ldrsh ip, [ip, #0x9C]
    sub r1, r1, ip
    strh r1, [r0]
    ldr r1, [lr]
    add r1, r1, #0xCB00
    ldrsh r1, [r1, #0x9E]
    sub r1, r2, r1
    strh r1, [r0, #2]
    b .L_store_z

.L_view1:
    ldr lr, .L_battle_context
    ldr ip, [lr]
    add ip, ip, #0xCB00
    ldrsh ip, [ip, #0xA0]
    sub r1, r1, ip
    strh r1, [r0]
    ldr r1, [lr]
    add r1, r1, #0xCB00
    ldrsh r1, [r1, #0xA2]
    sub r1, r2, r1
    strh r1, [r0, #2]

.L_store_z:
    cmp r3, #0
    movlt r3, #0
    strh r3, [r0, #4]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.size BattlePosition_StoreViewRelative, . - BattlePosition_StoreViewRelative
