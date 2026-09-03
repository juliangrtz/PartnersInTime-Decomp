/* Find an enemy's sorted AI task or insert a new task at the proper position. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAITask_GetOrInsert, "ax", %progbits
.balign 4

.extern BattleTaskList_Insert
.extern BattleTaskPool_Allocate

.global BattleAITask_GetOrInsert
.type BattleAITask_GetOrInsert, %function
BattleAITask_GetOrInsert:
    push {r4, lr}
    ldr r4, [r0]
    cmp r4, #0
    beq .L_insert_head
    ldrh r2, [r4, #0x10]
    cmp r2, r1
    ble .L_compare_head
    mov r1, #0
    bl BattleTaskList_Insert
    pop {r4, lr}
    bx lr
.L_compare_head:
    cmp r2, r1
    moveq r0, r4
    popeq {r4, lr}
    bxeq lr
.L_scan:
    ldr r3, [r4]
    cmp r3, #0
    beq .L_append
.L_compare_scanned:
    ldrh r2, [r3, #0x10]
    cmp r2, r1
    ble .L_compare_next
    bl BattleTaskPool_Allocate
    ldr r1, [r4]
    str r1, [r0]
    str r0, [r4]
    ldr r0, [r4]
    pop {r4, lr}
    bx lr
.L_compare_next:
    cmp r2, r1
    moveq r0, r3
    popeq {r4, lr}
    bxeq lr
    mov r4, r3
    ldr r3, [r3]
    cmp r3, #0
    bne .L_compare_scanned
.L_append:
    bl BattleTaskPool_Allocate
    mov r1, #0
    str r1, [r0]
    str r0, [r4]
    ldr r0, [r4]
    pop {r4, lr}
    bx lr
.L_insert_head:
    mov r1, #0
    bl BattleTaskList_Insert
    pop {r4, lr}
    bx lr
.size BattleAITask_GetOrInsert, . - BattleAITask_GetOrInsert
