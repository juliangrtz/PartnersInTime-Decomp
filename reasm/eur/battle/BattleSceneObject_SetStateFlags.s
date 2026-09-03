/* Set the low scene-object state byte and its independent bit-18 flag. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_SetStateFlags, "ax", %progbits
.balign 4

.global BattleSceneObject_SetStateFlags
.type BattleSceneObject_SetStateFlags, %function
BattleSceneObject_SetStateFlags:
    strb r1, [r0, #0xf4]
    ldr r3, [r0, #0xf4]
    and r1, r2, #1
    bic r2, r3, #0x40000
    orr r1, r2, r1, lsl #18
    str r1, [r0, #0xf4]
    bx lr
.size BattleSceneObject_SetStateFlags, . - BattleSceneObject_SetStateFlags
