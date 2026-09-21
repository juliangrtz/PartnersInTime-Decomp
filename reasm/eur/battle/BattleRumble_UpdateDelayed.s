/* Count down and dispatch one deferred primary or secondary screen effect. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleRumble_UpdateDelayed, "ax", %progbits
.balign 4

.extern BattleRumble_PlayTimed
.extern BattleRumble_PlayRepeated

.global BattleRumble_UpdateDelayed
.type BattleRumble_UpdateDelayed, %function
BattleRumble_UpdateDelayed:
    push {r4, lr}
    mov r4, r0
    ldr r0, [r4]
    cmp r0, #0
    subgt r0, r0, #1
    strgt r0, [r4]
    popgt {r4, lr}
    bxgt lr
    ldrb r0, [r4, #6]
    cmp r0, #0
    beq .L_primary
    cmp r0, #1
    beq .L_secondary
    b .L_release
.L_primary:
    ldrb r0, [r4, #7]
    ldrsh r1, [r4, #4]
    mov r2, #0
    add r0, r0, #1
    bl BattleRumble_PlayTimed
    b .L_release
.L_secondary:
    ldrb r0, [r4, #7]
    ldrsh r1, [r4, #4]
    mov r2, #0
    add r0, r0, #1
    bl BattleRumble_PlayRepeated
.L_release:
    mov r0, #0
    str r0, [r4, #8]
    pop {r4, lr}
    bx lr
.size BattleRumble_UpdateDelayed, . - BattleRumble_UpdateDelayed
