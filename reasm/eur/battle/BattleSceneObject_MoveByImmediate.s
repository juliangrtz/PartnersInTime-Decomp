/* Apply a coordinate delta and synchronize both stored target positions. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_MoveByImmediate, "ax", %progbits
.balign 4

.global BattleSceneObject_MoveByImmediate
.type BattleSceneObject_MoveByImmediate, %function
BattleSceneObject_MoveByImmediate:
    ldrsh ip, [r0, #4]
    strh ip, [r0, #0xa]
    ldrsh ip, [r0, #6]
    strh ip, [r0, #0xc]
    ldrsh ip, [r0, #8]
    strh ip, [r0, #0xe]
    ldrsh ip, [r0, #4]
    add r1, ip, r1
    strh r1, [r0, #4]
    ldrsh r1, [r0, #6]
    add r1, r1, r2
    strh r1, [r0, #6]
    ldrsh r1, [r0, #8]
    add r1, r1, r3
    strh r1, [r0, #8]
    ldrsh r1, [r0, #4]
    strh r1, [r0, #0x10]
    ldrsh r1, [r0, #6]
    strh r1, [r0, #0x12]
    ldrsh r1, [r0, #8]
    strh r1, [r0, #0x14]
    bx lr
.size BattleSceneObject_MoveByImmediate, . - BattleSceneObject_MoveByImmediate
