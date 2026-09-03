/* Return true when current HP is at or below 25 percent of maximum HP. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleActor_IsHpAtMostQuarter, "ax", %progbits
.balign 4

.global BattleActor_IsHpAtMostQuarter
.type BattleActor_IsHpAtMostQuarter, %function
BattleActor_IsHpAtMostQuarter:
    ldrsh r3, [r0, #6]
    ldrsh r2, [r0, #4]
    mov r0, #100
    mov r1, #25
    smulbb r3, r3, r0
    smulbb r0, r2, r1
    cmp r3, r0
    movle r0, #1
    movgt r0, #0
    bx lr
.size BattleActor_IsHpAtMostQuarter, . - BattleActor_IsHpAtMostQuarter
