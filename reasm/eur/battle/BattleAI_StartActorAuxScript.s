/* Initialize the auxiliary VM state embedded in one enemy actor. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartActorAuxScript, "ax", %progbits
.balign 4

.extern BattleAITask_GetOrInsert
.extern BattleAI_UpdateAuxTask
.extern gBattleAIAuxTaskPool
.extern gBattleContext

.global BattleAI_StartActorAuxScript
.type BattleAI_StartActorAuxScript, %function
BattleAI_StartActorAuxScript:
    push {r4, r5, lr}
    sub sp, sp, #4
    ldr r3, .L_battle_context
    mov r4, r0
    sub r0, r4, #0x3c
    ldr r3, [r3]
    lsl r0, r0, #0x10
    add r0, r3, r0, asr #14
    add r0, r0, #0x6000
    ldr r0, [r0, #0x508]
    mov r3, #0
    strh r1, [r0, #0x20]
    str r2, [r0, #0x1e0]
    add r5, r0, #0x1e0
    strh r3, [r5, #0xa8]
    strh r3, [r5, #0xaa]
    ldrh r2, [r5, #0xb2]
    ldr r0, .L_aux_pool
    mov r1, r4
    bic r2, r2, #1
    strh r2, [r5, #0xb2]
    strh r3, [r5, #0xb4]
    orr r2, r4, #0x3000
    strh r2, [r5, #0xb0]
    bl BattleAITask_GetOrInsert
    ldr r1, .L_update_callback
    str r1, [r0, #4]
    str r5, [r0, #0xc]
    strh r4, [r0, #0x10]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_aux_pool:
    .word gBattleAIAuxTaskPool
.L_update_callback:
    .word BattleAI_UpdateAuxTask
.size BattleAI_StartActorAuxScript, . - BattleAI_StartActorAuxScript
