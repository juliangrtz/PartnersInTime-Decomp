/* Test all collision-bound pairs for two objects and write their hit position. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleCollision_TestObjects, "ax", %progbits
.balign 4

.extern BattleCollision_GetBounds
.extern BattleSceneObject_GetById
.extern BattleCollision_TestVolumes
.extern func_ov002_020a3370

.global BattleCollision_TestObjects
.type BattleCollision_TestObjects, %function
BattleCollision_TestObjects:
    push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    sub sp, sp, #0x54
    str r0, [sp, #4]
    mov r0, r0, lsl #16
    mov r6, r1
    mov r0, r0, lsr #16
    str r2, [sp, #8]
    bl BattleSceneObject_GetById
    mov r1, r6, lsl #16
    mov fp, r0
    mov r0, r1, lsr #16
    bl BattleSceneObject_GetById
    ldr r1, [sp, #4]
    mov r5, r0
    mov r1, r1, lsl #16
    mov r0, r1, lsr #16
    bl BattleSceneObject_GetById
    bl func_ov002_020a3370
    mov r0, r6, lsl #16
    mov r0, r0, lsr #16
    bl BattleSceneObject_GetById
    bl func_ov002_020a3370
    cmp r0, #0
    addeq sp, sp, #0x54
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr
    ldr r0, [r0, #0x7C]
    mov r0, r0, lsl #23
    movs r0, r0, lsr #31
    addeq sp, sp, #0x54
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr

    mov r0, #0
    str r0, [sp, #0x0C]
    str r0, [sp, #0x1C]
    str r0, [sp, #0x14]
    str r0, [sp, #0x18]
    mvn r0, #0
    str r0, [sp, #0x10]

.L_source_loop:
    ldr r1, [sp, #4]
    ldr r2, [sp, #0x0C]
    ldr r3, [sp, #0x10]
    add r0, sp, #0x20
    bl BattleCollision_GetBounds
    cmp r0, #0
    addeq sp, sp, #0x54
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr
    ldr r1, [sp, #4]
    ldr r2, [sp, #0x0C]
    ldr r3, [sp, #0x14]
    add r0, sp, #0x2C
    bl BattleCollision_GetBounds
    cmp r0, #0
    addeq sp, sp, #0x54
    moveq r0, #0
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr

    ldrsh r1, [sp, #0x20]
    ldrsh r0, [fp, #0x0A]
    ldrsh r4, [sp, #0x22]
    ldrsh r3, [sp, #0x24]
    add r0, r1, r0
    strh r0, [sp, #0x20]
    ldrsh r0, [fp, #0x0A]
    ldrsh r2, [sp, #0x26]
    ldrsh r1, [sp, #0x28]
    add r0, r4, r0
    strh r0, [sp, #0x22]
    ldrsh r0, [fp, #0x0E]
    ldrsh sl, [sp, #0x2A]
    ldrsh sb, [sp, #0x2C]
    add r0, r3, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x24]
    ldrsh r0, [fp, #0x0E]
    ldrsh r8, [sp, #0x2E]
    ldrsh r7, [sp, #0x30]
    add r0, r2, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x26]
    ldrsh r0, [fp, #0x0C]
    ldrsh r3, [sp, #0x32]
    ldrsh r2, [sp, #0x34]
    add r0, r1, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x28]
    ldrsh r0, [fp, #0x0C]
    ldrsh r1, [sp, #0x36]
    ldr r4, [sp, #0x18]
    add r0, sl, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x2A]
    ldrsh r0, [fp, #4]
    add r0, sb, r0
    strh r0, [sp, #0x2C]
    ldrsh r0, [fp, #4]
    add r0, r8, r0
    strh r0, [sp, #0x2E]
    ldrsh r0, [fp, #8]
    add r0, r7, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x30]
    ldrsh r0, [fp, #8]
    add r0, r3, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x32]
    ldrsh r0, [fp, #6]
    add r0, r2, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x34]
    ldrsh r0, [fp, #6]
    add r0, r1, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x36]

.L_target_loop:
    ldr r3, [sp, #0x10]
    add r0, sp, #0x38
    mov r1, r6
    mov r2, r4
    bl BattleCollision_GetBounds
    cmp r0, #0
    beq .L_next_source
    ldr r3, [sp, #0x1C]
    add r0, sp, #0x44
    mov r1, r6
    mov r2, r4
    bl BattleCollision_GetBounds
    cmp r0, #0
    beq .L_next_source

    ldrsh r1, [sp, #0x38]
    ldrsh r0, [r5, #0x0A]
    ldrsh r2, [sp, #0x3A]
    ldrsh r7, [sp, #0x3C]
    add r0, r1, r0
    strh r0, [sp, #0x38]
    ldrsh r0, [r5, #0x0A]
    ldrsh r3, [sp, #0x3E]
    ldrsh r1, [sp, #0x40]
    add r0, r2, r0
    strh r0, [sp, #0x3A]
    ldrsh r0, [r5, #0x0E]
    ldrsh r2, [sp, #0x42]
    ldrsh sb, [sp, #0x44]
    add r0, r7, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x3C]
    ldrsh r0, [r5, #0x0E]
    ldrsh sl, [sp, #0x46]
    ldrsh r8, [sp, #0x48]
    add r0, r3, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x3E]
    ldrsh r0, [r5, #0x0C]
    ldrsh r7, [sp, #0x4A]
    ldrsh lr, [sp, #0x4C]
    add r0, r1, r0
    rsb r0, r0, #0
    strh r0, [sp, #0x40]
    ldrsh r1, [r5, #0x0C]
    ldrsh ip, [sp, #0x4E]
    add r0, sp, #0x20
    add r1, r2, r1
    rsb r1, r1, #0
    strh r1, [sp, #0x42]
    ldrsh r3, [r5, #4]
    add r1, sp, #0x2C
    add r2, sp, #0x38
    add r3, sb, r3
    strh r3, [sp, #0x44]
    ldrsh sb, [r5, #4]
    add r3, sp, #0x44
    add sb, sl, sb
    strh sb, [sp, #0x46]
    ldrsh sb, [r5, #8]
    add r8, r8, sb
    rsb r8, r8, #0
    strh r8, [sp, #0x48]
    ldrsh r8, [r5, #8]
    add r7, r7, r8
    rsb r7, r7, #0
    strh r7, [sp, #0x4A]
    ldrsh r7, [r5, #6]
    add r7, lr, r7
    rsb r7, r7, #0
    strh r7, [sp, #0x4C]
    ldrsh r7, [r5, #6]
    add r7, ip, r7
    rsb r7, r7, #0
    strh r7, [sp, #0x4E]
    ldr r7, [sp, #8]
    str r7, [sp]
    bl BattleCollision_TestVolumes
    cmp r0, #0
    addne sp, sp, #0x54
    popne {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxne lr
    add r4, r4, #1
    b .L_target_loop

.L_next_source:
    ldr r0, [sp, #0x0C]
    add r0, r0, #1
    str r0, [sp, #0x0C]
    b .L_source_loop
.size BattleCollision_TestObjects, . - BattleCollision_TestObjects
