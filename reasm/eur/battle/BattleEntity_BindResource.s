/* Bind a loaded party/enemy resource record to its live battle entity. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleEntity_BindResource, "ax", %progbits
.balign 4

.equ BATTLE_ENTITY_ID_LIMIT, 0x46
.equ BATTLE_RESOURCE_ID_LIMIT, 0x50
.equ BATTLE_ENEMY_RESOURCE_BASE, 0x48
.equ BATTLE_ENEMY_ACTOR_BASE, 0x3C
.equ BATTLE_ENEMY_ACTOR_COUNT, 8
.equ BATTLE_PARTY_RESOURCE_TABLE_OFFSET, 0xD3C8
.equ BATTLE_ENEMY_RESOURCE_TABLE_OFFSET, 0x155A8
.equ BATTLE_ENEMY_RESOURCE_STRIDE, 0x200C

.equ BATTLE_ACTOR_MAX_HP, 0x04
.equ BATTLE_ACTOR_CURRENT_HP, 0x06
.equ BATTLE_ACTOR_SPEED, 0x08
.equ BATTLE_ACTOR_POWER, 0x0A
.equ BATTLE_ACTOR_DEFENSE, 0x0C
.equ BATTLE_ACTOR_BASE_SPEED, 0x10
.equ BATTLE_ACTOR_BASE_POWER, 0x12
.equ BATTLE_ACTOR_BASE_DEFENSE, 0x14
.equ BATTLE_ACTOR_FLAGS, 0x24
.equ BATTLE_ACTOR_RESOURCE_SLOT, 0x6C
.equ BATTLE_ACTOR_STATE_FLAGS, 0x29C

.extern BattleSceneObject_GetById
.extern OS_Terminate
.extern gBattleContext

.global BattleEntity_BindResource
.type BattleEntity_BindResource, %function
BattleEntity_BindResource:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r5, r0
    mov r4, r1
    cmp r5, #BATTLE_ENTITY_ID_LIMIT
    blo .L_entity_id_valid
    bl OS_Terminate
.L_entity_id_valid:
    cmp r4, #BATTLE_RESOURCE_ID_LIMIT
    blo .L_resource_id_valid
    bl OS_Terminate
.L_resource_id_valid:
    cmp r4, #BATTLE_ENEMY_RESOURCE_BASE
    bhs .L_bind_enemy

    mov r0, r5
    bl BattleSceneObject_GetById
    ldr r2, .L_battle_context
    ldr r1, .L_party_resource_table_offset
    ldr r3, [r2]
    mov r2, #0x30
    add r1, r3, r1
    mla r2, r4, r2, r1
    str r2, [r0, #0xBC]
    mov r1, #0
    strh r1, [r0, #0xE8]
    ldr r3, [r0, #0xF4]
    bic r3, r3, #0x07000000
    str r3, [r0, #0xF4]
    str r1, [r0, #0xC8]
    ldr r3, [r2, #0x18]
    cmp r3, #0
    addeq sp, sp, #4
    popeq {r4, r5, lr}
    bxeq lr

    ldrsb r3, [r3, #9]
    add sp, sp, #4
    strb r3, [r0, #0xFC]
    ldr r3, [r2, #0x18]
    ldrsb r3, [r3, #10]
    strb r3, [r0, #0xFD]
    ldr r3, [r2, #0x18]
    ldrsb r3, [r3, #11]
    strh r3, [r0, #0xF8]
    ldr r3, [r2, #0x18]
    ldrsb r3, [r3, #12]
    strb r3, [r0, #0xFE]
    ldr r3, [r2, #0x18]
    ldrsb r3, [r3, #13]
    strb r3, [r0, #0xFF]
    ldr r3, [r2, #0x18]
    ldrsb r3, [r3, #14]
    strh r3, [r0, #0xFA]
    ldr r3, [r2, #0x18]
    ldrsb r3, [r3, #15]
    strb r3, [r0, #0x100]
    ldr r2, [r2, #0x18]
    ldrsb r2, [r2, #16]
    strb r2, [r0, #0x101]
    strb r1, [r0, #0x102]
    strb r1, [r0, #0x103]
    pop {r4, r5, lr}
    bx lr

.L_bind_enemy:
    sub r0, r5, #BATTLE_ENEMY_ACTOR_BASE
    mov r0, r0, lsl #16
    mov r5, r0, lsr #16
    cmp r5, #BATTLE_ENEMY_ACTOR_COUNT
    blo .L_enemy_id_valid
    bl OS_Terminate
.L_enemy_id_valid:
    ldr r1, .L_battle_context
    ldr r0, .L_enemy_resource_table_offset
    ldr r3, [r1]
    ldr r1, .L_enemy_resource_stride
    add r2, r3, r0
    sub r0, r4, #BATTLE_ENEMY_RESOURCE_BASE
    mla r1, r0, r1, r2
    add r0, r3, r5, lsl #2
    add r0, r0, #0x6000
    ldr r0, [r0, #0x508]
    mov r3, #0
    str r1, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]

    ldr r2, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    add r1, r0, #0x200
    ldr r4, [r2]
    mov r2, #1
    ldrh r4, [r4, #6]
    strh r4, [r0, #BATTLE_ACTOR_MAX_HP]
    ldrsh r4, [r0, #BATTLE_ACTOR_MAX_HP]
    strh r4, [r0, #BATTLE_ACTOR_CURRENT_HP]

    ldr r4, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r4, [r4]
    ldrh r4, [r4, #12]
    strh r4, [r0, #BATTLE_ACTOR_BASE_SPEED]
    ldrsh r4, [r0, #BATTLE_ACTOR_BASE_SPEED]
    strh r4, [r0, #BATTLE_ACTOR_SPEED]

    ldr r4, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r4, [r4]
    ldrh r4, [r4, #8]
    strh r4, [r0, #BATTLE_ACTOR_BASE_POWER]
    ldrsh r4, [r0, #BATTLE_ACTOR_BASE_POWER]
    strh r4, [r0, #BATTLE_ACTOR_POWER]

    ldr r4, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r4, [r4]
    ldrh r4, [r4, #10]
    strh r4, [r0, #BATTLE_ACTOR_BASE_DEFENSE]
    ldrsh r4, [r0, #BATTLE_ACTOR_BASE_DEFENSE]
    strh r4, [r0, #BATTLE_ACTOR_DEFENSE]

    strh r3, [r0, #0x16]
    ldrsh r3, [r0, #0x16]
    strh r3, [r0, #0x0E]

    ldr r3, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldrh r4, [r0, #BATTLE_ACTOR_FLAGS]
    ldr r3, [r3]
    ldrb r3, [r3, #5]
    bic r4, r4, #0x7F
    and r3, r3, #0x7F
    orr r3, r4, r3
    strh r3, [r0, #BATTLE_ACTOR_FLAGS]

    ldr r3, [r0, #BATTLE_ACTOR_RESOURCE_SLOT]
    ldr r4, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    ldr r3, [r3]
    bic r4, r4, #0xC0
    ldrh r3, [r3, #0x0E]
    mov r3, r3, lsl #30
    mov r3, r3, lsr #30
    and r3, r3, #3
    orr r3, r4, r3, lsl #6
    str r3, [r0, #BATTLE_ACTOR_STATE_FLAGS]

    ldrh r3, [r0, #BATTLE_ACTOR_FLAGS]
    orr r3, r3, #0x800
    strh r3, [r0, #BATTLE_ACTOR_FLAGS]
    strh r2, [r1, #0x9A]

    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x80
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x100
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x200
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x400
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x1000
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x2000
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]
    ldrh r1, [r0, #BATTLE_ACTOR_FLAGS]
    bic r1, r1, #0x4000
    strh r1, [r0, #BATTLE_ACTOR_FLAGS]

    ldr r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    bic r1, r1, #1
    str r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    ldr r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    bic r1, r1, #2
    str r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    ldr r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    bic r1, r1, #4
    str r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    ldr r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    bic r1, r1, #8
    str r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    ldr r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    bic r1, r1, #0x10
    str r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    ldr r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]
    bic r1, r1, #0x20
    str r1, [r0, #BATTLE_ACTOR_STATE_FLAGS]

    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr

.L_battle_context:
    .word gBattleContext
.L_party_resource_table_offset:
    .word BATTLE_PARTY_RESOURCE_TABLE_OFFSET
.L_enemy_resource_table_offset:
    .word BATTLE_ENEMY_RESOURCE_TABLE_OFFSET
.L_enemy_resource_stride:
    .word BATTLE_ENEMY_RESOURCE_STRIDE
.size BattleEntity_BindResource, . - BattleEntity_BindResource
