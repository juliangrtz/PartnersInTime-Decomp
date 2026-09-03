/* Stop a party VM slot or a typed battle-AI task identified by its packed ID. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StopScriptById, "ax", %progbits
.balign 4

.extern BattleAITask_StopById
.extern gBattleAIAuxTaskPool
.extern gBattleAIObjectTaskPool
.extern gBattleAIActionTaskPool
.extern gBattleAIReactionTaskPool
.extern gBattleContext

.global BattleAI_StopScriptById
.type BattleAI_StopScriptById, %function
BattleAI_StopScriptById:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r2, r0
    cmp r2, #4
    addls pc, pc, r2, lsl #2
    b .L_typed_id
    b .L_typed_id
    b .L_stop_party_1
    b .L_stop_party_2
    b .L_stop_party_3
    b .L_stop_party_4
.L_stop_party_1:
    ldr r0, .L_battle_context
    mov r1, #0
    ldr r0, [r0]
    add sp, sp, #4
    add r0, r0, #0x6000
    str r1, [r0, #0xa64]
    ldmia sp!, {lr}
    bx lr
.L_stop_party_2:
    ldr r0, .L_battle_context
    mov r1, #0
    ldr r0, [r0]
    add sp, sp, #4
    add r0, r0, #0x6000
    str r1, [r0, #0xb1c]
    ldmia sp!, {lr}
    bx lr
.L_stop_party_3:
    ldr r0, .L_battle_context
    mov r1, #0
    ldr r0, [r0]
    add sp, sp, #4
    add r0, r0, #0x6000
    str r1, [r0, #0xbd4]
    ldmia sp!, {lr}
    bx lr
.L_stop_party_4:
    ldr r0, .L_battle_context
    mov r1, #0
    ldr r0, [r0]
    add sp, sp, #4
    add r0, r0, #0x6000
    str r1, [r0, #0xc8c]
    ldmia sp!, {lr}
    bx lr
.L_typed_id:
    and r0, r2, #0xf000
    cmp r0, #0x2000
    bgt .L_above_reaction
    cmp r0, #0x2000
    bge .L_stop_reaction
    cmp r0, #0x1000
    beq .L_stop_action
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_above_reaction:
    cmp r0, #0x3000
    bgt .L_above_aux
    cmp r0, #0x3000
    beq .L_stop_aux
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_above_aux:
    cmp r0, #0x4000
    beq .L_stop_event
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_stop_action:
    ldr r1, .L_id_mask
    ldr r0, .L_action_pool
    and r1, r2, r1
    bl BattleAITask_StopById
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_stop_reaction:
    ldr r1, .L_id_mask
    ldr r0, .L_reaction_pool
    and r1, r2, r1
    bl BattleAITask_StopById
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_stop_aux:
    ldr r1, .L_id_mask
    ldr r0, .L_aux_pool
    and r1, r2, r1
    bl BattleAITask_StopById
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_stop_event:
    ldr r1, .L_id_mask
    ldr r0, .L_event_pool
    and r1, r2, r1
    bl BattleAITask_StopById
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_id_mask:
    .word 0x0fff
.L_action_pool:
    .word gBattleAIActionTaskPool
.L_reaction_pool:
    .word gBattleAIReactionTaskPool
.L_aux_pool:
    .word gBattleAIAuxTaskPool
.L_event_pool:
    .word gBattleAIObjectTaskPool
.size BattleAI_StopScriptById, . - BattleAI_StopScriptById
