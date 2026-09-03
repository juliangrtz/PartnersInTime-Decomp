/* Start or queue a continuation for one object's 0x4000-family VM state. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartObjectScript, "ax", %progbits
.balign 4

.extern BattleAITask_GetOrInsert
.extern BattleAI_UpdateChainedTask
.extern data_ov002_020c0918
.extern BattleScriptState_GetByObjectId

.global BattleAI_StartObjectScript
.type BattleAI_StartObjectScript, %function
BattleAI_StartObjectScript:
    push {r4, r5, r6, r7, r8, lr}
    mov r7, r0
    lsl r0, r7, #0x10
    lsr r0, r0, #0x10
    mov r6, r1
    mov r5, r2
    mov r8, r3
    bl BattleScriptState_GetByObjectId
    mov r4, r0
    ldr r0, [r4]
    cmp r0, #0
    beq .L_start_now
    cmp r8, #0
    beq .L_start_now
    ldrh r1, [r5, #0xb0]
    cmp r1, #0x4000
    ldrlo r0, .L_no_order
    strhlo r1, [r4, #0xbc]
    strhlo r0, [r4, #0xbe]
    blo .L_store_continuation
    ldrh r1, [r5, #0xb4]
    ldr r0, .L_id_mask
    strh r1, [r4, #0xbc]
    ldrh r1, [r5, #0xb0]
    and r0, r1, r0
    strh r0, [r4, #0xbe]
.L_store_continuation:
    str r6, [r4, #0xb8]
    b .L_queue_task
.L_start_now:
    ldrh r1, [r5, #0xb0]
    cmp r1, #0x4000
    ldrlo r0, .L_no_order
    strhlo r1, [r4, #0xb4]
    strhlo r0, [r4, #0xb6]
    blo .L_initialize
    ldrh r1, [r5, #0xb4]
    ldr r0, .L_id_mask
    strh r1, [r4, #0xb4]
    ldrh r1, [r5, #0xb0]
    and r0, r1, r0
    strh r0, [r4, #0xb6]
.L_initialize:
    str r6, [r4]
    mov r2, #0
    strh r2, [r4, #0xa8]
    strh r2, [r4, #0xaa]
    ldrh r1, [r4, #0xb2]
    ldr r0, .L_no_order
    bic r1, r1, #1
    strh r1, [r4, #0xb2]
    str r2, [r4, #0xb8]
    strh r2, [r4, #0xbc]
    strh r0, [r4, #0xbe]
.L_queue_task:
    ldr r0, .L_object_pool
    orr r2, r7, #0x4000
    mov r1, r7
    strh r2, [r4, #0xb0]
    bl BattleAITask_GetOrInsert
    ldr r1, .L_update_callback
    str r1, [r0, #4]
    str r4, [r0, #0xc]
    strh r7, [r0, #0x10]
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.L_no_order:
    .word 0xffff
.L_id_mask:
    .word 0x0fff
.L_object_pool:
    .word data_ov002_020c0918
.L_update_callback:
    .word BattleAI_UpdateChainedTask
.size BattleAI_StartObjectScript, . - BattleAI_StartObjectScript
