/* Initialize a fixed task pool with 12-byte headers and aligned payloads. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleTaskPool_Init, "ax", %progbits
.balign 4

.global BattleTaskPool_Init
.type BattleTaskPool_Init, %function
BattleTaskPool_Init:
    add ip, r0, #8
    add r3, r2, #3
    str ip, [r0, #4]
    mov r2, #0
    str r2, [r0]
    bic r2, r3, #3
.L_link_next:
    sub r1, r1, #1
    cmp r1, #0
    addgt r0, ip, #0x0C
    addgt r0, r0, r2
    strgt r0, [ip]
    ldrgt ip, [ip]
    bgt .L_link_next
    mov r0, #0
    str r0, [ip]
    bx lr
.size BattleTaskPool_Init, . - BattleTaskPool_Init
