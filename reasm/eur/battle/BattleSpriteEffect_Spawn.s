/* Construct a sprite effect at view-adjusted battle coordinates. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSpriteEffect_Spawn, "ax", %progbits
.balign 4

.extern func_02017d38
.extern gBattleContext

.global BattleSpriteEffect_Spawn
.type BattleSpriteEffect_Spawn, %function
BattleSpriteEffect_Spawn:
    push {r4, r5, lr}
    sub sp, sp, #0x0C
    ldr ip, .L_battle_context
    ldr lr, [sp, #0x18]
    ldr r5, [ip]
    ldr r4, [r5, #0x44]
    add ip, r5, #0xCB00
    str r3, [sp]
    str lr, [sp, #4]
    ldrsh lr, [ip, #0x9C]
    add r3, r5, #0xC000
    mov r5, r2
    mov r2, r1
    ldr r1, [r4, r0, lsl #2]
    ldrsh ip, [ip, #0x9E]
    ldr r0, [r3, #0xBF0]
    add r1, r4, r1
    add r2, r2, lr
    add r3, r5, ip
    bl func_02017d38
    add sp, sp, #0x0C
    pop {r4, r5, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.size BattleSpriteEffect_Spawn, . - BattleSpriteEffect_Spawn
