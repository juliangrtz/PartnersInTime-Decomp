/* Fold this frame's three-axis displacement into the object's travel metric. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_UpdateTravelDistance, "ax", %progbits
.balign 4

.global BattleSceneObject_UpdateTravelDistance
.type BattleSceneObject_UpdateTravelDistance, %function
BattleSceneObject_UpdateTravelDistance:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldrsh r2, [r0, #6]
    ldrsh r1, [r0, #0xc]
    ldrsh ip, [r0, #4]
    ldrsh r3, [r0, #0xa]
    sub r2, r2, r1
    mul r1, r2, r2
    sub r2, ip, r3
    mla r1, r2, r2, r1
    ldrsh lr, [r0, #8]
    ldrsh r2, [r0, #0xe]
    ldr r3, .L_sqrt_control
    mov ip, #0
    sub lr, lr, r2
    mla r2, lr, lr, r1
    ldr r1, .L_sqrt_parameter
    strh ip, [r3]
    lsl r2, r2, #0x10
    str r2, [r1]
.L_wait_for_sqrt:
    ldrh r1, [r3]
    ands r1, r1, #0x8000
    bne .L_wait_for_sqrt
    ldr r1, .L_sqrt_result
    ldrsh r2, [r0, #0x18]
    ldr r1, [r1]
    add r1, r2, r1
    add r1, r1, r1, lsr #31
    asr r1, r1, #1
    strh r1, [r0, #0x18]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_sqrt_control:
    .word 0x040002B0
.L_sqrt_parameter:
    .word 0x040002B8
.L_sqrt_result:
    .word 0x040002B4
.size BattleSceneObject_UpdateTravelDistance, . - BattleSceneObject_UpdateTravelDistance
