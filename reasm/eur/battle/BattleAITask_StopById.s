/* Stop one sorted script task and clear the attached VM state's script. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAITask_StopById, "ax", %progbits
.balign 4

.extern BattleTask_Release

.global BattleAITask_StopById
.type BattleAITask_StopById, %function
BattleAITask_StopById:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, [r0]
    cmp r0, #0
    addeq sp, sp, #4
    ldmiaeq sp!, {lr}
    bxeq lr
.L_check_head:
    ldrh r2, [r0, #0x10]
    cmp r2, r1
    addgt sp, sp, #4
    ldmiagt sp!, {lr}
    bxgt lr
    cmp r2, r1
    bne .L_scan_tail
    ldr r2, [r0, #0xc]
    cmp r2, #0
    movne r1, #0
    strne r1, [r2]
    bl BattleTask_Release
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_scan_tail:
    ldr r3, [r0]
    cmp r3, #0
    addeq sp, sp, #4
    ldmiaeq sp!, {lr}
    bxeq lr
.L_check_tail:
    ldrh r2, [r3, #0x10]
    cmp r2, r1
    addgt sp, sp, #4
    ldmiagt sp!, {lr}
    bxgt lr
    cmp r2, r1
    bne .L_next
    ldr r2, [r3, #0xc]
    cmp r2, #0
    movne r1, #0
    strne r1, [r2]
    ldr r0, [r0]
    bl BattleTask_Release
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_next:
    mov r0, r3
    ldr r3, [r3]
    cmp r3, #0
    bne .L_check_tail
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size BattleAITask_StopById, . - BattleAITask_StopById
