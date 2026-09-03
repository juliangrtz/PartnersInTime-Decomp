/* Configure and link the per-source descriptor later expanded into queued hits. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleHitDescriptor_Configure, "ax", %progbits
.balign 4

.equ BATTLE_CONTEXT_ACTIVE_HIT_DESCRIPTORS, 0xCAD4
.equ HIT_DESCRIPTOR_NEXT, 0x00
.equ HIT_DESCRIPTOR_CALLBACK, 0x04
.equ HIT_DESCRIPTOR_SOURCE_ID, 0x08
.equ HIT_DESCRIPTOR_TARGET_ID, 0x0A
.equ HIT_DESCRIPTOR_FLAGS, 0x0C
.equ SCENE_OBJECT_LINKED_ACTOR_ID, 0xEE
.equ HIT_KIND_DEFAULT, 0x3F
.equ INVALID_OBJECT_ID, 0xFFFF

.extern BattleDamage_DispatchHit
.extern BattleHitDescriptor_GetByActorId
.extern BattleSceneObject_GetById
.extern gBattleContext

.global BattleHitDescriptor_Configure
.type BattleHitDescriptor_Configure, %function
BattleHitDescriptor_Configure:
    push {r4, r5, r6, r7, r8, lr}
    mov r8, r0
    mov r7, r1
    mov r6, r2
    mov r5, r3
    bl BattleHitDescriptor_GetByActorId
    mov r4, r0
    ldrh r3, [r4, #HIT_DESCRIPTOR_FLAGS]
    ldr r2, .L_invalid_object_id
    add r1, r4, #HIT_DESCRIPTOR_FLAGS
    bic r3, r3, #0xFE00
    cmp r7, r2
    strh r3, [r4, #HIT_DESCRIPTOR_FLAGS]
    movhs r1, #0
    strhs r1, [r4, #HIT_DESCRIPTOR_CALLBACK]
    pophs {r4, r5, r6, r7, r8, lr}
    bxhs lr

    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r0, lsl #25
    movs r0, r0, lsr #31
    bne .L_linked
    ldrh r0, [r1]
    ldr r2, .L_battle_context
    orr r0, r0, #0x40
    strh r0, [r1]
    ldr r0, [r2]
    add r0, r0, #0xC000
    ldr r0, [r0, #0xAD4]
    str r0, [r4, #HIT_DESCRIPTOR_NEXT]
    ldr r0, [r2]
    add r0, r0, #0xC000
    str r4, [r0, #0xAD4]

.L_linked:
    ldr r0, [sp, #0x18]
    strh r8, [r4, #HIT_DESCRIPTOR_SOURCE_ID]
    strh r7, [r4, #HIT_DESCRIPTOR_TARGET_ID]
    cmp r0, #0
    moveq r0, #HIT_KIND_DEFAULT
    ldrh r1, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    cmp r6, #0
    ldreq r6, .L_default_callback
    bic r1, r1, #0x3F
    and r0, r0, #0x3F
    orr r0, r1, r0
    strh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    str r6, [r4, #HIT_DESCRIPTOR_CALLBACK]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    cmp r5, #0
    moveq r5, r8
    bic r0, r0, #0x80
    strh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    ldrh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    bic r0, r0, #0x100
    strh r0, [r4, #HIT_DESCRIPTOR_FLAGS]
    mov r0, r8
    bl BattleSceneObject_GetById
    strh r5, [r0, #SCENE_OBJECT_LINKED_ACTOR_ID]
    mov r0, r4
    pop {r4, r5, r6, r7, r8, lr}
    bx lr

.L_invalid_object_id:
    .word INVALID_OBJECT_ID
.L_battle_context:
    .word gBattleContext
.L_default_callback:
    .word BattleDamage_DispatchHit
.size BattleHitDescriptor_Configure, . - BattleHitDescriptor_Configure
