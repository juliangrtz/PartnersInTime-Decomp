/* Preserve a scene object's current coordinates as its motion origin. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_SnapshotPosition, "ax", %progbits
.balign 4

.global BattleSceneObject_SnapshotPosition
.type BattleSceneObject_SnapshotPosition, %function
BattleSceneObject_SnapshotPosition:
    ldrsh r1, [r0, #4]
    strh r1, [r0, #0xa]
    ldrsh r1, [r0, #6]
    strh r1, [r0, #0xc]
    ldrsh r1, [r0, #8]
    strh r1, [r0, #0xe]
    bx lr
.size BattleSceneObject_SnapshotPosition, . - BattleSceneObject_SnapshotPosition
