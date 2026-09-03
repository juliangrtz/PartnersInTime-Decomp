/* Start an enemy's ordinary action-script VM state. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartActionScript, "ax", %progbits
.balign 4

.extern BattleAI_StartScriptTask

.global BattleAI_StartActionScript
.type BattleAI_StartActionScript, %function
BattleAI_StartActionScript:
    ldr ip, .L_start_script
    lsl r0, r0, #0x10
    lsr r0, r0, #0x10
    mov r1, #0x1000
    bx ip
.L_start_script:
    .word BattleAI_StartScriptTask
.size BattleAI_StartActionScript, . - BattleAI_StartActionScript
