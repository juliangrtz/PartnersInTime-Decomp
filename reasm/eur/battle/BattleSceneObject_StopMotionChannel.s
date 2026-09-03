/* Stop one motion channel and reconcile its accumulated coordinate deltas. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_StopMotionChannel, "ax", %progbits
.balign 4

.extern BattleSceneObject_GetMotionChannel

.global BattleSceneObject_StopMotionChannel
.type BattleSceneObject_StopMotionChannel, %function
BattleSceneObject_StopMotionChannel:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, r1
    mov r4, r2
    bl BattleSceneObject_GetMotionChannel
    ldr r0, [r0]
    cmp r0, #0
    popeq {r4, r5, r6, lr}
    bxeq lr
    add r0, r6, #0x1c
    mov r1, #0
.L_scan:
    cmp r1, #4
    popeq {r4, r5, r6, lr}
    bxeq lr
    cmp r1, r5
    bne .L_other_channel
    ldr r2, [r0]
    cmp r2, #0
    beq .L_next
    ldrsh r5, [r6, #0x10]
    ldrsh r3, [r0, #0x12]
    mov r2, #0
    add r3, r5, r3
    strh r3, [r6, #0x10]
    ldrsh r5, [r6, #0x12]
    ldrsh r3, [r0, #0x14]
    add r3, r5, r3
    strh r3, [r6, #0x12]
    ldrsh r5, [r6, #0x14]
    ldrsh r3, [r0, #0x16]
    add r3, r5, r3
    strh r3, [r6, #0x14]
    strh r2, [r0, #0x12]
    strh r2, [r0, #0x14]
    strh r2, [r0, #0x16]
    str r2, [r0]
.L_next:
    add r1, r1, #1
    cmp r1, #4
    add r0, r0, #0x28
    popeq {r4, r5, r6, lr}
    bxeq lr
    ldr r2, [r0]
    cmp r2, #0
    popne {r4, r5, r6, lr}
    bxne lr
    ldrh r2, [r0, #0xa]
    cmp r2, #0
    beq .L_next
    cmp r4, #0
    beq .L_commit_target
    ldrsh r3, [r6, #4]
    ldrsh r2, [r0, #0xc]
    add r2, r3, r2
    strh r2, [r6, #4]
    ldrsh r3, [r6, #6]
    ldrsh r2, [r0, #0xe]
    add r2, r3, r2
    strh r2, [r6, #6]
    ldrsh r3, [r6, #8]
    ldrsh r2, [r0, #0x10]
    add r2, r3, r2
    strh r2, [r6, #8]
.L_commit_target:
    ldrsh r5, [r6, #0x10]
    ldrsh r3, [r0, #0xc]
    mov r2, #0
    add r3, r5, r3
    strh r3, [r6, #0x10]
    ldrsh r5, [r6, #0x12]
    ldrsh r3, [r0, #0xe]
    add r3, r5, r3
    strh r3, [r6, #0x12]
    ldrsh r5, [r6, #0x14]
    ldrsh r3, [r0, #0x10]
    add r3, r5, r3
    strh r3, [r6, #0x14]
    strh r2, [r0, #0xc]
    strh r2, [r0, #0xe]
    strh r2, [r0, #0x10]
    strh r2, [r0, #0xa]
    b .L_next
.L_other_channel:
    ldr r2, [r0]
    cmp r2, #0
    beq .L_continue_other
    mov r0, #0x28
    mul r3, r5, r0
    add r4, r6, #0x1c
    add r5, r4, r3
    ldrsh r2, [r5, #0x12]
    mov r1, #0
    mov r0, #1
    strh r2, [r5, #0xc]
    ldrsh r2, [r5, #0x14]
    strh r2, [r5, #0xe]
    ldrsh r2, [r5, #0x16]
    strh r2, [r5, #0x10]
    strh r1, [r5, #0x12]
    strh r1, [r5, #0x14]
    strh r1, [r5, #0x16]
    strh r0, [r5, #0xa]
    str r1, [r4, r3]
    pop {r4, r5, r6, lr}
    bx lr
.L_continue_other:
    add r0, r0, #0x28
    add r1, r1, #1
    b .L_scan
.size BattleSceneObject_StopMotionChannel, . - BattleSceneObject_StopMotionChannel
