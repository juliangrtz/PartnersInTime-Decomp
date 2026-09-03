/* Test whether an ordinary or large enemy object-data slot is still pending. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleObjectData_IsLoadPending, "ax", %progbits
.balign 4

.extern OS_Terminate
.extern gBattleContext

.global BattleObjectData_IsLoadPending
.type BattleObjectData_IsLoadPending, %function
BattleObjectData_IsLoadPending:
    push {r4, lr}
    mov r4, r0
    cmp r4, #0x50
    blo .L_valid_slot
    bl OS_Terminate
.L_valid_slot:
    cmp r4, #0x48
    bhs .L_enemy_slot
    ldr r1, .L_battle_context
    mov r0, #0x30
    ldr r1, [r1]
    mla r0, r4, r0, r1
    add r0, r0, #0xd000
    ldr r0, [r0, #0x3f4]
    lsl r0, r0, #1
    lsrs r0, r0, #0x1f
    bne .L_pending
    mov r0, #0
    pop {r4, lr}
    bx lr
.L_enemy_slot:
    ldr r1, .L_battle_context
    ldr r0, .L_enemy_load_stride
    ldr r2, [r1]
    sub r1, r4, #0x48
    mla r0, r1, r0, r2
    add r0, r0, #0x15000
    ldr r0, [r0, #0x5a8]
    cmp r0, #0
    movne r0, #0
    popne {r4, lr}
    bxne lr
.L_pending:
    mov r0, #1
    pop {r4, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_enemy_load_stride:
    .word 0x200C
.size BattleObjectData_IsLoadPending, . - BattleObjectData_IsLoadPending
