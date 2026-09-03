/* Initialize an enemy VM state and attach its sorted action or reaction task. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartScriptTask, "ax", %progbits
.balign 4

.extern BattleAITask_GetOrInsert
.extern BattleAI_UpdateActionTask
.extern BattleAI_UpdateReactionTask
.extern func_0202cbd4
.extern gBattleContext
.extern gBattleAIActionTaskPool
.extern gBattleAIReactionTaskPool

.global BattleAI_StartScriptTask
.type BattleAI_StartScriptTask, %function
BattleAI_StartScriptTask:
    push {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    ldr r2, .L_battle_context
    mov r8, r0
    sub r0, r8, #0x3c
    ldr r2, [r2]
    lsl r0, r0, #0x10
    add r0, r2, r0, asr #14
    add r0, r0, #0x6000
    ldr r0, [r0, #0x508]
    mov r7, r1
    cmp r7, #0x1000
    ldr r9, [r0, #0x6c]
    addeq r6, r0, #0x70
    ldreq r4, .L_action_pool
    ldreq r5, .L_action_callback
    beq .L_initialize
    cmp r7, #0x2000
    ldreq r4, .L_reaction_pool
    ldreq r5, .L_reaction_callback
    addeq r6, r0, #0x128
.L_initialize:
    mov r0, r6
    mov r1, #0
    mov r2, #0xb8
    bl func_0202cbd4
    ldr r0, [r9, #4]
    orr r2, r7, r8
    str r0, [r6]
    mov r0, r4
    mov r1, r8
    strh r2, [r6, #0xb0]
    bl BattleAITask_GetOrInsert
    str r5, [r0, #4]
    str r6, [r0, #0xc]
    strh r8, [r0, #0x10]
    add sp, sp, #4
    pop {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_action_pool:
    .word gBattleAIActionTaskPool
.L_action_callback:
    .word BattleAI_UpdateActionTask
.L_reaction_pool:
    .word gBattleAIReactionTaskPool
.L_reaction_callback:
    .word BattleAI_UpdateReactionTask
.size BattleAI_StartScriptTask, . - BattleAI_StartScriptTask
