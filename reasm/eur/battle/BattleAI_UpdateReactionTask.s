/* Run one enemy reaction VM task until its script reports completion. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_UpdateReactionTask, "ax", %progbits
.balign 4

.extern VM_Run
.extern BattleAI_TryClearOrderWait
.extern gBattleContext

.global BattleAI_UpdateReactionTask
.type BattleAI_UpdateReactionTask, %function
BattleAI_UpdateReactionTask:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    ldr r4, [r5, #0xc]
    ldrh r0, [r4, #0xb2]
    lsl r1, r0, #0x1f
    lsrs r1, r1, #0x1f
    addne sp, sp, #4
    popne {r4, r5, lr}
    bxne lr
    lsl r0, r0, #0x1e
    lsrs r0, r0, #0x1f
    beq .L_run_vm
    mov r0, r4
    bl BattleAI_TryClearOrderWait
    cmp r0, #0
    addeq sp, sp, #4
    popeq {r4, r5, lr}
    bxeq lr
.L_run_vm:
    ldr r1, .L_battle_context
    ldr r0, .L_vm_offset
    ldr r2, [r1]
    mov r1, r4
    add r0, r2, r0
    bl VM_Run
    cmp r0, #1
    beq .L_complete
    cmp r0, #2
    addne sp, sp, #4
    popne {r4, r5, lr}
    bxne lr
.L_complete:
    mov r0, #0
    str r0, [r5, #4]
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_vm_offset:
    .word 0x6954
.size BattleAI_UpdateReactionTask, . - BattleAI_UpdateReactionTask
