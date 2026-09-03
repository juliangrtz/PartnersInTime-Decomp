/* Test whether one of a scene object's four animation channels has state. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_IsAnimationChannelActive, "ax", %progbits
.balign 4

.extern OS_Terminate

.global BattleSceneObject_IsAnimationChannelActive
.type BattleSceneObject_IsAnimationChannelActive, %function
BattleSceneObject_IsAnimationChannelActive:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r1
    mov r5, r0
    cmp r4, #4
    blt .L_valid_channel
    bl OS_Terminate
.L_valid_channel:
    mov r0, #0x28
    mla r0, r4, r0, r5
    ldr r0, [r0, #0x1C]
    cmp r0, #0
    moveq r0, #0
    movne r0, #1
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size BattleSceneObject_IsAnimationChannelActive, . - BattleSceneObject_IsAnimationChannelActive
