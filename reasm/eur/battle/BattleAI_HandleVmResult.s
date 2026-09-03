/* Finish a VM task or activate the continuation script saved in its state. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_HandleVmResult, "ax", %progbits
.balign 4

.global BattleAI_HandleVmResult
.type BattleAI_HandleVmResult, %function
BattleAI_HandleVmResult:
    cmp r1, #1
    beq .L_finish
    cmp r1, #2
    beq .L_try_continuation
    cmp r1, #3
    b .L_return_zero
.L_try_continuation:
    ldr r1, [r2, #0xb8]
    cmp r1, #0
    beq .L_finish
    str r1, [r2]
    ldrh r0, [r2, #0xbc]
    mov r3, #0
    ldr r1, .L_no_order
    strh r0, [r2, #0xb4]
    ldrh ip, [r2, #0xbe]
    mov r0, #1
    strh ip, [r2, #0xb6]
    str r3, [r2, #0xb8]
    strh r3, [r2, #0xbc]
    strh r1, [r2, #0xbe]
    bx lr
.L_finish:
    mov r1, #0
    str r1, [r0, #4]
    ldr r0, .L_no_order
    strh r1, [r2, #0xb4]
    strh r0, [r2, #0xb6]
    str r1, [r2, #0xb8]
    strh r1, [r2, #0xbc]
    strh r0, [r2, #0xbe]
.L_return_zero:
    mov r0, #0
    bx lr
.L_no_order:
    .word 0xffff
.size BattleAI_HandleVmResult, . - BattleAI_HandleVmResult
