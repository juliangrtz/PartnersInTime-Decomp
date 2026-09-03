/* Apply positive damage to a battle actor and report a new knockout. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleActor_ApplyDamage, "ax", %progbits
.balign 4

.equ BATTLE_HIT_ACTOR_ID, 0xEC
.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ BATTLE_ACTOR_HIT_STATE, 0x1E
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_ACTOR_FLAG_KO, 0x100

.extern BattleActor_GetById

.global BattleActor_ApplyDamage
.type BattleActor_ApplyDamage, %function
BattleActor_ApplyDamage:
    push {r4, lr}
    mov r4, r1
    cmp r4, #1
    movlt r0, #0
    poplt {r4, lr}
    bxlt lr

    ldrh r0, [r0, #BATTLE_HIT_ACTOR_ID]
    bl BattleActor_GetById
    mov r2, #0
    strh r2, [r0, #BATTLE_ACTOR_HIT_STATE]
    ldrsh r1, [r0, #BATTLE_ACTOR_CURRENT_HP]
    cmp r1, #0
    bgt .L_actor_alive

    strh r2, [r0, #BATTLE_ACTOR_CURRENT_HP]
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    orr r1, r1, #BATTLE_ACTOR_FLAG_KO
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    mov r0, r2
    pop {r4, lr}
    bx lr

.L_actor_alive:
    cmp r1, r4
    subgt r1, r1, r4
    strhgt r1, [r0, #BATTLE_ACTOR_CURRENT_HP]
    movgt r0, r2
    popgt {r4, lr}
    bxgt lr

    strh r2, [r0, #BATTLE_ACTOR_CURRENT_HP]
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    orr r1, r1, #BATTLE_ACTOR_FLAG_KO
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    mov r0, #1
    pop {r4, lr}
    bx lr

.size BattleActor_ApplyDamage, . - BattleActor_ApplyDamage
