/* Run the fixed party VM states and all four battle-AI task lists. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_UpdateAll, "ax", %progbits
.balign 4

.extern BattleTaskList_Update
.extern VM_Run
.extern func_ov002_020698cc
.extern gBattleAIActionTaskPool
.extern gBattleAIAuxTaskPool
.extern gBattleAIObjectTaskPool
.extern gBattleAIReactionTaskPool
.extern gBattleContext

.global BattleAI_UpdateAll
.type BattleAI_UpdateAll, %function
BattleAI_UpdateAll:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_battle_context
    ldr r2, [r0]
    cmp r2, #0
    addeq sp, sp, #4
    ldmiaeq sp!, {lr}
    bxeq lr
    add r0, r2, #0x6000
    ldr r1, [r0, #0xa64]
    ldr r0, .L_party_vm_base
    cmp r1, #0
    add r1, r2, r0
    beq .L_update_party_2
    ldr r0, .L_vm_offset
    add r0, r2, r0
    bl VM_Run
.L_update_party_2:
    ldr r1, .L_battle_context
    ldr r0, .L_vm_offset
    ldr r2, [r1]
    ldr r1, .L_party_2_offset
    add r0, r2, r0
    add r1, r2, r1
    bl func_ov002_020698cc
    ldr r1, .L_battle_context
    ldr r0, .L_party_3_offset
    ldr r3, [r1]
    add r1, r3, #0x6000
    ldr r2, [r1, #0xbd4]
    add r1, r3, r0
    cmp r2, #0
    beq .L_update_party_4
    ldr r0, .L_vm_offset
    add r0, r3, r0
    bl VM_Run
.L_update_party_4:
    ldr r1, .L_battle_context
    ldr r0, .L_party_4_offset
    ldr r3, [r1]
    add r1, r3, #0x6000
    ldr r2, [r1, #0xc8c]
    add r1, r3, r0
    cmp r2, #0
    beq .L_update_tasks
    ldr r0, .L_vm_offset
    add r0, r3, r0
    bl VM_Run
.L_update_tasks:
    ldr r0, .L_action_pool
    bl BattleTaskList_Update
    ldr r0, .L_reaction_pool
    bl BattleTaskList_Update
    ldr r0, .L_aux_pool
    bl BattleTaskList_Update
    ldr r0, .L_object_pool
    bl BattleTaskList_Update
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_party_vm_base:
    .word 0x6a64
.L_vm_offset:
    .word 0x6954
.L_party_2_offset:
    .word 0x6b1c
.L_party_3_offset:
    .word 0x6bd4
.L_party_4_offset:
    .word 0x6c8c
.L_action_pool:
    .word gBattleAIActionTaskPool
.L_reaction_pool:
    .word gBattleAIReactionTaskPool
.L_aux_pool:
    .word gBattleAIAuxTaskPool
.L_object_pool:
    .word gBattleAIObjectTaskPool
.size BattleAI_UpdateAll, . - BattleAI_UpdateAll
