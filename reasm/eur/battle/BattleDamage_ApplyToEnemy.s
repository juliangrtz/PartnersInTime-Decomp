/* Complete enemy hit path: HP loss, reaction, number popup, and hit effects. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_ApplyToEnemy, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_ACTOR_RESOURCE_SLOT, 0x6C
.equ BATTLE_ACTOR_HIT_OFFSET_Y, 0x29A
.equ BATTLE_ACTOR_STATE_FLAGS, 0x29C
.equ BATTLE_ACTOR_IMPACT_X, 0x29D
.equ BATTLE_ACTOR_IMPACT_Y, 0x29E
.equ SCENE_OBJECT_X, 0x04
.equ SCENE_OBJECT_Y, 0x06
.equ SCENE_OBJECT_Z, 0x08
.equ SCENE_OBJECT_RESOURCE, 0xBC
.equ SCENE_OBJECT_SAVED_VALUE, 0xE6
.equ SCENE_OBJECT_DEPTH, 0xEA
.equ SCENE_OBJECT_ACTOR_ID, 0xEC
.equ SCENE_OBJECT_FLAGS, 0xF4
.equ SCENE_OBJECT_IMPACT_X, 0x102
.equ SCENE_OBJECT_IMPACT_Y, 0x103
.equ DAMAGE_MAX, 999
.equ SPECIAL_RESOURCE_1, 0xC1000082
.equ SPECIAL_RESOURCE_2, 0xC10000A4
.equ ARG_EFFECT_KIND, 0x30
.equ ARG_POPUP_STYLE, 0x34
.equ ARG_ATTACH_NUMBER, 0x38
.equ LOCAL_POSITION, 0x08

.extern BattleActor_ApplyDamage
.extern BattleActor_GetById
.extern BattleActor_GetEnemySlot
.extern func_ov002_02076b0c
.extern func_ov002_02091198
.extern func_ov002_0209cd9c
.extern func_ov002_0209d5cc
.extern func_ov002_020acb88
.extern func_ov002_020accb8

.global BattleDamage_ApplyToEnemy
.type BattleDamage_ApplyToEnemy, %function
BattleDamage_ApplyToEnemy:
    push {r4, r5, r6, r7, r8, sb, lr}
    sub sp, sp, #0x14
    mov sb, r0
    ldrh r0, [sb, #SCENE_OBJECT_ACTOR_ID]
    mov r8, r1
    mov r7, r2
    mov r6, r3
    bl BattleActor_GetEnemySlot
    mov r5, r0
    ldrsh r0, [r5, #BATTLE_ACTOR_CURRENT_HP]
    cmp r0, #0
    addle sp, sp, #0x14
    pople {r4, r5, r6, r7, r8, sb, lr}
    bxle lr
    ldr r0, .L_damage_max
    cmp r6, #1
    movlt r6, #1
    cmp r6, r0
    movgt r6, r0

    ldr r0, [r5, #BATTLE_ACTOR_STATE_FLAGS]
    mov r0, r0, lsl #27
    movs r0, r0, lsr #31
    bne .L_damage_done
    mov r0, sb
    mov r1, r6
    bl BattleActor_ApplyDamage
.L_damage_done:
    ldr r0, [r5, #BATTLE_ACTOR_STATE_FLAGS]
    mov r0, r0, lsl #27
    movs r0, r0, lsr #31
    bne .L_reaction_done

    ldr r1, [sb, #SCENE_OBJECT_RESOURCE]
    ldr r0, .L_special_resource_1
    ldr r1, [r1, #0x24]
    ldrsh r4, [sb, #SCENE_OBJECT_SAVED_VALUE]
    cmp r1, r0
    beq .L_start_reaction
    ldr r0, .L_special_resource_2
    cmp r1, r0
    beq .L_start_reaction
    add r0, r5, #0x200
    add r1, sb, #0x100
    ldrsb ip, [r0, #0x9D]
    ldrsb r3, [r1, #0x02]
    mov r2, #0
    add r3, ip, r3
    strb r3, [r5, #BATTLE_ACTOR_IMPACT_X]
    ldrsb r3, [r0, #0x9E]
    ldrsb r0, [r1, #0x03]
    add r0, r3, r0
    strb r0, [r5, #BATTLE_ACTOR_IMPACT_Y]
    strb r2, [sb, #SCENE_OBJECT_IMPACT_X]
    strb r2, [sb, #SCENE_OBJECT_IMPACT_Y]
.L_start_reaction:
    add r0, r5, #0x200
    ldrsh r1, [r0, #0x9A]
    mov r0, sb
    mvn r2, #0
    bl func_ov002_02091198
    ldrh r0, [sb, #SCENE_OBJECT_ACTOR_ID]
    bl BattleActor_GetById
    ldrh r0, [r0, #BATTLE_ACTOR_FLAGS]
    mov r0, r0, lsl #22
    movs r0, r0, lsr #31
    strhne r4, [sb, #SCENE_OBJECT_SAVED_VALUE]
.L_reaction_done:
    ldr r1, [sp, #ARG_EFFECT_KIND]
    mov r0, sb
    bl func_ov002_0209d5cc
    ldr r0, [r5, #BATTLE_ACTOR_STATE_FLAGS]
    mov r0, r0, lsl #27
    movs r0, r0, lsr #31
    addne sp, sp, #0x14
    popne {r4, r5, r6, r7, r8, sb, lr}
    bxne lr

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
    ldrsh r4, [sb, #SCENE_OBJECT_Z]
    ldrsh r3, [sb, #SCENE_OBJECT_DEPTH]
    ldrsh r1, [sb, #SCENE_OBJECT_X]
    sub r4, r0, r4
    add r0, r3, r2, lsl #4
    mov r3, r0, lsl #16
    mov r2, r4, lsl #16
    add r0, sp, #LOCAL_POSITION
    mov r2, r2, asr #16
    mov r3, r3, asr #16
    bl func_ov002_02076b0c

    ldr r0, [r5, #BATTLE_ACTOR_RESOURCE_SLOT]
    cmp r0, #0
    beq .L_spawn_number
    ldr r0, [r0]
    ldrh r0, [r0, #4]
    mov r0, r0, lsl #30
    movs r0, r0, lsr #31
    bne .L_number_done
.L_spawn_number:
    ldrh r0, [sp, #ARG_ATTACH_NUMBER]
    ldr r3, [sp, #ARG_POPUP_STYLE]
    cmp r0, #0
    ldrhne r0, [sb, #SCENE_OBJECT_ACTOR_ID]
    moveq r0, #0
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    str r0, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r2, [sp, #LOCAL_POSITION + 2]
    mov r0, r6
    add r1, r1, r8
    add r2, r2, r7
    bl func_ov002_0209cd9c
.L_number_done:
    ldr r0, [sp, #ARG_EFFECT_KIND]
    cmp r0, #0x11
    bne .L_check_second_effect
    mov r3, r7, lsl #16
    mov r2, #0
    mov r0, r8, lsl #16
    str r2, [sp]
    mov r4, #0x100
    mov r2, r0, asr #16
    mov r1, sb
    mov r3, r3, asr #16
    mov r0, #0x10
    str r4, [sp, #4]
    bl func_ov002_020acb88
    mov r0, r4
    str r0, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r0, [sp, #LOCAL_POSITION + 2]
    ldrsh r3, [sp, #LOCAL_POSITION + 4]
    add r2, r1, r8
    add r1, r0, r7
    mov r0, r2, lsl #16
    mov r2, r1, lsl #16
    mov r1, r0, asr #16
    mov r2, r2, asr #16
    mov r0, #0x0A
    bl func_ov002_020accb8
.L_check_second_effect:
    ldr r0, [sp, #ARG_EFFECT_KIND]
    cmp r0, #0x13
    addne sp, sp, #0x14
    popne {r4, r5, r6, r7, r8, sb, lr}
    bxne lr
    mov r3, r7, lsl #16
    mov r2, #0
    mov r0, r8, lsl #16
    str r2, [sp]
    mov r4, #0x100
    mov r2, r0, asr #16
    mov r1, sb
    mov r3, r3, asr #16
    mov r0, #0x12
    str r4, [sp, #4]
    bl func_ov002_020acb88
    mov r0, r4
    str r0, [sp]
    ldrsh r1, [sp, #LOCAL_POSITION]
    ldrsh r0, [sp, #LOCAL_POSITION + 2]
    ldrsh r3, [sp, #LOCAL_POSITION + 4]
    add r2, r1, r8
    add r1, r0, r7
    mov r0, r2, lsl #16
    mov r2, r1, lsl #16
    mov r1, r0, asr #16
    mov r2, r2, asr #16
    mov r0, #0x0B
    bl func_ov002_020accb8
    add sp, sp, #0x14
    pop {r4, r5, r6, r7, r8, sb, lr}
    bx lr

.L_damage_max:
    .word DAMAGE_MAX
.L_special_resource_1:
    .word SPECIAL_RESOURCE_1
.L_special_resource_2:
    .word SPECIAL_RESOURCE_2
.size BattleDamage_ApplyToEnemy, . - BattleDamage_ApplyToEnemy
