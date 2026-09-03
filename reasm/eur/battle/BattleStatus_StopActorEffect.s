/* Stop the actor-local timer or stat delta associated with one status ID. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleStatus_StopActorEffect, "ax", %progbits
.balign 4

.extern BattleActor_GetById

.global BattleStatus_StopActorEffect
.type BattleStatus_StopActorEffect, %function
BattleStatus_StopActorEffect:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, r1
    bl BattleActor_GetById
    cmp r5, #0x3c
    blo .L_other_actor
    cmp r5, #0x44
    bhs .L_other_actor
    cmp r4, #8
    addls pc, pc, r4, lsl #2
    b .L_return
    b .L_return
    b .L_enemy_status_1
    b .L_enemy_status_2
    b .L_enemy_status_3
    b .L_enemy_status_4
    b .L_enemy_status_5
    b .L_enemy_status_6
    b .L_enemy_status_7
    b .L_enemy_status_8
.L_enemy_status_1:
    add r1, r0, #0x30
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #1
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_enemy_status_2:
    add r1, r0, #0x30
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #2
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_enemy_status_3:
    add r1, r0, #0x30
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #3
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_enemy_status_4:
    add r1, r0, #0x3c
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #1
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_enemy_status_5:
    add r1, r0, #0x3c
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #2
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_enemy_status_6:
    mov r1, #0
    str r1, [r0, #0x48]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_enemy_status_7:
    mov r1, #0
    str r1, [r0, #0x54]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_enemy_status_8:
    mov r1, #0
    str r1, [r0, #0x60]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_other_actor:
    cmp r4, #8
    addls pc, pc, r4, lsl #2
    b .L_return
    b .L_return
    b .L_other_status_1
    b .L_other_status_2
    b .L_other_status_3
    b .L_other_status_4
    b .L_other_status_5
    b .L_other_status_6
    b .L_other_status_7
    b .L_other_status_8
.L_other_status_1:
    add r1, r0, #0x30
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #1
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_other_status_2:
    add r1, r0, #0x30
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #2
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_other_status_3:
    add r1, r0, #0x30
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #3
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_other_status_4:
    add r1, r0, #0x3c
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #1
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_other_status_5:
    add r1, r0, #0x3c
    ldrsb r0, [r1, #8]
    add sp, sp, #4
    cmp r0, #2
    moveq r0, #0
    streq r0, [r1]
    pop {r4, r5, lr}
    bx lr
.L_other_status_6:
    mov r1, #0
    str r1, [r0, #0x48]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_other_status_7:
    mov r1, #0
    str r1, [r0, #0x54]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_other_status_8:
    mov r1, #0
    str r1, [r0, #0x60]
.L_return:
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size BattleStatus_StopActorEffect, . - BattleStatus_StopActorEffect
