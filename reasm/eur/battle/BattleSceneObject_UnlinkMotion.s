/* Remove a scene object from the active motion list and preserve its target. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_UnlinkMotion, "ax", %progbits
.balign 4

.extern gBattleMotionObjectList

.global BattleSceneObject_UnlinkMotion
.type BattleSceneObject_UnlinkMotion, %function
BattleSceneObject_UnlinkMotion:
    ldr r1, .L_motion_list
    mov r2, #0
    ldr r1, [r1]
    cmp r1, #0
    bxeq lr
.L_scan:
    cmp r1, r0
    movne r2, r1
    bne .L_next
    ldrsh r1, [r0, #4]
    cmp r2, #0
    strh r1, [r0, #0x10]
    ldrsh r1, [r0, #6]
    strh r1, [r0, #0x12]
    ldrsh r1, [r0, #8]
    strh r1, [r0, #0x14]
    ldrne r0, [r0]
    strne r0, [r2]
    ldreq r1, [r0]
    ldreq r0, .L_motion_list
    streq r1, [r0]
    bx lr
.L_next:
    ldr r1, [r1]
    cmp r1, #0
    bne .L_scan
    bx lr
.L_motion_list:
    .word gBattleMotionObjectList
.size BattleSceneObject_UnlinkMotion, . - BattleSceneObject_UnlinkMotion
