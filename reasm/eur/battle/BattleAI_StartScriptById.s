/* Route a party slot or typed battle-AI ID to its script starter. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartScriptById, "ax", %progbits
.balign 4

.extern BattleAI_StartActionScript
.extern BattleAI_StartPartyVmSlot1
.extern BattleAI_StartPartyVmSlot2
.extern BattleAI_StartPartyVmSlot3
.extern BattleAI_StartPartyVmSlot4
.extern BattleAI_StartReactionScript

.global BattleAI_StartScriptById
.type BattleAI_StartScriptById, %function
BattleAI_StartScriptById:
    stmdb sp!, {lr}
    sub sp, sp, #4
    cmp r0, #4
    addls pc, pc, r0, lsl #2
    b .L_typed_id
    b .L_typed_id
    b .L_party_1
    b .L_party_2
    b .L_party_3
    b .L_party_4
.L_party_1:
    bl BattleAI_StartPartyVmSlot1
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_party_2:
    bl BattleAI_StartPartyVmSlot2
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_party_3:
    bl BattleAI_StartPartyVmSlot3
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_party_4:
    bl BattleAI_StartPartyVmSlot4
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_typed_id:
    and r1, r0, #0xf000
    cmp r1, #0x1000
    beq .L_action
    cmp r1, #0x2000
    beq .L_reaction
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_action:
    ldr r1, .L_id_mask
    and r0, r0, r1
    bl BattleAI_StartActionScript
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_reaction:
    ldr r1, .L_id_mask
    and r0, r0, r1
    bl BattleAI_StartReactionScript
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_id_mask:
    .word 0x0fff
.size BattleAI_StartScriptById, . - BattleAI_StartScriptById
