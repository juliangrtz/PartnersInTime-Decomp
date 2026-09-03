/* Advance one normalized constant- or accelerated-speed motion channel. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleSceneObject_UpdateAcceleratedMotion, "ax", %progbits
.balign 4

.global BattleSceneObject_UpdateAcceleratedMotion
.type BattleSceneObject_UpdateAcceleratedMotion, %function
BattleSceneObject_UpdateAcceleratedMotion:
    push {r4, r5, r6, lr}
    ldr r4, [r1, #4]
    ldrsh r3, [r1, #8]
    asr r2, r4, #7
    add r2, r4, r2, lsr #24
    asr r2, r2, #8
    cmp r2, r3
    add r2, r1, #0x18
    bne .L_accelerated_step
    ldrsh r1, [r2, #8]
    ldrsh r3, [r2, #0xa]
    ldrsh r5, [r0, #4]
    mul r4, r3, r1
    asr r3, r4, #0xd
    add r3, r4, r3, lsr #18
    add r3, r5, r3, asr #14
    strh r3, [r0, #4]
    ldrsh r3, [r2, #0xc]
    ldrsh r5, [r0, #6]
    mul r4, r3, r1
    asr r3, r4, #0xd
    add r3, r4, r3, lsr #18
    add r3, r5, r3, asr #14
    strh r3, [r0, #6]
    ldrsh r2, [r2, #0xe]
    ldrsh r4, [r0, #8]
    mul r3, r2, r1
    asr r1, r3, #0xd
    add r1, r3, r1, lsr #18
    add r1, r4, r1, asr #14
    strh r1, [r0, #8]
    pop {r4, r5, r6, lr}
    bx lr
.L_accelerated_step:
    asr r1, r4, #3
    add r1, r4, r1, lsr #28
    ldr r3, [r2]
    asr r1, r1, #4
    mul r4, r3, r1
    asr r3, r4, #3
    ldr r5, [r2, #4]
    add r3, r4, r3, lsr #28
    add r4, r5, r3, asr #4
    asr r3, r4, #4
    add r3, r4, r3, lsr #27
    ldrsh r4, [r2, #0xa]
    asr r6, r3, #5
    ldrsh r3, [r2, #0xc]
    mul ip, r4, r6
    mul lr, r3, r6
    asr r3, ip, #0xb
    add r3, ip, r3, lsr #20
    ldrsh r4, [r2, #0xe]
    asr r2, r3, #0xc
    mul r5, r2, r1
    mul r6, r4, r6
    asr r2, lr, #0xb
    add r2, lr, r2, lsr #20
    asr r2, r2, #0xc
    mul r4, r2, r1
    asr r3, r5, #0x11
    asr r2, r6, #0xb
    add r2, r6, r2, lsr #20
    asr r2, r2, #0xc
    mul lr, r2, r1
    asr r2, r4, #0x11
    asr ip, lr, #0x11
    ldrsh r6, [r0, #4]
    add r1, r5, r3, lsr #14
    add r2, r4, r2, lsr #14
    add r1, r6, r1, asr #18
    strh r1, [r0, #4]
    ldrsh r3, [r0, #6]
    add r1, lr, ip, lsr #14
    add r2, r3, r2, asr #18
    strh r2, [r0, #6]
    ldrsh r2, [r0, #8]
    add r1, r2, r1, asr #18
    strh r1, [r0, #8]
    pop {r4, r5, r6, lr}
    bx lr
.size BattleSceneObject_UpdateAcceleratedMotion, . - BattleSceneObject_UpdateAcceleratedMotion
