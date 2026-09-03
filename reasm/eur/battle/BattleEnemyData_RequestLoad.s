/* Initialize an enemy-data request and queue its stat-record load stage. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleEnemyData_RequestLoad, "ax", %progbits
.balign 4

.equ BATTLE_ENEMY_REQUEST_INDEX, 0x0A

.extern BattleEnemyData_LoadStatRecord
.extern BattleTaskQueue_Enqueue

.global BattleEnemyData_RequestLoad
.type BattleEnemyData_RequestLoad, %function
BattleEnemyData_RequestLoad:
    mov r2, r0
    mov r0, #0
    str r0, [r2]
    strh r1, [r2, #BATTLE_ENEMY_REQUEST_INDEX]
    ldr r12, .L_enqueue
    ldr r0, .L_load_stat_record
    mov r1, r2
    bx r12

.L_enqueue:
    .word BattleTaskQueue_Enqueue
.L_load_stat_record:
    .word BattleEnemyData_LoadStatRecord
.size BattleEnemyData_RequestLoad, . - BattleEnemyData_RequestLoad
