/* Clear an AI state's order-wait flag once no earlier live task blocks it. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_TryClearOrderWait, "ax", %progbits
.balign 4

.extern gBattleAIActionTaskPool
.extern gBattleAIReactionTaskPool

.global BattleAI_TryClearOrderWait
.type BattleAI_TryClearOrderWait, %function
BattleAI_TryClearOrderWait:
    push {r4, r5, r6, lr}
    ldr r1, .L_action_pool
    ldr r5, [r1]
    cmp r5, #0
    beq .L_scan_reactions
    ldrh lr, [r0, #0xb0]
    mov r3, #0
    mov ip, #1
.L_action_loop:
    ldr r4, [r5, #0xc]
    ldrh r2, [r4, #0xb0]
    cmp r2, lr
    beq .L_next_action
    ldr r1, [r4]
    cmp r1, #0
    beq .L_next_action
    ldrh r1, [r4, #0xb2]
    lsl r1, r1, #0x1f
    lsrs r1, r1, #0x1f
    bne .L_next_action
    ldrsh r1, [r0, #0xb4]
    cmp r2, lr
    movlo r6, ip
    movhs r6, r3
    ldrsh r2, [r4, #0xb4]
    sub r1, r1, r6
    cmp r2, r1
    movgt r0, #0
    popgt {r4, r5, r6, lr}
    bxgt lr
.L_next_action:
    ldr r5, [r5]
    cmp r5, #0
    bne .L_action_loop
.L_scan_reactions:
    ldr r1, .L_reaction_pool
    ldr lr, [r1]
    cmp lr, #0
    beq .L_ready
    ldrh r4, [r0, #0xb0]
    mov r3, #0
    mov ip, #1
.L_reaction_loop:
    ldr r5, [lr, #0xc]
    ldrh r2, [r5, #0xb0]
    cmp r2, r4
    beq .L_next_reaction
    ldr r1, [r5]
    cmp r1, #0
    beq .L_next_reaction
    ldrh r1, [r5, #0xb2]
    lsl r1, r1, #0x1f
    lsrs r1, r1, #0x1f
    bne .L_next_reaction
    ldrsh r1, [r0, #0xb4]
    cmp r2, r4
    movlo r6, ip
    movhs r6, r3
    ldrsh r2, [r5, #0xb4]
    sub r1, r1, r6
    cmp r2, r1
    movgt r0, #0
    popgt {r4, r5, r6, lr}
    bxgt lr
.L_next_reaction:
    ldr lr, [lr]
    cmp lr, #0
    bne .L_reaction_loop
.L_ready:
    ldrh r1, [r0, #0xb2]
    bic r1, r1, #2
    strh r1, [r0, #0xb2]
    mov r0, #1
    pop {r4, r5, r6, lr}
    bx lr
.L_action_pool:
    .word gBattleAIActionTaskPool
.L_reaction_pool:
    .word gBattleAIReactionTaskPool
.size BattleAI_TryClearOrderWait, . - BattleAI_TryClearOrderWait
