/* Resolve the six-halfword collision bounds used while expanding battle hits. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleCollision_GetBounds, "ax", %progbits
.balign 4

.extern BattleActor_GetById
.extern BattleActor_GetPartySlot
.extern BattleSceneObject_GetById
.extern BattleModel_GetAnimationBounds
.extern BattleSceneObject_GetActiveModel

.global BattleCollision_GetBounds
.type BattleCollision_GetBounds, %function
BattleCollision_GetBounds:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r6, r1
    sub r1, r6, #0x38
    mov r7, r0
    mov r5, r2
    mov r4, r3
    cmp r1, #1
    bhi .L_check_special_objects
    cmp r5, #0
    addgt sp, sp, #4
    movgt r0, #0
    popgt {r4, r5, r6, r7, lr}
    bxgt lr
    mov r0, r6, lsl #16
    mov r0, r0, lsr #16
    bl BattleActor_GetPartySlot
    ldrh r0, [r0, #0x7E]
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_success
    b .L_party_large_0
    b .L_party_large_1
    b .L_party_small_0
    b .L_party_small_1
    b .L_party_wide_0
    b .L_party_wide_1

.L_party_large_0:
    mvn r0, #2
    strh r0, [r7]
    mov r0, #6
    strh r0, [r7, #2]
    mov r0, #0x1E
    strh r0, [r7, #4]
    mov r0, #8
    strh r0, [r7, #6]
    mov r0, #0
    strh r0, [r7, #8]
    mvn r0, #3
    strh r0, [r7, #0x0A]
    b .L_success
.L_party_large_1:
    mvn r0, #2
    strh r0, [r7]
    mov r0, #6
    strh r0, [r7, #2]
    mov r0, #0x1E
    strh r0, [r7, #4]
    mov r0, #8
    strh r0, [r7, #6]
    mov r0, #0
    strh r0, [r7, #8]
    mvn r0, #3
    strh r0, [r7, #0x0A]
    b .L_success
.L_party_small_0:
    mvn r0, #2
    strh r0, [r7]
    mov r0, #3
    strh r0, [r7, #2]
    mov r0, #0x15
    strh r0, [r7, #4]
    mov r0, #6
    strh r0, [r7, #6]
    mov r0, #0
    strh r0, [r7, #8]
    mvn r0, #3
    strh r0, [r7, #0x0A]
    b .L_success
.L_party_small_1:
    mvn r0, #2
    strh r0, [r7]
    mov r0, #3
    strh r0, [r7, #2]
    mov r0, #0x15
    strh r0, [r7, #4]
    mov r0, #6
    strh r0, [r7, #6]
    mov r0, #0
    strh r0, [r7, #8]
    mvn r0, #3
    strh r0, [r7, #0x0A]
    b .L_success
.L_party_wide_0:
    mvn r0, #2
    strh r0, [r7]
    mov r0, #6
    strh r0, [r7, #2]
    mov r0, #0x20
    strh r0, [r7, #4]
    mov r0, #8
    strh r0, [r7, #6]
    mov r0, #0
    strh r0, [r7, #8]
    mvn r0, #3
    strh r0, [r7, #0x0A]
    b .L_success
.L_party_wide_1:
    mvn r0, #2
    strh r0, [r7]
    mov r0, #6
    strh r0, [r7, #2]
    mov r0, #0x20
    strh r0, [r7, #4]
    mov r0, #8
    strh r0, [r7, #6]
    mov r0, #0
    strh r0, [r7, #8]
    mvn r0, #3
    strh r0, [r7, #0x0A]
    b .L_success

.L_check_special_objects:
    sub r0, r6, #8
    cmp r0, #1
    bhi .L_resource_bounds
    cmp r5, #0
    addgt sp, sp, #4
    movgt r0, #0
    popgt {r4, r5, r6, r7, lr}
    bxgt lr
    mvn r0, #7
    strh r0, [r7]
    mov r0, #6
    strh r0, [r7, #2]
    mov r0, #0x0A
    strh r0, [r7, #4]
    mvn r0, #9
    strh r0, [r7, #6]
    mov r0, #4
    strh r0, [r7, #8]
    mvn r0, #3
    strh r0, [r7, #0x0A]
    b .L_success

.L_resource_bounds:
    cmp r6, #0x3C
    blo .L_get_scene_object
    mov r0, r6, lsl #16
    mov r0, r0, lsr #16
    bl BattleActor_GetById
.L_get_scene_object:
    mov r0, r6, lsl #16
    mov r0, r0, lsr #16
    bl BattleSceneObject_GetById
    bl BattleSceneObject_GetActiveModel
    movs r1, r0
    addeq sp, sp, #4
    moveq r0, #0
    popeq {r4, r5, r6, r7, lr}
    bxeq lr
    mov r0, r7
    mov r2, r5
    mov r3, r4
    bl BattleModel_GetAnimationBounds
    cmp r0, #0
    addeq sp, sp, #4
    moveq r0, #0
    popeq {r4, r5, r6, r7, lr}
    bxeq lr
    ldrsh r0, [r7, #2]
    ldrsh r2, [r7, #4]
    mov r1, #0
    rsb r0, r0, #0
    strh r2, [r7, #2]
    strh r0, [r7, #4]
    ldrsh r2, [r7, #6]
    mvn r0, #3
    rsb r2, r2, #0
    strh r2, [r7, #6]
    strh r1, [r7, #8]
    strh r0, [r7, #0x0A]

.L_success:
    mov r0, #1
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size BattleCollision_GetBounds, . - BattleCollision_GetBounds
