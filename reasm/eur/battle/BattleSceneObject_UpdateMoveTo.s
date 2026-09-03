/* Interpolate a scene object toward an absolute three-axis target. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_UpdateMoveTo, "ax", %progbits
.balign 4

.extern _s32_div_f

.global BattleSceneObject_UpdateMoveTo
.type BattleSceneObject_UpdateMoveTo, %function
BattleSceneObject_UpdateMoveTo:
    push {r4, r5, r6, lr}
    mov r6, r1
    ldr r2, [r6, #4]
    ldrsh r1, [r6, #8]
    mov r5, r0
    lsl r0, r2, #4
    add r4, r6, #0x18
    bl _s32_div_f
    ldrsh r3, [r5, #4]
    ldrsh r1, [r6, #0x18]
    sub r1, r1, r3
    mul r2, r0, r1
    asr r1, r2, #0xb
    add r1, r2, r1, lsr #20
    add r1, r3, r1, asr #12
    strh r1, [r5, #4]
    ldrsh r3, [r5, #6]
    ldrsh r1, [r4, #2]
    sub r1, r1, r3
    mul r2, r0, r1
    asr r1, r2, #0xb
    add r1, r2, r1, lsr #20
    add r1, r3, r1, asr #12
    strh r1, [r5, #6]
    ldrsh r2, [r5, #8]
    ldrsh r1, [r4, #4]
    sub r1, r1, r2
    mul r1, r0, r1
    asr r0, r1, #0xb
    add r0, r1, r0, lsr #20
    add r0, r2, r0, asr #12
    strh r0, [r5, #8]
    pop {r4, r5, r6, lr}
    bx lr
.size BattleSceneObject_UpdateMoveTo, . - BattleSceneObject_UpdateMoveTo
