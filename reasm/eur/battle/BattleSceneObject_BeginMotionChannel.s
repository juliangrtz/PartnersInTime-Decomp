/* Attach an object to the motion list and initialize one callback channel. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_BeginMotionChannel, "ax", %progbits
.balign 4

.extern BattleSceneObject_GetMotionChannel
.extern BattleSceneObject_StopMotionChannel
.extern gBattleMotionObjectList

.global BattleSceneObject_BeginMotionChannel
.type BattleSceneObject_BeginMotionChannel, %function
BattleSceneObject_BeginMotionChannel:
    push {r4, r5, r6, r7, r8, lr}
    ldr r4, .L_motion_list
    mov r8, r0
    ldr r4, [r4]
    mov r7, r1
    mov r6, r2
    mov r5, r3
    mov r0, r4
.L_scan:
    cmp r0, #0
    ldreq r0, .L_motion_list
    streq r4, [r8]
    streq r8, [r0]
    beq .L_get_channel
    cmp r0, r8
    ldrne r0, [r0]
    bne .L_scan
.L_get_channel:
    mov r0, r8
    mov r1, r7
    bl BattleSceneObject_GetMotionChannel
    mov r4, r0
    ldr r0, [r4]
    cmp r0, #0
    beq .L_initialize
    mov r0, r8
    mov r1, r7
    mov r2, #0
    bl BattleSceneObject_StopMotionChannel
.L_initialize:
    ldrh r0, [r4, #0xa]
    cmp r0, #0
    movne r0, #0
    strhne r0, [r4, #0xc]
    strhne r0, [r4, #0xe]
    strhne r0, [r4, #0x10]
    strhne r0, [r4, #0xa]
    str r5, [r4]
    mov r0, #0
    str r0, [r4, #4]
    strh r6, [r4, #8]
    add r0, r4, #0x18
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.L_motion_list:
    .word gBattleMotionObjectList
.size BattleSceneObject_BeginMotionChannel, . - BattleSceneObject_BeginMotionChannel
