/* Offset a scene object's live and stored target coordinates together. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_AddPositionDelta, "ax", %progbits
.balign 4

.global BattleSceneObject_AddPositionDelta
.type BattleSceneObject_AddPositionDelta, %function
BattleSceneObject_AddPositionDelta:
    ldrsh ip, [r0, #4]
    add ip, ip, r1
    strh ip, [r0, #4]
    ldrsh ip, [r0, #6]
    add ip, ip, r2
    strh ip, [r0, #6]
    ldrsh ip, [r0, #8]
    add ip, ip, r3
    strh ip, [r0, #8]
    ldrsh ip, [r0, #0x10]
    add r1, ip, r1
    strh r1, [r0, #0x10]
    ldrsh r1, [r0, #0x12]
    add r1, r1, r2
    strh r1, [r0, #0x12]
    ldrsh r1, [r0, #0x14]
    add r1, r1, r3
    strh r1, [r0, #0x14]
    bx lr
.size BattleSceneObject_AddPositionDelta, . - BattleSceneObject_AddPositionDelta
