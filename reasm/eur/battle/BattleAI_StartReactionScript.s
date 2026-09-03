/* Start an enemy's reaction-script VM state. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartReactionScript, "ax", %progbits
.balign 4

.extern BattleAI_StartScriptTask

.global BattleAI_StartReactionScript
.type BattleAI_StartReactionScript, %function
BattleAI_StartReactionScript:
    ldr ip, .L_start_script
    lsl r0, r0, #0x10
    lsr r0, r0, #0x10
    mov r1, #0x2000
    bx ip
.L_start_script:
    .word BattleAI_StartScriptTask
.size BattleAI_StartReactionScript, . - BattleAI_StartReactionScript
