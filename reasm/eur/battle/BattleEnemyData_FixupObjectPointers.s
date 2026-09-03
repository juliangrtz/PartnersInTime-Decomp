/* Convert offsets in a freshly loaded enemy object record to RAM pointers. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleEnemyData_FixupObjectPointers, "ax", %progbits
.balign 4

.equ BATTLE_ENEMY_REQUEST_DATA, 0x08
.equ BATTLE_ENEMY_STAT_RECORD_SIZE, 0x2C
.equ BATTLE_ENEMY_PAYLOAD_OFFSET, 0x0C

.global BattleEnemyData_FixupObjectPointers
.type BattleEnemyData_FixupObjectPointers, %function
BattleEnemyData_FixupObjectPointers:
    ldr r12, [r0, #BATTLE_ENEMY_REQUEST_DATA]
    mov r1, #0
    add r2, r12, #BATTLE_ENEMY_PAYLOAD_OFFSET
    str r2, [r12]
    add r2, r2, #BATTLE_ENEMY_STAT_RECORD_SIZE
    str r2, [r12, #4]
    ldr r3, [r12, #4]
    ldrh r2, [r3]
    add r2, r3, r2
    str r2, [r12, #4]
    str r1, [r0]
    bx lr
.size BattleEnemyData_FixupObjectPointers, . - BattleEnemyData_FixupObjectPointers
