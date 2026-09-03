/* Move a scene object to absolute coordinates immediately or over a duration. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_MoveTo, "ax", %progbits
.balign 4

.extern BattleSceneObject_AdjustPosition
.extern BattleSceneObject_BeginMotionChannel
.extern BattleSceneObject_UpdateMoveTo

.global BattleSceneObject_MoveTo
.type BattleSceneObject_MoveTo, %function
BattleSceneObject_MoveTo:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r2
    ldr r2, [sp, #0x14]
    mov r4, r3
    cmp r2, #0
    bgt .L_interpolate
    ldrsh r1, [r0, #4]
    ldrsh r2, [r0, #6]
    ldrsh r3, [r0, #8]
    ldr ip, [sp, #0x10]
    sub r1, r5, r1
    sub r2, r4, r2
    sub r3, ip, r3
    bl BattleSceneObject_AdjustPosition
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_interpolate:
    ldr r3, .L_update_callback
    bl BattleSceneObject_BeginMotionChannel
    strh r5, [r0]
    ldr r1, [sp, #0x10]
    strh r4, [r0, #2]
    strh r1, [r0, #4]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_update_callback:
    .word BattleSceneObject_UpdateMoveTo
.size BattleSceneObject_MoveTo, . - BattleSceneObject_MoveTo
