/* Queue the 44-byte BDataMon record selected by an enemy load request. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleEnemyData_LoadStatRecord, "ax", %progbits
.balign 4

.equ BATTLE_CONTEXT_FILE_STATE, 0x140
.equ BATTLE_CONTEXT_READ_RANGE, 0x144
.equ BATTLE_CONTEXT_PATH, 0x150
.equ BATTLE_CONTEXT_RECORD_SIZE, 0x154
.equ BATTLE_CONTEXT_LOAD_FLAG, 0x164
.equ BATTLE_CONTEXT_READ_RANGE_OFFSET, 0xE168
.equ BATTLE_ENEMY_STAT_RECORD_SIZE, 0x2C

.extern BattleEnemyData_LoadObjectData
.extern func_0202c3d0
.extern gBattleContext
.extern gBattleFileArchive
.extern gBattlePrimaryResourcePaths

.global BattleEnemyData_LoadStatRecord
.type BattleEnemyData_LoadStatRecord, %function
BattleEnemyData_LoadStatRecord:
    push {r4, r5, r6, lr}
    sub sp, sp, #8
    mov r4, r0
    ldr lr, .L_battle_context
    ldr r1, [r4, #8]
    ldr r0, [lr]
    add r2, r1, #12
    str r2, [r0, #0x14c]
    ldr r2, [lr]
    mov r0, #BATTLE_ENEMY_STAT_RECORD_SIZE
    str r0, [r2, #BATTLE_CONTEXT_RECORD_SIZE]
    ldr r3, [lr]
    mov r2, #0
    add r3, r3, #0x100
    strh r2, [r3, #0x64]

    ldr r3, .L_primary_resource_paths
    ldr r5, [lr]
    ldr r6, [r3]
    ldr r3, .L_read_range_offset
    str r6, [r5, #BATTLE_CONTEXT_PATH]
    ldr r5, [lr]
    ldr r12, .L_file_archive
    add r3, r5, r3
    str r3, [r5, #BATTLE_CONTEXT_READ_RANGE]

    ldrh r5, [r1, #10]
    ldr r3, [lr]
    mul r6, r5, r0
    ldr r3, [r3, #BATTLE_CONTEXT_READ_RANGE]
    str r6, [r3]
    ldrh r3, [r1, #10]
    ldr r1, [lr]
    add r3, r3, #1
    mul r5, r3, r0
    ldr r0, [r1, #BATTLE_CONTEXT_READ_RANGE]
    str r5, [r0, #4]

    ldr r3, [lr]
    ldr r0, [r3, #BATTLE_CONTEXT_PATH]
    add r1, r3, #BATTLE_CONTEXT_FILE_STATE
    str r0, [sp]
    str r2, [sp, #4]
    ldr r0, [r12]
    ldr r3, [r3, #BATTLE_CONTEXT_READ_RANGE]
    bl func_0202c3d0

    ldr r0, .L_next_callback
    str r0, [r4]
    add sp, sp, #8
    pop {r4, r5, r6, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.L_primary_resource_paths:
    .word gBattlePrimaryResourcePaths
.L_read_range_offset:
    .word BATTLE_CONTEXT_READ_RANGE_OFFSET
.L_file_archive:
    .word gBattleFileArchive
.L_next_callback:
    .word BattleEnemyData_LoadObjectData
.size BattleEnemyData_LoadStatRecord, . - BattleEnemyData_LoadStatRecord
