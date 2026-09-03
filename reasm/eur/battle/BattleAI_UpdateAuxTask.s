/* Run one auxiliary battle-AI VM task until it completes. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_UpdateAuxTask, "ax", %progbits
.balign 4

.extern VM_Run
.extern gBattleContext

.global BattleAI_UpdateAuxTask
.type BattleAI_UpdateAuxTask, %function
BattleAI_UpdateAuxTask:
    push {r4, lr}
    mov r4, r0
    ldr r1, [r4, #0xc]
    ldrh r0, [r1, #0xb2]
    lsl r0, r0, #0x1f
    lsrs r0, r0, #0x1f
    popne {r4, lr}
    bxne lr
    ldr r2, .L_battle_context
    ldr r0, .L_vm_offset
    ldr r2, [r2]
    add r0, r2, r0
    bl VM_Run
    cmp r0, #1
    beq .L_complete
    cmp r0, #2
    popne {r4, lr}
    bxne lr
.L_complete:
    mov r0, #0
    str r0, [r4, #4]
    pop {r4, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_vm_offset:
    .word 0x6954
.size BattleAI_UpdateAuxTask, . - BattleAI_UpdateAuxTask
