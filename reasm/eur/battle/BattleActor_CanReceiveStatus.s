/* Test whether a live actor is currently eligible for status application. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleActor_CanReceiveStatus, "ax", %progbits
.balign 4

.extern gSaveData

.global BattleActor_CanReceiveStatus
.type BattleActor_CanReceiveStatus, %function
BattleActor_CanReceiveStatus:
    ldrsh r1, [r0, #6]
    cmp r1, #0
    movle r0, #0
    bxle lr
    ldr r1, [r0]
    ldrh r1, [r1, #0xEC]
    cmp r1, #0x3C
    blo .L_party
    ldr r0, [r0, #0x6C]
    cmp r0, #0
    movne r0, #1
    moveq r0, #0
    bx lr
.L_party:
    cmp r1, #0x3A
    movlo r0, #1
    bxlo lr
    ldr r0, .L_save_data
    ldr r0, [r0]
    add r0, r0, #0x500
    ldrsh r0, [r0, #0x58]
    cmp r0, #2
    moveq r0, #1
    movne r0, #0
    bx lr
.L_save_data:
    .word gSaveData
.size BattleActor_CanReceiveStatus, . - BattleActor_CanReceiveStatus
