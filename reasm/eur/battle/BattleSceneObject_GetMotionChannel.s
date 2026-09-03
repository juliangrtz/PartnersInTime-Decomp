/* Resolve one of a scene object's fixed-size motion callback channels. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_GetMotionChannel, "ax", %progbits
.balign 4

.global BattleSceneObject_GetMotionChannel
.type BattleSceneObject_GetMotionChannel, %function
BattleSceneObject_GetMotionChannel:
    add r2, r0, #0x1c
    mov r0, #0x28
    mla r0, r1, r0, r2
    bx lr
.size BattleSceneObject_GetMotionChannel, . - BattleSceneObject_GetMotionChannel
