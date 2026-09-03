/* Spawn a model effect and bind it to an owner pointer slot. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleModelEffect_SpawnAttached, "ax", %progbits
.balign 4

.extern BattleModelEffect_Spawn

.global BattleModelEffect_SpawnAttached
.type BattleModelEffect_SpawnAttached, %function
BattleModelEffect_SpawnAttached:
    push {r4, lr}
    sub sp, sp, #8
    ldrsh lr, [sp, #0x14]
    mov r4, r0
    mov r0, r1
    mov r1, r2
    mov r2, r3
    ldrsh r3, [sp, #0x10]
    ldr ip, [sp, #0x18]
    str lr, [sp]
    str ip, [sp, #4]
    bl BattleModelEffect_Spawn
    str r4, [r0, #0x34]
    str r0, [r4]
    add sp, sp, #8
    pop {r4, lr}
    bx lr
.size BattleModelEffect_SpawnAttached, . - BattleModelEffect_SpawnAttached
