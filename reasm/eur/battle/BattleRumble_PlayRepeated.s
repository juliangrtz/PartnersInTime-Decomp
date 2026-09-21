/* Start or defer a secondary battle screen-effect preset. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleRumble_PlayRepeated, "ax", %progbits
.balign 4

.extern BattleRumble_FindFreeDelaySlot
.extern BattleRumble_UpdateDelayed
.extern data_ov002_020c0660
.extern GameRumble_PlayRepeated

.global BattleRumble_PlayRepeated
.type BattleRumble_PlayRepeated, %function
BattleRumble_PlayRepeated:
    push {r4, r5, r6, lr}
    mov r6, r0
    mov r5, r1
    mov r4, r2
    subs r0, r6, #1
    popmi {r4, r5, r6, lr}
    bxmi lr
    cmp r4, #0
    beq .L_start_now
    bl BattleRumble_FindFreeDelaySlot
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
    bl GameRumble_PlayRepeated
    pop {r4, r5, r6, lr}
    bx lr
.L_runtime_context:
    .word data_ov002_020c0660
.L_delay_slots_offset:
    .word 0x11354
.L_update_callback:
    .word BattleRumble_UpdateDelayed
.size BattleRumble_PlayRepeated, . - BattleRumble_PlayRepeated
