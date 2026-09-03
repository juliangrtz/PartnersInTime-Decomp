/* Apply a delta while respecting membership in the active motion list. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_AdjustPosition, "ax", %progbits
.balign 4

.extern BattleSceneObject_AddPositionDelta
.extern BattleSceneObject_MoveByImmediate
.extern gBattleMotionObjectList

.global BattleSceneObject_AdjustPosition
.type BattleSceneObject_AdjustPosition, %function
BattleSceneObject_AdjustPosition:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr ip, .L_motion_list
    ldr ip, [ip]
.L_scan:
    cmp ip, #0
    bne .L_compare
    bl BattleSceneObject_MoveByImmediate
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_compare:
    cmp ip, r0
    bne .L_next
    bl BattleSceneObject_AddPositionDelta
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_next:
    ldr ip, [ip]
    b .L_scan
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_motion_list:
    .word gBattleMotionObjectList
.size BattleSceneObject_AdjustPosition, . - BattleSceneObject_AdjustPosition
