/* Resolve a scene-object ID and test one of its animation channels. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_IsAnimationActiveById, "ax", %progbits
.balign 4

.extern BattleSceneObject_GetById
.extern BattleSceneObject_IsAnimationChannelActive
.extern OS_Terminate

.global BattleSceneObject_IsAnimationActiveById
.type BattleSceneObject_IsAnimationActiveById, %function
BattleSceneObject_IsAnimationActiveById:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, r1
    cmp r5, #0x46
    blo .L_valid_id
    bl OS_Terminate
.L_valid_id:
    mov r0, r5
    bl BattleSceneObject_GetById
    mov r1, r4
    bl BattleSceneObject_IsAnimationChannelActive
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size BattleSceneObject_IsAnimationActiveById, . - BattleSceneObject_IsAnimationActiveById
