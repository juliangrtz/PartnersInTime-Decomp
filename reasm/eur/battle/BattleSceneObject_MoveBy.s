/* Apply a scene-object delta immediately or interpolate it over a duration. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_MoveBy, "ax", %progbits
.balign 4

.extern BattleSceneObject_UpdateMoveBy
.extern func_ov002_020a4e08

.global BattleSceneObject_MoveBy
.type BattleSceneObject_MoveBy, %function
BattleSceneObject_MoveBy:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r2
    ldr r2, [sp, #0x14]
    mov r4, r3
    cmp r2, #0
    bgt .L_interpolate
    ldrsh r1, [r0, #4]
    ldr r2, [sp, #0x10]
    add sp, sp, #4
    add r1, r1, r5
    strh r1, [r0, #4]
    ldrsh r1, [r0, #6]
    add r1, r1, r4
    strh r1, [r0, #6]
    ldrsh r1, [r0, #8]
    add r1, r1, r2
    strh r1, [r0, #8]
    ldrsh r1, [r0, #0x10]
    add r1, r1, r5
    strh r1, [r0, #0x10]
    ldrsh r1, [r0, #0x12]
    add r1, r1, r4
    strh r1, [r0, #0x12]
    ldrsh r1, [r0, #0x14]
    add r1, r1, r2
    strh r1, [r0, #0x14]
    pop {r4, r5, lr}
    bx lr
.L_interpolate:
    ldr r3, .L_update_callback
    bl func_ov002_020a4e08
    strh r5, [r0]
    ldr r1, [sp, #0x10]
    strh r4, [r0, #2]
    strh r1, [r0, #4]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_update_callback:
    .word BattleSceneObject_UpdateMoveBy
.size BattleSceneObject_MoveBy, . - BattleSceneObject_MoveBy
