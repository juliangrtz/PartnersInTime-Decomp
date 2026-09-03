/* Clear every ailment and temporary stat modifier from one battle actor. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleStatus_ClearAll, "ax", %progbits
.balign 4

.extern BattleStatus_ClearEffect

.global BattleStatus_ClearAll
.type BattleStatus_ClearAll, %function
BattleStatus_ClearAll:
    push {r4, lr}
    mov r4, r0

    mov r1, #1
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #2
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #3
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #4
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #5
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #6
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #7
    bl BattleStatus_ClearEffect
    mov r0, r4
    mov r1, #8
    bl BattleStatus_ClearEffect

    pop {r4, lr}
    bx lr
.size BattleStatus_ClearAll, . - BattleStatus_ClearAll
