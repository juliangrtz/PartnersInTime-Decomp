/* Initialize the action, reaction, auxiliary, and object-script task pools. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_TaskPoolsInit, "ax", %progbits
.balign 4

.extern BattleTaskPool_Init
.extern gBattleAIActionTaskPool
.extern gBattleAIAuxTaskPool
.extern gBattleAIObjectTaskPool
.extern gBattleAIReactionTaskPool

.global BattleAI_TaskPoolsInit
.type BattleAI_TaskPoolsInit, %function
BattleAI_TaskPoolsInit:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r1, #8
    ldr r0, .L_action_pool
    mov r2, r1
    bl BattleTaskPool_Init
    mov r1, #8
    ldr r0, .L_reaction_pool
    mov r2, r1
    bl BattleTaskPool_Init
    mov r1, #8
    ldr r0, .L_aux_pool
    mov r2, r1
    bl BattleTaskPool_Init
    ldr r0, .L_object_pool
    mov r1, #0x28
    mov r2, #8
    bl BattleTaskPool_Init
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_action_pool:
    .word gBattleAIActionTaskPool
.L_reaction_pool:
    .word gBattleAIReactionTaskPool
.L_aux_pool:
    .word gBattleAIAuxTaskPool
.L_object_pool:
    .word gBattleAIObjectTaskPool
.size BattleAI_TaskPoolsInit, . - BattleAI_TaskPoolsInit
