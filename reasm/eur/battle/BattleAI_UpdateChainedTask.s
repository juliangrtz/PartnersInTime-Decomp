/* Run a script task and immediately enter any continuation it activates. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_UpdateChainedTask, "ax", %progbits
.balign 4

.extern BattleAI_HandleVmResult
.extern VM_Run
.extern gBattleContext

.global BattleAI_UpdateChainedTask
.type BattleAI_UpdateChainedTask, %function
BattleAI_UpdateChainedTask:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r5, r0
    ldr r4, [r5, #0xc]
    ldr r0, [r4]
    cmp r0, #0
    bne .L_check_disabled
    mov r1, #0
    str r1, [r5, #4]
    ldr r0, .L_no_order
    strh r1, [r4, #0xb4]
    strh r0, [r4, #0xb6]
    str r1, [r4, #0xb8]
    strh r1, [r4, #0xbc]
    add sp, sp, #4
    strh r0, [r4, #0xbe]
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_check_disabled:
    ldrh r0, [r4, #0xb2]
    lsl r0, r0, #0x1f
    lsrs r0, r0, #0x1f
    addne sp, sp, #4
    popne {r4, r5, r6, r7, lr}
    bxne lr
    ldr r7, .L_battle_context
    ldr r6, .L_vm_offset
.L_run:
    ldr r0, [r7]
    mov r1, r4
    add r0, r0, r6
    bl VM_Run
    mov r1, r0
    mov r0, r5
    mov r2, r4
    bl BattleAI_HandleVmResult
    cmp r0, #0
    bne .L_run
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_no_order:
    .word 0xffff
.L_battle_context:
    .word gBattleContext
.L_vm_offset:
    .word 0x6954
.size BattleAI_UpdateChainedTask, . - BattleAI_UpdateChainedTask
