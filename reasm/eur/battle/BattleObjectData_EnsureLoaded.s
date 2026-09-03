/* Request a resource only when its battle object slot does not already own it. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleObjectData_EnsureLoaded, "ax", %progbits
.balign 4

.extern BattleEnemyData_RequestLoad
.extern BattleObjectData_GetLoadState
.extern BattleObjectData_QueueLoad
.extern OS_Terminate
.extern gBattleContext

.global BattleObjectData_EnsureLoaded
.type BattleObjectData_EnsureLoaded, %function
BattleObjectData_EnsureLoaded:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, r1
    cmp r6, #0x50
    blo .L_valid_slot
    bl OS_Terminate
.L_valid_slot:
    cmp r6, #0x48
    bhs .L_enemy_slot
    mov r0, #0x30
    mul r4, r6, r0
    ldr r0, .L_battle_context
    ldr r0, [r0]
    add r0, r0, r4
    add r0, r0, #0xd000
    ldr r0, [r0, #0x3c8]
    cmp r0, #0
    bne .L_compare_resource
    bl OS_Terminate
.L_compare_resource:
    mov r0, r6
    bl BattleObjectData_GetLoadState
    ldr r0, [r0, #0x24]
    cmp r5, r0
    popeq {r4, r5, r6, lr}
    bxeq lr
    ldr r1, .L_battle_context
    ldr r0, .L_load_state_offset
    ldr r2, [r1]
    mov r1, r5
    add r0, r2, r0
    add r0, r0, r4
    bl BattleObjectData_QueueLoad
    strh r6, [r0, #4]
    pop {r4, r5, r6, lr}
    bx lr
.L_enemy_slot:
    ldr r1, .L_battle_context
    ldr r0, .L_enemy_load_offset
    ldr r2, [r1]
    ldr r1, .L_enemy_load_stride
    add r0, r2, r0
    sub r2, r6, #0x48
    mla r0, r2, r1, r0
    mov r1, r5
    bl BattleEnemyData_RequestLoad
    pop {r4, r5, r6, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_load_state_offset:
    .word 0xD3C8
.L_enemy_load_offset:
    .word 0x155A8
.L_enemy_load_stride:
    .word 0x200C
.size BattleObjectData_EnsureLoaded, . - BattleObjectData_EnsureLoaded
