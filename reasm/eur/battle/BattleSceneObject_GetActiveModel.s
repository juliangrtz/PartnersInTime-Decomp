/* Select the scene object's primary or alternate active model pointer. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_GetActiveModel, "ax", %progbits
.balign 4

.global BattleSceneObject_GetActiveModel
.type BattleSceneObject_GetActiveModel, %function
BattleSceneObject_GetActiveModel:
    ldr r1, [r0, #0xF4]
    mov r1, r1, lsl #17
    movs r1, r1, lsr #31
    ldrne r0, [r0, #0xC4]
    ldreq r0, [r0, #0xC0]
    bx lr
.size BattleSceneObject_GetActiveModel, . - BattleSceneObject_GetActiveModel
