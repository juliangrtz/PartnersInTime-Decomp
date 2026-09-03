/* Dispatch the previous hit queue, expand active descriptors, and calculate damage. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleHitQueue_Update, "ax", %progbits
.balign 4

.equ BATTLE_CONTEXT_CURRENT_ACTOR_ID, 0x20
.equ BATTLE_CONTEXT_HIT_DESCRIPTORS_HEAD, 0xCAD4
.equ BATTLE_CONTEXT_HIT_QUEUE, 0xCAD8
.equ BATTLE_CONTEXT_FLAGS, 0xD3A0
.equ HIT_RECORD_SIZE, 0x14
.equ HIT_RECORD_SOURCE_ID, 0x00
.equ HIT_RECORD_TARGET_ID, 0x02
.equ HIT_RECORD_POSITION, 0x04
.equ HIT_RECORD_KIND, 0x0A
.equ HIT_RECORD_STATUS_ID, 0x0C
.equ HIT_RECORD_STATUS_CHANCE, 0x0D
.equ HIT_RECORD_STATUS_MAGNITUDE, 0x0E
.equ HIT_RECORD_CALLBACK, 0x10
.equ HIT_DESCRIPTOR_NEXT, 0x00
.equ HIT_DESCRIPTOR_CALLBACK, 0x04
.equ HIT_DESCRIPTOR_SOURCE_ID, 0x08
.equ HIT_DESCRIPTOR_TARGET_ID, 0x0A
.equ HIT_DESCRIPTOR_FLAGS, 0x0C
.equ HIT_DESCRIPTOR_STATUS_CHANCE, 0x0E
.equ HIT_DESCRIPTOR_STATUS_MAGNITUDE, 0x0F
.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ BATTLE_ACTOR_PENDING_DAMAGE, 0x1E
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ SCENE_OBJECT_LINKED_ACTOR_ID, 0xEE
.equ PARTY_ID_FIRST, 0x38
.equ PARTY_ID_SECOND, 0x39
.equ PARTY_ID_END, 0x3C
.equ ENEMY_ID_END, 0x44

.extern BattleActor_GetById
.extern BattleActor_GetPartySlot
.extern BattleCollision_TestObjects
.extern BattleDamage_CalculateByObject
.extern BattleHitDescriptor_GetByActorId
.extern BattleSceneObject_GetById
.extern BattleAI_StartReactionScript
.extern gBattleContext

.global BattleHitQueue_Update
.type BattleHitQueue_Update, %function
BattleHitQueue_Update:
    push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    sub sp, sp, #0x14
    mov r6, #0
    mov r4, r6
    ldr r8, .L_battle_context
    mov sb, r6
    ldr r7, .L_hit_queue_offset

.L_dispatch_previous:
    ldr r0, [r8]
    add r0, r0, r7
    add r5, r0, r4
    ldrsh r0, [r5, #HIT_RECORD_KIND]
    cmp r0, #0
    beq .L_build_queue
    ldrh r0, [r5, #HIT_RECORD_SOURCE_ID]
    cmp r0, #0
    beq .L_previous_next
    ldr r1, [r5, #HIT_RECORD_CALLBACK]
    cmp r1, #0
    beq .L_clear_previous
    mov r0, r5
    blx r1
.L_clear_previous:
    strh sb, [r5, #HIT_RECORD_KIND]
.L_previous_next:
    add r6, r6, #1
    cmp r6, #8
    add r4, r4, #HIT_RECORD_SIZE
    blt .L_dispatch_previous

.L_build_queue:
    ldr r0, .L_battle_context
    ldr r1, .L_hit_queue_offset
    ldr r2, [r0]
    mov r0, #0
    add r5, r2, r1
    ldr r1, .L_battle_context
    strh r0, [r5, #HIT_RECORD_KIND]
    ldr r1, [r1]
    add r1, r1, #0xC000
    ldr r4, [r1, #0xAD4]
    cmp r4, #0
    beq .L_prune_special_sources
    mov r1, #0x28
    str r1, [sp, #4]
    mov r1, #PARTY_ID_END
    mov r7, r0
    mov r6, r0
    mov fp, r0
    str r0, [sp, #0x0C]
    str r0, [sp]
    mov sb, #PARTY_ID_FIRST
    mov r8, #PARTY_ID_SECOND
    str r1, [sp, #8]

.L_descriptor_loop:
    ldr r1, [r4, #HIT_DESCRIPTOR_CALLBACK]
    cmp r1, #0
    beq .L_unlink_descriptor
    ldrh sl, [r4, #HIT_DESCRIPTOR_TARGET_ID]
    cmp sl, #PARTY_ID_FIRST
    bne .L_check_all_targets

    mov r0, sb
    bl BattleActor_GetPartySlot
    ldrsh r1, [r0, #BATTLE_ACTOR_CURRENT_HP]
    cmp r1, #0
    ble .L_second_party_target
    ldrh r0, [r0, #BATTLE_ACTOR_FLAGS]
    mov r0, r0, lsl #22
    movs r0, r0, lsr #31
    bne .L_second_party_target
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r0, lsl #24
    movs r0, r0, lsr #31
    bne .L_second_party_target
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    mov r1, sb
    add r2, r5, #HIT_RECORD_POSITION
    bl BattleCollision_TestObjects
    strh r0, [r5, #HIT_RECORD_KIND]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    ldrsh r1, [r5, #HIT_RECORD_KIND]
    mov r0, r0, lsl #26
    mov r0, r0, lsr #26
    ands r0, r1, r0
    strheq fp, [r5, #HIT_RECORD_KIND]
    beq .L_second_party_target
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    strh r0, [r5, #HIT_RECORD_SOURCE_ID]
    strh sb, [r5, #HIT_RECORD_TARGET_ID]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #25
    strb r0, [r5, #HIT_RECORD_STATUS_ID]
    ldrsb r0, [r4, #HIT_DESCRIPTOR_STATUS_CHANCE]
    strb r0, [r5, #HIT_RECORD_STATUS_CHANCE]
    ldrsb r0, [r4, #HIT_DESCRIPTOR_STATUS_MAGNITUDE]
    strb r0, [r5, #HIT_RECORD_STATUS_MAGNITUDE]
    ldr r0, [r4, #HIT_DESCRIPTOR_CALLBACK]
    str r0, [r5, #HIT_RECORD_CALLBACK]
    add r5, r5, #HIT_RECORD_SIZE

.L_second_party_target:
    mov r0, r8
    bl BattleActor_GetPartySlot
    ldrsh r1, [r0, #BATTLE_ACTOR_CURRENT_HP]
    cmp r1, #0
    ble .L_descriptor_done
    ldrh r0, [r0, #BATTLE_ACTOR_FLAGS]
    mov r0, r0, lsl #22
    movs r0, r0, lsr #31
    bne .L_descriptor_done
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r0, lsl #23
    movs r0, r0, lsr #31
    bne .L_descriptor_done
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    mov r1, r8
    add r2, r5, #HIT_RECORD_POSITION
    bl BattleCollision_TestObjects
    strh r0, [r5, #HIT_RECORD_KIND]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    ldrsh r1, [r5, #HIT_RECORD_KIND]
    mov r0, r0, lsl #26
    mov r0, r0, lsr #26
    ands r0, r1, r0
    ldreq r0, [sp]
    strheq r0, [r5, #HIT_RECORD_KIND]
    beq .L_descriptor_done
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    strh r0, [r5, #HIT_RECORD_SOURCE_ID]
    strh r8, [r5, #HIT_RECORD_TARGET_ID]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #25
    strb r0, [r5, #HIT_RECORD_STATUS_ID]
    ldrsb r0, [r4, #HIT_DESCRIPTOR_STATUS_CHANCE]
    strb r0, [r5, #HIT_RECORD_STATUS_CHANCE]
    ldrsb r0, [r4, #HIT_DESCRIPTOR_STATUS_MAGNITUDE]
    strb r0, [r5, #HIT_RECORD_STATUS_MAGNITUDE]
    ldr r0, [r4, #HIT_DESCRIPTOR_CALLBACK]
    str r0, [r5, #HIT_RECORD_CALLBACK]
    add r5, r5, #HIT_RECORD_SIZE
    b .L_descriptor_done

.L_check_all_targets:
    cmp sl, #PARTY_ID_END
    bne .L_single_target
    ldr sl, [sp, #4]

.L_all_object_targets:
    mov r0, sl, lsl #16
    mov r0, r0, lsr #16
    bl BattleHitDescriptor_GetByActorId
    ldr r0, [r0, #HIT_DESCRIPTOR_CALLBACK]
    cmp r0, #0
    beq .L_next_object_target
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    mov r1, sl
    add r2, r5, #HIT_RECORD_POSITION
    bl BattleCollision_TestObjects
    strh r0, [r5, #HIT_RECORD_KIND]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    ldrsh r1, [r5, #HIT_RECORD_KIND]
    mov r0, r0, lsl #26
    mov r0, r0, lsr #26
    ands r0, r1, r0
    strheq r7, [r5, #HIT_RECORD_KIND]
    beq .L_next_object_target
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    strh r0, [r5, #HIT_RECORD_SOURCE_ID]
    strh sl, [r5, #HIT_RECORD_TARGET_ID]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #25
    strb r0, [r5, #HIT_RECORD_STATUS_ID]
    ldrsb r0, [r4, #HIT_DESCRIPTOR_STATUS_CHANCE]
    strb r0, [r5, #HIT_RECORD_STATUS_CHANCE]
    ldrsb r0, [r4, #HIT_DESCRIPTOR_STATUS_MAGNITUDE]
    strb r0, [r5, #HIT_RECORD_STATUS_MAGNITUDE]
    ldr r0, [r4, #HIT_DESCRIPTOR_CALLBACK]
    str r0, [r5, #HIT_RECORD_CALLBACK]
    add r5, r5, #HIT_RECORD_SIZE
.L_next_object_target:
    add sl, sl, #1
    cmp sl, #PARTY_ID_FIRST
    blt .L_all_object_targets

    ldr sl, [sp, #8]
.L_all_enemy_targets:
    mov r0, sl, lsl #16
    mov r0, r0, lsr #16
    bl BattleHitDescriptor_GetByActorId
    ldr r0, [r0, #HIT_DESCRIPTOR_CALLBACK]
    cmp r0, #0
    beq .L_next_enemy_target
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    mov r1, sl
    add r2, r5, #HIT_RECORD_POSITION
    bl BattleCollision_TestObjects
    strh r0, [r5, #HIT_RECORD_KIND]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    ldrsh r1, [r5, #HIT_RECORD_KIND]
    mov r0, r0, lsl #26
    mov r0, r0, lsr #26
    ands r0, r1, r0
    strheq r6, [r5, #HIT_RECORD_KIND]
    beq .L_next_enemy_target
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    strh r0, [r5, #HIT_RECORD_SOURCE_ID]
    strh sl, [r5, #HIT_RECORD_TARGET_ID]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #25
    strb r0, [r5, #HIT_RECORD_STATUS_ID]
    ldrsb r0, [r4, #HIT_DESCRIPTOR_STATUS_CHANCE]
    strb r0, [r5, #HIT_RECORD_STATUS_CHANCE]
    ldrsb r0, [r4, #HIT_DESCRIPTOR_STATUS_MAGNITUDE]
    strb r0, [r5, #HIT_RECORD_STATUS_MAGNITUDE]
    ldr r0, [r4, #HIT_DESCRIPTOR_CALLBACK]
    str r0, [r5, #HIT_RECORD_CALLBACK]
    add r5, r5, #HIT_RECORD_SIZE
.L_next_enemy_target:
    add sl, sl, #1
    cmp sl, #ENEMY_ID_END
    blt .L_all_enemy_targets
    b .L_descriptor_done

.L_single_target:
    cmp sl, #PARTY_ID_END
    blo .L_check_party_target_lock
    cmp sl, #ENEMY_ID_END
    bhs .L_check_party_target_lock
    mov r0, sl, lsl #16
    mov r0, r0, lsr #16
    bl BattleActor_GetById
    ldrh r0, [r0, #BATTLE_ACTOR_FLAGS]
    mov r0, r0, lsl #22
    movs r0, r0, lsr #31
    bne .L_descriptor_done
.L_check_party_target_lock:
    cmp sl, #PARTY_ID_FIRST
    blo .L_test_single_target
    cmp sl, #PARTY_ID_END
    bhs .L_test_single_target
    mov r0, sl, lsl #16
    mov r0, r0, lsr #16
    bl BattleActor_GetById
    ldrh r0, [r0, #BATTLE_ACTOR_FLAGS]
    mov r0, r0, lsl #22
    movs r0, r0, lsr #31
    bne .L_descriptor_done
.L_test_single_target:
    ldrh r0, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    mov r1, sl
    add r2, r5, #HIT_RECORD_POSITION
    bl BattleCollision_TestObjects
    strh r0, [r5, #HIT_RECORD_KIND]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    ldrsh r1, [r5, #HIT_RECORD_KIND]
    mov r0, r0, lsl #26
    mov r0, r0, lsr #26
    ands r0, r1, r0
    ldreq r0, [sp, #0x0C]
    strheq r0, [r5, #HIT_RECORD_KIND]
    beq .L_descriptor_done
    ldrh r1, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    mov r0, sl, lsl #16
    mov r0, r0, lsr #16
    strh r1, [r5, #HIT_RECORD_SOURCE_ID]
    strh r0, [r5, #HIT_RECORD_TARGET_ID]
    ldrh r1, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r1, r1, lsl #16
    mov r1, r1, lsr #25
    strb r1, [r5, #HIT_RECORD_STATUS_ID]
    ldrsb r1, [r4, #HIT_DESCRIPTOR_STATUS_CHANCE]
    strb r1, [r5, #HIT_RECORD_STATUS_CHANCE]
    ldrsb r1, [r4, #HIT_DESCRIPTOR_STATUS_MAGNITUDE]
    strb r1, [r5, #HIT_RECORD_STATUS_MAGNITUDE]
    ldr r1, [r4, #HIT_DESCRIPTOR_CALLBACK]
    str r1, [r5, #HIT_RECORD_CALLBACK]
    add r5, r5, #HIT_RECORD_SIZE
    bl BattleSceneObject_GetById
    strh sl, [r0, #SCENE_OBJECT_LINKED_ACTOR_ID]

.L_descriptor_done:
    mov r0, r4
    b .L_advance_descriptor

.L_unlink_descriptor:
    cmp r0, #0
    ldrne r1, [r4, #HIT_DESCRIPTOR_NEXT]
    strne r1, [r0, #HIT_DESCRIPTOR_NEXT]
    ldreq r1, .L_battle_context
    ldreq r2, [r4, #HIT_DESCRIPTOR_NEXT]
    ldreq r1, [r1]
    addeq r1, r1, #0xC000
    streq r2, [r1, #0xAD4]
    ldrh r1, [r4, #HIT_DESCRIPTOR_FLAGS]
    bic r1, r1, #0x40
    strh r1, [r4, #HIT_DESCRIPTOR_FLAGS]
.L_advance_descriptor:
    ldr r4, [r4, #HIT_DESCRIPTOR_NEXT]
    cmp r4, #0
    bne .L_descriptor_loop

.L_prune_special_sources:
    ldr r1, .L_battle_context
    ldr r0, .L_hit_queue_offset
    ldr r2, [r1]
    mov r1, #0
    add r4, r2, r0
    mov r0, r4
    mov r5, r1
    mov r7, r1
    ldr r2, .L_minus_party_id_first
.L_special_outer:
    ldrsh r3, [r0, #HIT_RECORD_KIND]
    cmp r3, #0
    beq .L_prune_chains
    ldrh r3, [r0, #HIT_RECORD_SOURCE_ID]
    add r3, r3, r2
    mov r3, r3, lsl #16
    mov r3, r3, lsr #16
    cmp r3, #1
    bhi .L_special_next
    mov sb, r7
    mov r8, r4
.L_special_inner:
    ldrsh r3, [r8, #HIT_RECORD_KIND]
    cmp r3, #0
    beq .L_special_next
    ldrh r6, [r8, #HIT_RECORD_SOURCE_ID]
    ldrh r3, [r0, #HIT_RECORD_TARGET_ID]
    add sb, sb, #1
    cmp r6, r3
    strheq r5, [r8, #HIT_RECORD_SOURCE_ID]
    strheq r5, [r8, #HIT_RECORD_TARGET_ID]
    streq r5, [r8, #HIT_RECORD_CALLBACK]
    cmp sb, #8
    add r8, r8, #HIT_RECORD_SIZE
    blt .L_special_inner
.L_special_next:
    add r1, r1, #1
    cmp r1, #8
    add r0, r0, #HIT_RECORD_SIZE
    blt .L_special_outer

.L_prune_chains:
    mov r6, r4
    mov r8, #0
    mov r0, r8
    mov r3, r8
.L_chain_outer:
    ldrsh r1, [r6, #HIT_RECORD_KIND]
    cmp r1, #0
    beq .L_prune_duplicate_targets
    ldrh r1, [r6, #HIT_RECORD_SOURCE_ID]
    cmp r1, #0
    beq .L_chain_next
    mov r7, r3
    mov r5, r4
.L_chain_inner:
    ldrsh r1, [r5, #HIT_RECORD_KIND]
    cmp r1, #0
    beq .L_chain_next
    ldrh r2, [r5, #HIT_RECORD_SOURCE_ID]
    ldrh r1, [r6, #HIT_RECORD_TARGET_ID]
    add r7, r7, #1
    cmp r2, r1
    strheq r0, [r5, #HIT_RECORD_SOURCE_ID]
    strheq r0, [r5, #HIT_RECORD_TARGET_ID]
    streq r0, [r5, #HIT_RECORD_CALLBACK]
    cmp r7, #8
    add r5, r5, #HIT_RECORD_SIZE
    blt .L_chain_inner
.L_chain_next:
    add r8, r8, #1
    cmp r8, #8
    add r6, r6, #HIT_RECORD_SIZE
    blt .L_chain_outer

.L_prune_duplicate_targets:
    mov r6, r4
    mov r8, #0
    mov r1, r8
    mov r0, #HIT_RECORD_SIZE
.L_duplicate_outer:
    ldrsh r2, [r6, #HIT_RECORD_KIND]
    cmp r2, #0
    beq .L_calculate_damage
    ldrh r2, [r6, #HIT_RECORD_SOURCE_ID]
    cmp r2, #0
    beq .L_duplicate_next
    add r7, r8, #1
    cmp r7, #8
    bge .L_duplicate_next
    mla r5, r7, r0, r4
.L_duplicate_inner:
    ldrsh r2, [r5, #HIT_RECORD_KIND]
    cmp r2, #0
    beq .L_duplicate_next
    ldrh r3, [r5, #HIT_RECORD_TARGET_ID]
    ldrh r2, [r6, #HIT_RECORD_TARGET_ID]
    add r7, r7, #1
    cmp r3, r2
    strheq r1, [r5, #HIT_RECORD_SOURCE_ID]
    strheq r1, [r5, #HIT_RECORD_TARGET_ID]
    streq r1, [r5, #HIT_RECORD_CALLBACK]
    cmp r7, #8
    add r5, r5, #HIT_RECORD_SIZE
    blt .L_duplicate_inner
.L_duplicate_next:
    add r8, r8, #1
    cmp r8, #8
    add r6, r6, #HIT_RECORD_SIZE
    blt .L_duplicate_outer

.L_calculate_damage:
    ldr r8, .L_battle_context
    mov r7, #0
.L_damage_loop:
    ldrsh r0, [r4, #HIT_RECORD_KIND]
    cmp r0, #0
    addeq sp, sp, #0x14
    popeq {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bxeq lr
    ldrh r0, [r4, #HIT_RECORD_SOURCE_ID]
    cmp r0, #0
    beq .L_damage_next
    bl BattleSceneObject_GetById
    ldrh r6, [r0, #SCENE_OBJECT_LINKED_ACTOR_ID]
    ldrh r0, [r4, #HIT_RECORD_TARGET_ID]
    bl BattleSceneObject_GetById
    ldrh r5, [r0, #SCENE_OBJECT_LINKED_ACTOR_ID]
    cmp r5, #PARTY_ID_FIRST
    blo .L_check_enemy_target
    cmp r5, #PARTY_ID_END
    blo .L_check_active_actor
.L_check_enemy_target:
    cmp r5, #PARTY_ID_END
    blo .L_after_damage
    cmp r5, #ENEMY_ID_END
    bhs .L_after_damage
.L_check_active_actor:
    ldr r0, [r8]
    ldrh r0, [r0, #BATTLE_CONTEXT_CURRENT_ACTOR_ID]
    cmp r0, #PARTY_ID_FIRST
    blo .L_compute_damage
    cmp r0, #PARTY_ID_END
    blo .L_after_damage
.L_compute_damage:
    ldrh r0, [r4, #HIT_RECORD_SOURCE_ID]
    ldrh r1, [r4, #HIT_RECORD_TARGET_ID]
    bl BattleDamage_CalculateByObject
    movs sb, r0
    bmi .L_reverse_damage
    mov r0, r5
    bl BattleActor_GetById
    strh sb, [r0, #BATTLE_ACTOR_PENDING_DAMAGE]
.L_reverse_damage:
    cmp sb, #0
    bge .L_after_damage
    mov r0, r6
    bl BattleActor_GetById
    rsb r1, sb, #0
    strh r1, [r0, #BATTLE_ACTOR_PENDING_DAMAGE]
    ldrh r1, [r4, #HIT_RECORD_SOURCE_ID]
    ldrh r0, [r4, #HIT_RECORD_TARGET_ID]
    mov r5, r6
    strh r0, [r4, #HIT_RECORD_SOURCE_ID]
    strh r1, [r4, #HIT_RECORD_TARGET_ID]
.L_after_damage:
    cmp r5, #PARTY_ID_END
    blo .L_damage_next
    cmp r5, #ENEMY_ID_END
    bhs .L_damage_next
    ldr r0, [r8]
    ldrh r0, [r0, #BATTLE_CONTEXT_CURRENT_ACTOR_ID]
    cmp r0, #PARTY_ID_END
    blo .L_clear_processing_flag
    cmp r0, #ENEMY_ID_END
    bhs .L_clear_processing_flag
    mov r0, r5
    bl BattleAI_StartReactionScript
.L_clear_processing_flag:
    ldr r0, [r8]
    add r0, r0, #0xD000
    ldr r1, [r0, #0x3A0]
    bic r1, r1, #0x1000
    str r1, [r0, #0x3A0]
.L_damage_next:
    add r7, r7, #1
    cmp r7, #8
    add r4, r4, #HIT_RECORD_SIZE
    blt .L_damage_loop
    add sp, sp, #0x14
    pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.L_hit_queue_offset:
    .word BATTLE_CONTEXT_HIT_QUEUE
.L_minus_party_id_first:
    .word 0x0000FFC8
.size BattleHitQueue_Update, . - BattleHitQueue_Update
