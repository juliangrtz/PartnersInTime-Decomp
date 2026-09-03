/* Clear one active ailment/stat effect and restore its base stat if needed. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleStatus_ClearEffect, "ax", %progbits
.balign 4

.equ BATTLE_ACTOR_SPEED, 0x08
.equ BATTLE_ACTOR_POWER, 0x0A
.equ BATTLE_ACTOR_DEFENSE, 0x0C
.equ BATTLE_ACTOR_BASE_SPEED, 0x10
.equ BATTLE_ACTOR_BASE_POWER, 0x12
.equ BATTLE_ACTOR_BASE_DEFENSE, 0x14
.equ BATTLE_ACTOR_AILMENT_1, 0x30
.equ BATTLE_ACTOR_AILMENT_2, 0x3C
.equ BATTLE_ACTOR_POWER_CHANGE, 0x48
.equ BATTLE_ACTOR_DEFENSE_CHANGE, 0x54
.equ BATTLE_ACTOR_SPEED_CHANGE, 0x60
.equ BATTLE_STATUS_ACTIVE_OFFSET, 0x08

.extern BattleStatus_StopActorEffect

.global BattleStatus_ClearEffect
.type BattleStatus_ClearEffect, %function
BattleStatus_ClearEffect:
    push {r4, r5, r6, lr}
    mov r5, r1
    mov r6, r0
    cmp r5, #8
    addls pc, pc, r5, lsl #2
    b .L_not_active

    b .L_not_active
    b .L_status_1
    b .L_status_2
    b .L_status_3
    b .L_status_4
    b .L_status_5
    b .L_status_6
    b .L_status_7
    b .L_status_8

.L_status_1:
    add r4, r6, #BATTLE_ACTOR_AILMENT_1
    ldrsb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]
    cmp r0, #1
    beq .L_clear
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_status_2:
    add r4, r6, #BATTLE_ACTOR_AILMENT_1
    ldrsb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]
    cmp r0, #2
    beq .L_clear
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_status_3:
    add r4, r6, #BATTLE_ACTOR_AILMENT_1
    ldrsb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]
    cmp r0, #3
    beq .L_clear
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_status_4:
    add r4, r6, #BATTLE_ACTOR_AILMENT_2
    ldrsb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]
    cmp r0, #1
    beq .L_clear
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_status_5:
    add r4, r6, #BATTLE_ACTOR_AILMENT_2
    ldrsb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]
    cmp r0, #2
    beq .L_clear
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_status_6:
    add r4, r6, #BATTLE_ACTOR_POWER_CHANGE
    ldrsb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]
    cmp r0, #0
    bne .L_clear
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_status_7:
    add r4, r6, #BATTLE_ACTOR_DEFENSE_CHANGE
    ldrsb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]
    cmp r0, #0
    bne .L_clear
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_status_8:
    add r4, r6, #BATTLE_ACTOR_SPEED_CHANGE
    ldrsb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]
    cmp r0, #0
    bne .L_clear
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_not_active:
    mov r0, #0
    pop {r4, r5, r6, lr}
    bx lr

.L_clear:
    ldr r0, [r6]
    mov r1, r5
    ldrh r0, [r0, #0xEC]
    bl BattleStatus_StopActorEffect
    mov r0, #0
    strb r0, [r4, #BATTLE_STATUS_ACTIVE_OFFSET]

    cmp r5, #6
    beq .L_restore_power
    cmp r5, #7
    beq .L_restore_defense
    cmp r5, #8
    ldrsheq r0, [r6, #BATTLE_ACTOR_BASE_SPEED]
    strheq r0, [r6, #BATTLE_ACTOR_SPEED]
    b .L_cleared

.L_restore_power:
    ldrsh r0, [r6, #BATTLE_ACTOR_BASE_POWER]
    strh r0, [r6, #BATTLE_ACTOR_POWER]
    b .L_cleared

.L_restore_defense:
    ldrsh r0, [r6, #BATTLE_ACTOR_BASE_DEFENSE]
    strh r0, [r6, #BATTLE_ACTOR_DEFENSE]

.L_cleared:
    mov r0, r5
    pop {r4, r5, r6, lr}
    bx lr
.size BattleStatus_ClearEffect, . - BattleStatus_ClearEffect
