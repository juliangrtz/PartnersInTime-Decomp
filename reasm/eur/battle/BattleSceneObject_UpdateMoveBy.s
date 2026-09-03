/* Apply one fixed-point interpolation step for BattleSceneObject_MoveBy. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_UpdateMoveBy, "ax", %progbits
.balign 4

.extern _s32_div_f

.global BattleSceneObject_UpdateMoveBy
.type BattleSceneObject_UpdateMoveBy, %function
BattleSceneObject_UpdateMoveBy:
    push {r4, r5, r6, lr}
    mov r6, r1
    ldr r2, [r6, #4]
    ldrsh r1, [r6, #8]
    mov r5, r0
    mov r0, r2, lsl #4
    add r4, r6, #0x18
    bl _s32_div_f
    ldrsh r1, [r6, #0x18]
    ldrsh r3, [r5, #4]
    mul r2, r1, r0
    mov r1, r2, asr #11
    add r1, r2, r1, lsr #20
    add r1, r3, r1, asr #12
    strh r1, [r5, #4]
    ldrsh r1, [r4, #2]
    ldrsh r3, [r5, #6]
    mul r2, r1, r0
    mov r1, r2, asr #11
    add r1, r2, r1, lsr #20
    add r1, r3, r1, asr #12
    strh r1, [r5, #6]
    ldrsh r1, [r4, #4]
    ldrsh r3, [r5, #8]
    mul r2, r1, r0
    mov r0, r2, asr #11
    add r0, r2, r0, lsr #20
    add r0, r3, r0, asr #12
    strh r0, [r5, #8]
    pop {r4, r5, r6, lr}
    bx lr
.size BattleSceneObject_UpdateMoveBy, . - BattleSceneObject_UpdateMoveBy
