/* Run active battle tasks and recycle tasks whose callback was cleared. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleTaskList_Update, "ax", %progbits
.balign 4

.global BattleTaskList_Update
.type BattleTaskList_Update, %function
BattleTaskList_Update:
    push {r4, r5, r6, r7, r8, lr}
    mov r7, r0
    ldr r6, [r7]
    mov r2, r7
    cmp r6, #0
    mov r5, #0
    beq .L_done
    mov r4, r5
    mov r8, r5
.L_next:
    ldr r0, [r6, #8]
    cmp r0, #0
    beq .L_check_callback
    ldr r0, [r0]
    cmp r0, r6
    strne r4, [r6, #4]
.L_check_callback:
    ldr r1, [r6, #4]
    cmp r1, #0
    beq .L_recycle
    mov r0, r6
    blx r1
    mov r2, r6
    add r5, r5, #1
    ldr r6, [r6]
    b .L_continue
.L_recycle:
    ldr r1, [r6, #8]
    cmp r1, #0
    beq .L_unlink
    ldr r0, [r1]
    cmp r0, r6
    streq r8, [r1]
.L_unlink:
    ldr r1, [r6]
    ldr r0, [r7, #4]
    str r0, [r6]
    str r6, [r7, #4]
    mov r6, r1
    str r1, [r2]
.L_continue:
    cmp r6, #0
    bne .L_next
.L_done:
    mov r0, r5
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.size BattleTaskList_Update, . - BattleTaskList_Update
