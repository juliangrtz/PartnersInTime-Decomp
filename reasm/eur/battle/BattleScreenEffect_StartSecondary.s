/* Start or defer a secondary battle screen-effect preset. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleScreenEffect_StartSecondary, "ax", %progbits
.balign 4

.extern BattleScreenEffect_FindFreeDelaySlot
.extern BattleScreenEffect_UpdateDelayed
.extern data_ov002_020c0660
.extern func_020074bc

.global BattleScreenEffect_StartSecondary
.type BattleScreenEffect_StartSecondary, %function
BattleScreenEffect_StartSecondary:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, r1
    mov r4, r2
    subs r0, r6, #1
    popmi {r4, r5, r6, lr}
    bxmi lr
    cmp r4, #0
    beq .L_start_now
    bl BattleScreenEffect_FindFreeDelaySlot
    mvn r1, #0
    cmp r0, r1
    popeq {r4, r5, r6, lr}
    bxeq lr
    mov r1, #0xc
    mul r2, r0, r1
    ldr r1, .L_runtime_context
    ldr r0, .L_delay_slots_offset
    ldr r3, [r1]
    sub r1, r6, #1
    add r3, r3, r0
    add r6, r3, r2
    ldr r0, .L_update_callback
    strb r1, [r6, #7]
    str r0, [r6, #8]
    str r4, [r3, r2]
    mov r0, #1
    strb r0, [r6, #6]
    strh r5, [r6, #4]
    pop {r4, r5, r6, lr}
    bx lr
.L_start_now:
    bl func_020074bc
    pop {r4, r5, r6, lr}
    bx lr
.L_runtime_context:
    .word data_ov002_020c0660
.L_delay_slots_offset:
    .word 0x11354
.L_update_callback:
    .word BattleScreenEffect_UpdateDelayed
.size BattleScreenEffect_StartSecondary, . - BattleScreenEffect_StartSecondary
