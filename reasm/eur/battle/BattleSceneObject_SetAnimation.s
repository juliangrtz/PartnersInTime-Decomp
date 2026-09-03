/* Select, create, stop, and start a battle scene-object animation. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_SetAnimation, "ax", %progbits
.balign 4

.extern BattleActor_GetById
.extern BattleActor_GetEnemySlot
.extern BattleActor_GetPartySlot
.extern BattleActor_IsHpAtMostQuarter
.extern BattleSceneObject_GetActiveModel
.extern OS_Terminate
.extern data_ov002_020bf718
.extern func_02010634
.extern func_02029be0
.extern func_ov002_020696f4
.extern func_ov002_0206f1b8
.extern func_ov002_0206f1f0
.extern func_ov002_0206f384
.extern gBattleContext

.global BattleSceneObject_SetAnimation
.type BattleSceneObject_SetAnimation, %function
BattleSceneObject_SetAnimation:
    push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    sub sp, sp, #4
    mov sb, r0
    ldr r0, [sb, #0xBC]
    mov r8, r1
    mov r7, r2
    cmp r0, #0
    bne .L_resource_present
    bl OS_Terminate
.L_resource_present:
    ldr r0, [sb, #0xBC]
    ldr r0, [r0, #0x2C]
    mov r0, r0, lsl #1
    movs r0, r0, lsr #31
    beq .L_resource_valid
    bl OS_Terminate
.L_resource_valid:
    ldrh r5, [sb, #0xEC]
    ldr r6, [sb, #0xC0]
    mov r4, #0
    cmp r5, #0x38
    blo .L_check_enemy
    cmp r5, #0x3C
    movlo r4, #1
    blo .L_classified
.L_check_enemy:
    cmp r5, #0x3C
    blo .L_classified
    cmp r5, #0x44
    movlo r4, #2
.L_classified:
    ldr r0, [sb, #0xF4]
    mov r0, r0, lsl #5
    movs r0, r0, lsr #29
    beq .L_normal_path

    cmp r6, #0
    bne .L_toggle_special_model
    ldr r0, .L_battle_context
    ldr r2, .L_model_alloc_config
    ldr r1, [r0]
    mov r0, #0x1B8
    add r1, r1, #0xE000
    ldr r1, [r1, #0x158]
    mov r3, #0
    bl func_02029be0
    movs r6, r0
    beq .L_store_special_model
    bl func_02010634
    mov r6, r0
.L_store_special_model:
    str r6, [sb, #0xC0]
    mov r1, #0
    mov r0, r6
    strb r1, [r6, #0x10]
    ldr r1, [r0]
    ldr r1, [r1, #0x24]
    blx r1
    ldrb r2, [r6, #0x144]
    mov r1, #0
    mov r0, #0x100
    bic r2, r2, #4
    strb r2, [r6, #0x144]
    ldrb r2, [r6, #0x15E]
    bic r2, r2, #0x20
    strb r2, [r6, #0x15E]
    ldr r2, [r6, #0x7C]
    orr r2, r2, #0x100
    str r2, [r6, #0x7C]
    strh r1, [r6, #0x5C]
    strh r1, [r6, #0x5E]
    strh r0, [r6, #0x74]
    strh r0, [r6, #0x76]
    str sb, [r6, #0x0C]
.L_toggle_special_model:
    cmp r8, #0
    bge .L_enable_special_model
    mov r0, r6
    ldr r1, [r0]
    ldr r1, [r1, #0x28]
    blx r1
    ldr r0, [r6, #0x7C]
    add sp, sp, #4
    bic r0, r0, #0x100
    str r0, [r6, #0x7C]
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr
.L_enable_special_model:
    mov r0, r6
    ldr r1, [r0]
    ldr r1, [r1, #0x24]
    blx r1
    ldr r0, [r6, #0x7C]
    add sp, sp, #4
    orr r0, r0, #0x100
    str r0, [r6, #0x7C]
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr

.L_normal_path:
    mov r0, #0
    strb r0, [sb, #0xF4]
    cmp r7, #0
    beq .L_adjust_animation_for_actor
    cmp r7, #1
    beq .L_after_party_adjustment
    ldr r0, [sb, #0xF4]
    mov r0, r0, lsl #17
    movs r0, r0, lsr #31
    bne .L_after_party_adjustment
.L_adjust_animation_for_actor:
    cmp r4, #2
    bne .L_check_party_adjustment
    mov r0, r5
    bl BattleActor_GetEnemySlot
    cmp r8, #0
    bne .L_after_party_adjustment
    ldrsb r1, [r0, #0x38]
    cmp r1, #2
    addeq r0, r0, #0x200
    ldrsheq r8, [r0, #0x9A]
    b .L_after_party_adjustment
.L_check_party_adjustment:
    cmp r4, #1
    bne .L_after_party_adjustment
    ldrsh r0, [sb, #0xE8]
    cmp r0, #0
    bne .L_after_party_adjustment
    mov r0, r5
    bl BattleActor_GetPartySlot
    mov sl, r0
    cmp r8, #0
    bne .L_check_party_status_animation
    ldr r0, [sb, #0xBC]
    ldr r1, [sb, #0xE0]
    ldr r0, [r0, #0x24]
    cmp r0, r1
    bne .L_select_party_idle
    ldr r1, .L_resource_55
    cmp r0, r1
    bhi .L_idle_range_high
    cmp r0, r1
    bhs .L_maybe_keep_animation
    ldr r1, .L_resource_53
    cmp r0, r1
    bhi .L_idle_range_middle
    cmp r0, r1
    bhs .L_maybe_keep_animation
    sub r1, r0, #0xC0000000
    cmp r1, #0x0D
    addls pc, pc, r1, lsl #2
    b .L_select_party_idle
    b .L_maybe_keep_animation
    b .L_maybe_keep_animation
    b .L_maybe_keep_animation
    b .L_maybe_keep_animation
    b .L_select_party_idle
    b .L_select_party_idle
    b .L_select_party_idle
    b .L_select_party_idle
    b .L_select_party_idle
    b .L_select_party_idle
    b .L_select_party_idle
    b .L_maybe_keep_animation
    b .L_select_party_idle
    b .L_maybe_keep_animation
.L_idle_range_middle:
    ldr r1, .L_resource_54
    cmp r0, r1
    beq .L_maybe_keep_animation
    b .L_select_party_idle
.L_idle_range_high:
    ldr r1, .L_resource_57
    cmp r0, r1
    bhi .L_idle_top
    cmp r0, r1
    bhs .L_maybe_keep_animation
    ldr r1, .L_resource_56
    cmp r0, r1
    beq .L_maybe_keep_animation
    b .L_select_party_idle
.L_idle_top:
    ldr r1, .L_resource_58
    cmp r0, r1
    bne .L_select_party_idle
.L_maybe_keep_animation:
    cmp r6, #0
    beq .L_select_party_idle
    ldr r1, [r6, #0x7C]
    mov r1, r1, lsl #23
    movs r1, r1, lsr #31
    beq .L_select_party_idle
    ldrsh r1, [r6, #0x54]
    cmp r1, r8
    addeq sp, sp, #4
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr

.L_select_party_idle:
    ldr r1, .L_resource_55
    cmp r0, r1
    bhi .L_select_idle_range_high
    cmp r0, r1
    bhs .L_apply_party_idle
    ldr r1, .L_resource_53
    cmp r0, r1
    bhi .L_select_idle_range_middle
    cmp r0, r1
    bhs .L_apply_party_idle
    sub r1, r0, #0xC0000000
    cmp r1, #0x0D
    addls pc, pc, r1, lsl #2
    b .L_check_party_status_animation
    b .L_apply_party_idle
    b .L_apply_party_idle
    b .L_apply_party_idle
    b .L_apply_party_idle
    b .L_check_party_status_animation
    b .L_check_party_status_animation
    b .L_check_party_status_animation
    b .L_check_party_status_animation
    b .L_check_party_status_animation
    b .L_check_party_status_animation
    b .L_check_party_status_animation
    b .L_apply_party_idle
    b .L_check_party_status_animation
    b .L_apply_party_idle
.L_select_idle_range_middle:
    ldr r1, .L_resource_54
    cmp r0, r1
    beq .L_apply_party_idle
    b .L_check_party_status_animation
.L_select_idle_range_high:
    ldr r1, .L_resource_57
    cmp r0, r1
    bhi .L_select_idle_top
    cmp r0, r1
    bhs .L_apply_party_idle
    ldr r1, .L_resource_56
    cmp r0, r1
    beq .L_apply_party_idle
    b .L_check_party_status_animation
.L_select_idle_top:
    ldr r1, .L_resource_58
    cmp r0, r1
    bne .L_check_party_status_animation
.L_apply_party_idle:
    ldrsb r0, [sl, #0x38]
    cmp r0, #1
    moveq r8, #0x19
    beq .L_check_party_status_animation
    cmp r0, #2
    moveq r8, #0x1B
    beq .L_check_party_status_animation
    cmp r0, #3
    moveq r8, #0x1D
    beq .L_check_party_status_animation
    ldrsb r0, [sl, #0x44]
    cmp r0, #0
    movne r8, #0x16
    bne .L_check_party_status_animation
    mov r0, r5
    bl BattleActor_GetById
    bl BattleActor_IsHpAtMostQuarter
    cmp r0, #0
    movne r8, #0x16

.L_check_party_status_animation:
    cmp r8, #0
    blt .L_after_party_adjustment
    ldrsb r0, [sl, #0x38]
    cmp r0, #3
    bne .L_after_party_adjustment
    ldr r2, [sl, #0x30]
    cmp r2, #0
    beq .L_after_party_adjustment
    ldr r0, [sb, #0xBC]
    ldr r1, .L_resource_55
    ldr r0, [r0, #0x24]
    ldr fp, [r2, #0x0C]
    cmp r0, r1
    bhi .L_status_range_high
    cmp r0, r1
    bhs .L_remap_linked_party_animation
    ldr r1, .L_resource_53
    cmp r0, r1
    bhi .L_status_range_middle
    cmp r0, r1
    bhs .L_remap_linked_party_animation
    sub r1, r0, #0xC0000000
    cmp r1, #0x0D
    addls pc, pc, r1, lsl #2
    b .L_after_party_adjustment
    b .L_remap_linked_party_animation
    b .L_remap_linked_party_animation
    b .L_remap_linked_party_animation
    b .L_remap_linked_party_animation
    b .L_after_party_adjustment
    b .L_after_party_adjustment
    b .L_after_party_adjustment
    b .L_after_party_adjustment
    b .L_after_party_adjustment
    b .L_after_party_adjustment
    b .L_after_party_adjustment
    b .L_remap_linked_party_animation
    b .L_after_party_adjustment
    b .L_remap_linked_party_animation
.L_status_range_middle:
    ldr r1, .L_resource_54
    cmp r0, r1
    beq .L_remap_linked_party_animation
    b .L_after_party_adjustment
.L_status_range_high:
    ldr r1, .L_resource_57
    cmp r0, r1
    bhi .L_status_top
    cmp r0, r1
    bhs .L_remap_linked_party_animation
    ldr r1, .L_resource_56
    cmp r0, r1
    beq .L_remap_linked_party_animation
    b .L_after_party_adjustment
.L_status_top:
    ldr r1, .L_resource_58
    cmp r0, r1
    bne .L_after_party_adjustment

.L_remap_linked_party_animation:
    ldrh r0, [sl, #0x7E]
    mov r1, r8
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_start_linked_party_animation
    b .L_start_linked_party_animation
    b .L_linked_offset_1
    b .L_linked_offset_2
    b .L_linked_offset_3
    b .L_linked_offset_4
    b .L_linked_offset_5
.L_linked_offset_1:
    add r1, r8, #0x2C
    b .L_start_linked_party_animation
.L_linked_offset_2:
    add r1, r8, #0x58
    b .L_start_linked_party_animation
.L_linked_offset_3:
    add r1, r8, #0x84
    b .L_start_linked_party_animation
.L_linked_offset_4:
    add r1, r8, #0xB0
    b .L_start_linked_party_animation
.L_linked_offset_5:
    add r1, r8, #0xDC
.L_start_linked_party_animation:
    mov r0, fp
    ldr r3, [r0]
    and r1, r1, #0xFF
    ldr r3, [r3, #0x34]
    mov r2, #0
    blx r3
    ldr r0, [fp, #0x7C]
    bic r0, r0, #0xF000
    str r0, [fp, #0x7C]

.L_after_party_adjustment:
    cmp r8, #0
    blt .L_animation_id_ready
    ldrsh r1, [sb, #0xE8]
    ldrsh r0, [sb, #0xE4]
    add r8, r8, r1
    sub r0, r0, r1
    strh r0, [sb, #0xE6]
    strh r8, [sb, #0xE4]
.L_animation_id_ready:
    cmp r8, #0
    mov sl, #0
    blt .L_select_model_channel
    ldr r0, .L_battle_context
    ldr r0, [r0]
    add r0, r0, #0x6000
    ldr r0, [r0, #0x8C8]
    cmp r0, #0
    beq .L_select_model_channel
    ldr r2, [sb, #0xBC]
    ldr r1, [r2, #0x24]
    and r0, r1, #0xFF000000
    cmp r0, #0xC0000000
    bne .L_select_model_channel
    ldr r0, [r2, #0x18]
    ldrh r0, [r0, #2]
    cmp r0, #0
    bne .L_select_model_channel
    ldr r0, [sb, #0xE0]
    cmp r0, r1
    movne sl, #1
    bne .L_select_model_channel
    cmp r7, #0
    beq .L_check_primary_model
    cmp r7, #1
    beq .L_check_alternate_model
    ldr r0, [sb, #0xF4]
    mov r0, r0, lsl #17
    movs r0, r0, lsr #31
    bne .L_check_alternate_model
.L_check_primary_model:
    ldr r0, [sb, #0xC0]
    cmp r0, #0
    beq .L_mark_resource_changed
    ldr r0, [r0, #0x7C]
    mov r0, r0, lsl #23
    movs r0, r0, lsr #31
    bne .L_select_model_channel
.L_mark_resource_changed:
    mov sl, #1
    b .L_select_model_channel
.L_check_alternate_model:
    ldr r0, [sb, #0xC4]
    cmp r0, #0
    beq .L_mark_alternate_changed
    ldr r0, [r0, #0x7C]
    mov r0, r0, lsl #23
    movs r0, r0, lsr #31
    bne .L_select_model_channel
.L_mark_alternate_changed:
    mov sl, #1

.L_select_model_channel:
    cmp r7, #0
    bne .L_update_secondary_channel
    ldr r0, [sb, #0xF4]
    mov r1, r0, lsl #17
    movs r1, r1, lsr #31
    beq .L_channel_selected
    bic r0, r0, #0x4000
    str r0, [sb, #0xF4]
    ldr r0, [sb, #0xC4]
    cmp r0, #0
    beq .L_channel_selected
    bl func_ov002_0206f384
    b .L_channel_selected
.L_update_secondary_channel:
    mov r0, sb
    mov r1, r8
    mov r2, r7
    bl func_ov002_020696f4
.L_channel_selected:
    ldr r0, [sb, #0xF4]
    mov r0, r0, lsl #17
    movs r0, r0, lsr #31
    bne .L_notify_resource_change
    cmp r8, #0
    bge .L_start_animation
    ldr r0, [sb, #0xBC]
    cmp r6, #0
    ldr r0, [r0, #0x24]
    addeq sp, sp, #4
    str r0, [sb, #0xE0]
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr
    ldr r0, [r6, #0x7C]
    mov r0, r0, lsl #23
    movs r0, r0, lsr #31
    addeq sp, sp, #4
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr
    mov r0, r6
    bl func_ov002_0206f1b8
    ldr r0, [r6, #0x7C]
    add sp, sp, #4
    bic r0, r0, #0x100
    str r0, [r6, #0x7C]
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr

.L_start_animation:
    cmp r6, #0
    mov r7, #0
    beq .L_check_resource_key
    add r0, r6, #0x100
    ldrh r0, [r0, #0x62]
    mov r7, #1
    mov r0, r0, lsl #27
    mov r0, r0, lsr #27
    str r0, [sp]
.L_check_resource_key:
    ldr r1, [sb, #0xBC]
    ldr r0, [sb, #0xE0]
    ldr r1, [r1, #0x24]
    cmp r0, r1
    beq .L_ensure_model
    str r1, [sb, #0xE0]
    cmp r6, #0
    beq .L_ensure_model
    mov r0, r6
    bl func_ov002_0206f1b8
    ldr r0, [r6, #0x7C]
    bic r0, r0, #0x100
    str r0, [r6, #0x7C]
.L_ensure_model:
    cmp r6, #0
    bne .L_initialize_model
    ldr r0, .L_battle_context
    ldr r2, .L_model_alloc_config
    ldr r1, [r0]
    mov r0, #0x1B8
    add r1, r1, #0xE000
    ldr r1, [r1, #0x158]
    mov r3, #0
    bl func_02029be0
    movs r6, r0
    beq .L_store_model
    bl func_02010634
    mov r6, r0
.L_store_model:
    str r6, [sb, #0xC0]
    ldr r0, [r6, #0x7C]
    bic r0, r0, #0x100
    str r0, [r6, #0x7C]
    str sb, [r6, #0x0C]
.L_initialize_model:
    ldr r0, [r6, #0x7C]
    bic r0, r0, #0x200
    str r0, [r6, #0x7C]
    ldr r0, [r6, #0x7C]
    bic r0, r0, #4
    str r0, [r6, #0x7C]
    ldr r0, [r6, #0x7C]
    mov r0, r0, lsl #23
    movs r0, r0, lsr #31
    bne .L_set_animation
    ldr r1, [sb, #0xBC]
    mov r0, r6
    ldr r2, [r1, #0x2C]
    mov r2, r2, lsl #5
    mov r2, r2, lsr #5
    sub r2, r2, #1
    mov r2, r2, lsl #6
    bl func_ov002_0206f1f0
    mov r0, #0
    strh r0, [r6, #0x5C]
    strh r0, [r6, #0x5E]
.L_set_animation:
    mov r0, r6
    ldr fp, [r0]
    and r1, r8, #0xFF
    ldr fp, [fp, #0x68]
    mov r2, #0
    mov r3, #1
    blx fp
    cmp r7, #0
    beq .L_post_animation
    add r1, r6, #0x100
    ldr r0, [sp]
    ldrh r2, [r1, #0x62]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    bic r2, r2, #0x1F
    and r0, r0, #0x1F
    orr r0, r2, r0
    strh r0, [r1, #0x62]
.L_post_animation:
    cmp r4, #1
    beq .L_notify_resource_change
    cmp r4, #2
    bne .L_notify_resource_change
    mov r0, r5
    bl BattleActor_GetById
    cmp r8, #0
    bne .L_notify_resource_change
    ldrsb r0, [r0, #0x38]
    cmp r0, #3
    moveq r0, #0x200
    strheq r0, [r6, #0x5A]

.L_notify_resource_change:
    cmp sl, #0
    addeq sp, sp, #4
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr
    mov r0, sb
    bl BattleSceneObject_GetActiveModel
    ldr r2, .L_battle_context
    mov r1, r0
    ldr r0, [r2]
    add r0, r0, #0x6000
    ldr r0, [r0, #0x8C8]
    ldr r2, [r0]
    ldr r2, [r2, #0x90]
    blx r2
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.L_model_alloc_config:
    .word data_ov002_020bf718
.L_resource_55:
    .word 0xC0000055
.L_resource_53:
    .word 0xC0000053
.L_resource_54:
    .word 0xC0000054
.L_resource_57:
    .word 0xC0000057
.L_resource_56:
    .word 0xC0000056
.L_resource_58:
    .word 0xC0000058
.size BattleSceneObject_SetAnimation, . - BattleSceneObject_SetAnimation
