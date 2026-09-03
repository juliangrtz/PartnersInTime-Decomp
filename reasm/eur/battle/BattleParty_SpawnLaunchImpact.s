/* Spawn the form-specific impact effect used by the party launch reaction. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleParty_SpawnLaunchImpact, "ax", %progbits
.balign 4

.extern BattlePosition_StoreViewRelative
.extern BattleSound_Play
.extern func_ov002_020ac904

.global BattleParty_SpawnLaunchImpact
.type BattleParty_SpawnLaunchImpact, %function
BattleParty_SpawnLaunchImpact:
    push {r4, r5, lr}
    sub sp, sp, #0x14
    ldrh r2, [r0, #0x7E]
    ldr r1, .L_form_bias
    ldr r5, [r0]
    add r1, r2, r1
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    cmp r1, #1
    ldr r1, [r5, #0xF4]
    ldrsh lr, [r5, #6]
    mov r1, r1, lsl #14
    mov r1, r1, lsr #31
    str r1, [sp]
    ldr r1, [r5, #0xF4]
    mov r4, #0x10
    mov r1, r1, lsl #17
    mov r1, r1, lsr #31
    str r1, [sp, #4]
    ldrsh ip, [r5, #8]
    ldrsh r3, [r5, #0xEA]
    rsb r2, lr, #0x100
    ldrsh r1, [r5, #4]
    add r3, r3, r2, lsl #4
    sub ip, lr, ip
    mov r2, ip, lsl #16
    mov r3, r3, lsl #16
    add r0, sp, #8
    mov r2, r2, asr #16
    mov r3, r3, asr #16
    movls r4, #0x11
    bl BattlePosition_StoreViewRelative
    ldrsh r2, [sp, #0x0C]
    mov r0, #0x100
    mov r1, r4
    str r2, [sp]
    str r0, [sp, #4]
    ldrsh r2, [sp, #8]
    ldrsh r3, [sp, #0x0A]
    mov r0, #0x13
    bl func_ov002_020ac904
    mov r1, #0
    mov r2, r1
    mov r3, r1
    mov r0, #0x39
    bl BattleSound_Play
    add sp, sp, #0x14
    pop {r4, r5, lr}
    bx lr
.L_form_bias:
    .word 0x0000FFFE
.size BattleParty_SpawnLaunchImpact, . - BattleParty_SpawnLaunchImpact
