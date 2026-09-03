/* Spawn a sprite effect and bind it to an owner pointer slot. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSpriteEffect_SpawnAttached, "ax", %progbits
.balign 4

.extern BattleSpriteEffect_Spawn

.global BattleSpriteEffect_SpawnAttached
.type BattleSpriteEffect_SpawnAttached, %function
BattleSpriteEffect_SpawnAttached:
    push {r4, lr}
    sub sp, sp, #8
    mov r4, r0
    mov r0, r1
    mov r1, r2
    mov r2, r3
    ldr ip, [sp, #0x14]
    ldrsh r3, [sp, #0x10]
    str ip, [sp]
    bl BattleSpriteEffect_Spawn
    str r4, [r0, #0x2C]
    str r0, [r4]
    add sp, sp, #8
    pop {r4, lr}
    bx lr
.size BattleSpriteEffect_SpawnAttached, . - BattleSpriteEffect_SpawnAttached
