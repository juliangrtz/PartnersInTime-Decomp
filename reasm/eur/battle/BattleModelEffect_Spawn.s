/* Construct a model effect from the battle resource table. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleModelEffect_Spawn, "ax", %progbits
.balign 4

.extern func_02018af4
.extern func_ov002_020aaac8
.extern gBattleContext

.global BattleModelEffect_Spawn
.type BattleModelEffect_Spawn, %function
BattleModelEffect_Spawn:
    push {r4, r5, r6, lr}
    ldr r4, .L_battle_context
    mov r5, r2
    ldr r2, [r4]
    mov r4, r3
    ldr ip, [r2, #0x48]
    add r2, r2, #0xC000
    ldr r3, [ip, r0, lsl #2]
    ldr r0, [r2, #0xBF4]
    mov r6, r1
    add r1, ip, r3
    ldr r2, .L_update_callback
    mov r3, #0x1000
    bl func_02018af4
    strh r5, [r0, #0x14]
    ldr r1, [sp, #0x14]
    ldrsh r2, [sp, #0x10]
    strh r4, [r0, #0x16]
    mov r1, r1, lsl #4
    strh r2, [r0, #0x18]
    str r6, [r0, #0x2C]
    strh r1, [r0, #0x12]
    pop {r4, r5, r6, lr}
    bx lr
.L_battle_context:
    .word gBattleContext
.L_update_callback:
    .word func_ov002_020aaac8
.size BattleModelEffect_Spawn, . - BattleModelEffect_Spawn
