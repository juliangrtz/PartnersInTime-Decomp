/* Resolve and queue the variable-sized object data following an enemy stat record. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleEnemyData_LoadObjectData, "ax", %progbits
.balign 4

.equ BATTLE_ENEMY_REQUEST_DATA, 0x08
.equ BATTLE_ENEMY_OBJECT_ID, 0x0E
.equ BATTLE_ENEMY_PAYLOAD_OFFSET, 0x0C
.equ BATTLE_ENEMY_STAT_RECORD_SIZE, 0x2C
.equ BATTLE_CONTEXT_LOADED_OBJECT, 0x544

.extern BattleEnemyData_FixupObjectPointers
.extern func_0202c3d0
.extern BattleObjectData_ResolveSlot
.extern gBattleContext
.extern gBattleSystem

.global BattleEnemyData_LoadObjectData
.type BattleEnemyData_LoadObjectData, %function
BattleEnemyData_LoadObjectData:
    push {r4, r5, lr}
    sub sp, sp, #12
    mov r4, r0
    ldr r5, [r4, #BATTLE_ENEMY_REQUEST_DATA]
    ldrh r0, [r5, #BATTLE_ENEMY_OBJECT_ID]
    bl BattleObjectData_ResolveSlot

    ldr r1, .L_battle_context
    add r2, r5, #BATTLE_ENEMY_PAYLOAD_OFFSET
    ldr r12, [r1]
    ldr r3, .L_object_path_offset
    str r0, [r12, #BATTLE_CONTEXT_LOADED_OBJECT]
    ldr r0, [r1]
    add r12, r2, #BATTLE_ENEMY_STAT_RECORD_SIZE
    ldr r0, [r0, #BATTLE_CONTEXT_LOADED_OBJECT]
    mov r2, #0
    str r3, [r0, #0x14]
    ldr r3, [r1]
    ldr r0, .L_battle_system
    ldr r3, [r3, #BATTLE_CONTEXT_LOADED_OBJECT]
    str r12, [r3, #0x0C]
    ldr r1, [r1]
    ldr r1, [r1, #BATTLE_CONTEXT_LOADED_OBJECT]
    ldr r3, [r1, #0x10]
    str r3, [sp]
    str r2, [sp, #4]
    ldr r0, [r0]
    ldr r3, [r1, #4]
    bl func_0202c3d0

    ldr r0, .L_next_callback
    str r0, [r4]
    add sp, sp, #12
    pop {r4, r5, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.L_object_path_offset:
    .word 0x00001fd4
.L_battle_system:
    .word gBattleSystem
.L_next_callback:
    .word BattleEnemyData_FixupObjectPointers
.size BattleEnemyData_LoadObjectData, . - BattleEnemyData_LoadObjectData
