/* Reset a VM state and select the executable stream encoded by a script block. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_InitStateFromScriptBlock, "ax", %progbits
.balign 4

.extern func_0202cbd4

.global BattleAI_InitStateFromScriptBlock
.type BattleAI_InitStateFromScriptBlock, %function
BattleAI_InitStateFromScriptBlock:
    push {r4, r5, r6, lr}
    mov r5, r1
    mov r4, r2
    mov r1, #0
    mov r2, #0xb8
    mov r6, r0
    bl func_0202cbd4
    ldrh r0, [r5]
    lsr r0, r0, #1
    add r0, r5, r0, lsl #1
    str r0, [r6]
    strh r4, [r6, #0xb0]
    pop {r4, r5, r6, lr}
    bx lr
.size BattleAI_InitStateFromScriptBlock, . - BattleAI_InitStateFromScriptBlock
