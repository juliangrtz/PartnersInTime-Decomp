/* Animate and retire the multi-stage party launch/impact reaction. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_UpdatePartyLaunchReaction, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_FLAGS, 0x24
.equ ACTOR_FLAG_HIT_LOCK, 0x200
.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ SCENE_OBJECT_X, 0x04
.equ SCENE_OBJECT_Y, 0x06
.equ SCENE_OBJECT_Z, 0x08
.equ SCENE_OBJECT_OFFSET_Y, 0x14
.equ SCENE_OBJECT_ACTOR_ID, 0xEC
.equ SCENE_OBJECT_DEPTH, 0xEA
.equ SCENE_OBJECT_FLAGS, 0xF4
.equ TASK_CALLBACK, 0x04
.equ TASK_DATA, 0x0C
.equ TASK_DATA_COUNT, 0x00
.equ TASK_DATA_ACTOR_ID, 0x02
.equ TASK_DATA_PHASE, 0x08

.extern BattleActor_GetById
.extern BattleActor_GetPartySlot
.extern BattleEntity_BindResource
.extern BattleSceneObject_GetById
.extern data_ov002_020bfa3c
.extern func_ov002_02076b0c
.extern func_ov002_02091198
.extern func_ov002_02091a58
.extern func_ov002_020a3370
.extern func_ov002_020a3f9c
.extern func_ov002_020a4934
.extern func_ov002_020a9280
.extern func_ov002_020a9c18

.global BattleDamage_UpdatePartyLaunchReaction
.type BattleDamage_UpdatePartyLaunchReaction, %function
BattleDamage_UpdatePartyLaunchReaction:
    push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    sub sp, sp, #0x2C
    mov r4, r0
    add sl, r4, #TASK_DATA
    ldrh r0, [sl, #TASK_DATA_ACTOR_ID]
    bl BattleSceneObject_GetById
    mov sb, r0
    ldr r1, [sb, #SCENE_OBJECT_FLAGS]
    ldrsh r0, [sb, #SCENE_OBJECT_Y]
    mov r1, r1, lsl #14
    mov r1, r1, lsr #31
    str r1, [sp]
    ldr r1, [sb, #SCENE_OBJECT_FLAGS]
    rsb r2, r0, #0x100
    mov r1, r1, lsl #17
    mov r1, r1, lsr #31
    str r1, [sp, #4]
    ldrsh r5, [sb, #SCENE_OBJECT_Z]
    ldrsh r3, [sb, #SCENE_OBJECT_DEPTH]
    ldrsh r1, [sb, #SCENE_OBJECT_X]
    sub r5, r0, r5
    add r0, r3, r2, lsl #4
    mov r3, r0, lsl #16
    mov r2, r5, lsl #16
    add r0, sp, #0x20
    mov r2, r2, asr #16
    mov r3, r3, asr #16
    bl func_ov002_02076b0c

    ldrsh r0, [sl, #TASK_DATA_PHASE]
    cmp r0, #1
    bne .L_check_return_phase
    ldrsh r0, [sp, #0x22]
    cmp r0, #0
    bge .L_wait_for_action
    ldrh r0, [sl, #TASK_DATA_ACTOR_ID]
    bl BattleSceneObject_GetById
    bl func_ov002_020a3370
    ldrsh r1, [r0, #0x54]
    mov r0, sb
    mov r2, #1
    bl func_ov002_02091198
    ldrsh r1, [sb, #SCENE_OBJECT_OFFSET_Y]
    mov r0, #2
    sub r1, r1, #0xE0
    strh r1, [sb, #SCENE_OBJECT_OFFSET_Y]
    strh r0, [sl, #TASK_DATA_PHASE]
    b .L_wait_for_action

.L_check_return_phase:
    cmp r0, #2
    bne .L_wait_for_action
    ldrsh r0, [sp, #0x22]
    cmp r0, #0xE0
    blt .L_wait_for_action
    ldrh r0, [sl, #TASK_DATA_ACTOR_ID]
    bl BattleSceneObject_GetById
    bl func_ov002_020a3370
    ldrsh r1, [r0, #0x54]
    mov r0, sb
    mov r2, #0
    bl func_ov002_02091198
    ldrsh r1, [sb, #SCENE_OBJECT_OFFSET_Y]
    mov r0, #1
    add r1, r1, #0xE0
    strh r1, [sb, #SCENE_OBJECT_OFFSET_Y]
    strh r0, [sl, #TASK_DATA_PHASE]

.L_wait_for_action:
    ldrsh r0, [sl, #TASK_DATA_COUNT]
    cmp r0, #3
    beq .L_count_ready
    ldrh r0, [sl, #TASK_DATA_ACTOR_ID]
    mov r1, #2
    bl func_ov002_02091a58
    cmp r0, #0
    addne sp, sp, #0x2C
    popne {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxne lr
    ldrh r0, [sl, #TASK_DATA_ACTOR_ID]
    bl BattleActor_GetPartySlot
    bl func_ov002_020a9c18

.L_count_ready:
    ldrsh r0, [sl, #TASK_DATA_COUNT]
    cmp r0, #0
    bne .L_emit_effects
    mov r1, #0
    str r1, [r4, #TASK_CALLBACK]
    ldrh r0, [sb, #SCENE_OBJECT_ACTOR_ID]
    cmp r0, #0x38
    movne r1, #1
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    bl BattleEntity_BindResource
    ldrh r0, [sb, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_GetById
    ldrsh r0, [r0, #BATTLE_ACTOR_CURRENT_HP]
    cmp r0, #0
    bgt .L_restore_actor
    mov r0, sb
    bl func_ov002_020a9280
    add sp, sp, #0x2C
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr
.L_restore_actor:
    mov r0, sb
    mov r1, #0
    mvn r2, #0
    bl func_ov002_02091198
    ldrh r0, [sl, #TASK_DATA_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    add sp, sp, #0x2C
    bic r1, r1, #ACTOR_FLAG_HIT_LOCK
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr

.L_emit_effects:
    cmp r0, #3
    bne .L_emit_count_effect
    ldrh r0, [sb, #SCENE_OBJECT_ACTOR_ID]
    cmp r0, #0x38
    bne .L_other_party_position
    ldrsh r0, [sb, #SCENE_OBJECT_X]
    ldrsh r1, [sb, #SCENE_OBJECT_Y]
    rsb r0, r0, #0x38
    str r0, [sp, #0x14]
    rsb r0, r1, #0x70
    str r0, [sp, #0x18]
    b .L_emit_burst
.L_other_party_position:
    ldrsh r0, [sb, #SCENE_OBJECT_X]
    ldrsh r1, [sb, #SCENE_OBJECT_Y]
    rsb r0, r0, #0x2E
    str r0, [sp, #0x14]
    rsb r0, r1, #0x98
    str r0, [sp, #0x18]
.L_emit_burst:
    ldrsh r0, [sb, #SCENE_OBJECT_Z]
    mov r8, #0
    mov r7, #2
    rsb r0, r0, #0
    mov fp, r7
    str r8, [sp, #0x1C]
    mov r4, r8
    mov r6, #1
    mvn r5, #0x7F
.L_burst_loop:
    str r6, [sp]
    str r0, [sp, #4]
    ldr r0, .L_effect_table
    str r5, [sp, #8]
    mov r1, r7, lsl #1
    ldrh r2, [r0, r1]
    mov r0, sb
    mov r1, fp
    str r2, [sp, #0x0C]
    ldr r2, [sp, #0x1C]
    str r6, [sp, #0x10]
    mov r3, r2
    bl func_ov002_020a3f9c
    add r8, r8, r0
    mov r0, r4
    subs r7, r7, #1
    bpl .L_burst_loop
    ldr r2, [sp, #0x14]
    str r4, [sp]
    ldr r3, [sp, #0x18]
    mov r0, sb
    mov r1, #3
    str r8, [sp, #4]
    bl func_ov002_020a4934

.L_emit_count_effect:
    ldrsh r0, [sl, #TASK_DATA_COUNT]
    mov r2, #0
    mov r4, #1
    sub r0, r0, #1
    strh r0, [sl, #TASK_DATA_COUNT]
    str r4, [sp]
    ldrsh r3, [sb, #SCENE_OBJECT_Z]
    mvn r0, #0x7F
    ldr r1, .L_effect_table
    rsb r3, r3, #0
    str r3, [sp, #4]
    str r0, [sp, #8]
    ldrsh r5, [sl, #TASK_DATA_COUNT]
    mov r0, sb
    mov r3, r2
    mov r5, r5, lsl #1
    ldrh r5, [r1, r5]
    mov r1, #2
    str r5, [sp, #0x0C]
    str r4, [sp, #0x10]
    bl func_ov002_020a3f9c
    add sp, sp, #0x2C
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr
.L_effect_table:
    .word data_ov002_020bfa3c
.size BattleDamage_UpdatePartyLaunchReaction, . - BattleDamage_UpdatePartyLaunchReaction
