/* Reverse queued hits targeting one actor and rebuild their damage/status data. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_ReflectQueuedHits, "ax", %progbits
.balign 4

.equ BATTLE_CONTEXT_HIT_QUEUE, 0xCAD8
.equ HIT_RECORD_SIZE, 0x14
.equ HIT_RECORD_SOURCE_ID, 0x00
.equ HIT_RECORD_TARGET_ID, 0x02
.equ HIT_RECORD_KIND, 0x0A
.equ HIT_RECORD_STATUS_ID, 0x0C
.equ HIT_RECORD_STATUS_CHANCE, 0x0D
.equ HIT_RECORD_STATUS_MAGNITUDE, 0x0E
.equ BATTLE_ACTOR_PENDING_DAMAGE, 0x1E
.equ SCENE_OBJECT_LINKED_ACTOR_ID, 0xEE

.extern BattleActor_GetById
.extern BattleDamage_CalculateByObject
.extern BattleSceneObject_GetById
.extern func_ov002_0209ebac
.extern gBattleContext

.global BattleDamage_ReflectQueuedHits
.type BattleDamage_ReflectQueuedHits, %function
BattleDamage_ReflectQueuedHits:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    ldr r2, .L_battle_context
    ldr r1, .L_hit_queue_offset
    ldr r2, [r2]
    mov r6, r0
    add r4, r2, r1
    mov r5, #0
.L_loop:
    ldrsh r0, [r4, #HIT_RECORD_KIND]
    cmp r0, #0
    addeq sp, sp, #4
    popeq {r4, r5, r6, r7, lr}
    bxeq lr
    ldrh r0, [r4, #HIT_RECORD_TARGET_ID]
    cmp r6, r0
    bne .L_next

    ldrh r1, [r4, #HIT_RECORD_SOURCE_ID]
    bl BattleDamage_CalculateByObject
    mov r7, r0
    cmp r7, #0
    ble .L_swap
    ldrh r0, [r4, #HIT_RECORD_SOURCE_ID]
    bl BattleSceneObject_GetById
    ldrh r0, [r0, #SCENE_OBJECT_LINKED_ACTOR_ID]
    bl BattleActor_GetById
    strh r7, [r0, #BATTLE_ACTOR_PENDING_DAMAGE]

.L_swap:
    ldrh r0, [r4, #HIT_RECORD_TARGET_ID]
    ldrh r1, [r4, #HIT_RECORD_SOURCE_ID]
    strh r1, [r4, #HIT_RECORD_TARGET_ID]
    strh r0, [r4, #HIT_RECORD_SOURCE_ID]
    bl func_ov002_0209ebac
    ldrh r1, [r0, #0x0C]
    mov r1, r1, lsl #16
    mov r1, r1, lsr #25
    strb r1, [r4, #HIT_RECORD_STATUS_ID]
    ldrsb r1, [r0, #0x0E]
    strb r1, [r4, #HIT_RECORD_STATUS_CHANCE]
    ldrsb r0, [r0, #0x0F]
    strb r0, [r4, #HIT_RECORD_STATUS_MAGNITUDE]

.L_next:
    add r5, r5, #1
    cmp r5, #8
    add r4, r4, #HIT_RECORD_SIZE
    blt .L_loop
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_hit_queue_offset:
    .word BATTLE_CONTEXT_HIT_QUEUE
.size BattleDamage_ReflectQueuedHits, . - BattleDamage_ReflectQueuedHits
