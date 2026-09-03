/* Route one queued hit record to the enemy or party damage implementation. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleDamage_DispatchHit, "ax", %progbits
.balign 4

.equ HIT_RECORD_ACTOR_ID, 0x02
.equ HIT_RECORD_X, 0x04
.equ HIT_RECORD_Y, 0x06
.equ HIT_RECORD_Z, 0x08
.equ HIT_RECORD_KIND, 0x0A
.equ SCENE_OBJECT_X, 0x04
.equ SCENE_OBJECT_Y, 0x06
.equ SCENE_OBJECT_Z, 0x08
.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ BATTLE_ACTOR_PENDING_DAMAGE, 0x1E
.equ PARTY_ACTOR_ID_FIRST, 0x38
.equ PARTY_ACTOR_ID_END, 0x3C
.equ ENEMY_ACTOR_ID_END, 0x44
.equ BATTLE_CONTEXT_ENEMY_HIT_MODE, 0xCB78

.extern BattleActor_GetById
.extern BattleDamage_ApplyToEnemy
.extern BattleDamage_ApplyToParty
.extern BattleSceneObject_GetById
.extern gBattleContext

.global BattleDamage_DispatchHit
.type BattleDamage_DispatchHit, %function
BattleDamage_DispatchHit:
    push {r4, r5, r6, r7, r8, sb, lr}
    sub sp, sp, #0x0C
    mov sb, r0
    ldrh r8, [sb, #HIT_RECORD_ACTOR_ID]
    cmp r8, #PARTY_ACTOR_ID_FIRST
    blo .L_not_party
    cmp r8, #PARTY_ACTOR_ID_END
    movlo r4, #0
    blo .L_actor_kind_ready

.L_not_party:
    cmp r8, #PARTY_ACTOR_ID_END
    addlo sp, sp, #0x0C
    poplo {r4, r5, r6, r7, r8, sb, lr}
    bxlo lr
    cmp r8, #ENEMY_ACTOR_ID_END
    movlo r4, #1
    addhs sp, sp, #0x0C
    pophs {r4, r5, r6, r7, r8, sb, lr}
    bxhs lr

.L_actor_kind_ready:
    mov r0, r8
    bl BattleSceneObject_GetById
    mov r7, r0
    mov r0, r8
    bl BattleActor_GetById
    ldrsh r5, [sb, #HIT_RECORD_Y]
    ldrsh r3, [r7, #SCENE_OBJECT_Y]
    ldrsh r2, [sb, #HIT_RECORD_Z]
    ldrsh r1, [r7, #SCENE_OBJECT_Z]
    ldrsh ip, [sb, #HIT_RECORD_X]
    ldrsh r6, [r7, #SCENE_OBJECT_X]
    sub r3, r5, r3
    sub r1, r2, r1
    cmp r4, #0
    sub r5, ip, r6
    sub r4, r3, r1
    ldrsh r6, [r0, #BATTLE_ACTOR_PENDING_DAMAGE]
    beq .L_party_hit

    ldr r0, .L_battle_context
    ldr r0, [r0]
    add r0, r0, #0xCB00
    ldrsh r0, [r0, #0x78]
    cmp r0, #0
    beq .L_enemy_mode_valid
    cmp r0, #1
    addne sp, sp, #0x0C
    popne {r4, r5, r6, r7, r8, sb, lr}
    bxne lr
.L_enemy_mode_valid:
    cmp r0, #0
    moveq r0, #0x11
    movne r0, #0x13
    str r0, [sp]
    mov r8, #7
    mov r2, r4
    mov r0, r7
    mov r1, r5
    mov r3, r6
    str r8, [sp, #4]
    mov r4, #0
    str r4, [sp, #8]
    bl BattleDamage_ApplyToEnemy
    add sp, sp, #0x0C
    pop {r4, r5, r6, r7, r8, sb, lr}
    bx lr

.L_party_hit:
    mov r0, r8
    bl BattleActor_GetById
    ldrsh r0, [r0, #BATTLE_ACTOR_CURRENT_HP]
    cmp r0, #0
    addle sp, sp, #0x0C
    pople {r4, r5, r6, r7, r8, sb, lr}
    bxle lr
    ldrsh r0, [sb, #HIT_RECORD_KIND]
    mov r1, r5
    mov r2, r4
    cmp r0, #2
    moveq r0, #1
    movne r0, #0
    str r0, [sp]
    mov r0, r7
    mov r3, r6
    str sb, [sp, #4]
    bl BattleDamage_ApplyToParty
    add sp, sp, #0x0C
    pop {r4, r5, r6, r7, r8, sb, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.size BattleDamage_DispatchHit, . - BattleDamage_DispatchHit
