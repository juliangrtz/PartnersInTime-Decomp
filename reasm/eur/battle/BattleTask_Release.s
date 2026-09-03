/* Clear an active task or return a not-yet-inserted task to its pool. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleTask_Release, "ax", %progbits
.balign 4

.global BattleTask_Release
.type BattleTask_Release, %function
BattleTask_Release:
    ldr r2, [r0, #8]
    cmp r2, #0
    beq .L_check_callback
    ldr r1, [r2]
    cmp r1, r0
    moveq r1, #0
    streq r1, [r2]
.L_check_callback:
    ldr r1, [r0, #4]
    cmp r1, #0
    ldreq r2, [r0]
    ldreq r1, [r2, #4]
    streq r1, [r0]
    streq r0, [r2, #4]
    movne r1, #0
    strne r1, [r0, #4]
    bx lr
.size BattleTask_Release, . - BattleTask_Release
