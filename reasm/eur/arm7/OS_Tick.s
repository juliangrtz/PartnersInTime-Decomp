/* NitroSDK ARM7 tick counter built on hardware timer 0. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Tick, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_EnableIrqMask
.extern OS_RestoreInterrupts
.extern OS_SetIrqFunction
.extern OSi_SetTimerCallback
.extern OSi_TickCounter
.extern OSi_TickInitialized
.extern OSi_TickResetRequested
.extern OSi_TimerReserved
.extern REG_IF
.extern REG_TM0CNT_H
.extern REG_TM0CNT_L

.global OSi_SetTimerReserved
.type OSi_SetTimerReserved, %function
OSi_SetTimerReserved:
    ldr r1, .L_timer_reserved
    ldrh r3, [r1]
    mov r2, #1
    orr r0, r3, r2, lsl r0
    strh r0, [r1]
    bx lr
.L_timer_reserved:
    .word OSi_TimerReserved
.size OSi_SetTimerReserved, . - OSi_SetTimerReserved

.global OS_GetTick
.type OS_GetTick, %function
OS_GetTick:
    stmdb sp!, {lr}
    sub sp, sp, #0xC
    bl OS_DisableInterrupts
    ldr r1, .L_timer_count
    ldrh r1, [r1]
    strh r1, [sp]
    ldr r1, .L_tick_counter_get
    ldr r12, [r1]
    ldr r3, [r1, #4]
    ldr r2, .L_tick_high_mask
    mvn r1, #0
    and r2, r3, r2
    and r1, r12, r1
    str r1, [sp, #4]
    str r2, [sp, #8]
    ldr r1, .L_irq_request
    ldr r1, [r1]
    ands r1, r1, #8
    beq .L_restore_get_tick
    ldrh r1, [sp]
    ands r1, r1, #0x8000
    bne .L_restore_get_tick
    ldr r3, [sp, #4]
    ldr r2, [sp, #8]
    mov r1, #1
    adds r3, r3, r1
    adc r1, r2, #0
    str r3, [sp, #4]
    str r1, [sp, #8]
.L_restore_get_tick:
    bl OS_RestoreInterrupts
    ldr r2, [sp, #4]
    ldr r0, [sp, #8]
    mov r1, r0, lsl #16
    orr r1, r1, r2, lsr #16
    ldrh r0, [sp]
    orr r1, r1, r0, asr #31
    orr r0, r0, r2, lsl #16
    add sp, sp, #0xC
    ldmia sp!, {lr}
    bx lr
.L_timer_count:
    .word REG_TM0CNT_L
.L_tick_counter_get:
    .word OSi_TickCounter
.L_tick_high_mask:
    .word 0x0000FFFF
.L_irq_request:
    .word REG_IF
.size OS_GetTick, . - OS_GetTick

.global OSi_CountUpTick
.type OSi_CountUpTick, %function
OSi_CountUpTick:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_tick_counter_irq
    ldr r12, [r1]
    ldr r2, [r1, #4]
    mov r3, #0
    mov r0, #1
    adds r12, r12, r0
    adc r0, r2, #0
    str r12, [r1]
    str r0, [r1, #4]
    ldr r0, .L_tick_reset_requested_irq
    ldr r1, [r0]
    cmp r1, #0
    beq .L_rearm_tick_callback
    ldr r2, .L_timer_control_irq
    strh r3, [r2]
    ldr r1, .L_timer_count_irq
    strh r3, [r1]
    mov r1, #0xC1
    strh r1, [r2]
    str r3, [r0]
.L_rearm_tick_callback:
    mov r0, #0
    ldr r1, .L_count_up_tick
    mov r2, r0
    bl OSi_SetTimerCallback
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_tick_counter_irq:
    .word OSi_TickCounter
.L_tick_reset_requested_irq:
    .word OSi_TickResetRequested
.L_timer_control_irq:
    .word REG_TM0CNT_H
.L_timer_count_irq:
    .word REG_TM0CNT_L
.L_count_up_tick:
    .word OSi_CountUpTick
.size OSi_CountUpTick, . - OSi_CountUpTick

.global OS_IsTickAvailable
.type OS_IsTickAvailable, %function
OS_IsTickAvailable:
    ldr r0, .L_tick_initialized_get
    ldrh r0, [r0]
    bx lr
.L_tick_initialized_get:
    .word OSi_TickInitialized
.size OS_IsTickAvailable, . - OS_IsTickAvailable

.global OS_InitTick
.type OS_InitTick, %function
OS_InitTick:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_tick_initialized_init
    ldrh r1, [r0]
    cmp r1, #0
    bne .L_init_tick_done
    mov r1, #1
    strh r1, [r0]
    mov r0, #0
    bl OSi_SetTimerReserved
    mov r2, #0
    ldr r0, .L_tick_counter_init
    str r2, [r0]
    str r2, [r0, #4]
    ldr r1, .L_timer_control_init
    strh r2, [r1]
    ldr r0, .L_timer_count_init
    strh r2, [r0]
    mov r0, #0xC1
    strh r0, [r1]
    mov r0, #8
    ldr r1, .L_count_up_tick_init
    bl OS_SetIrqFunction
    mov r0, #8
    bl OS_EnableIrqMask
    mov r1, #0
    ldr r0, .L_tick_reset_requested_init
    str r1, [r0]
.L_init_tick_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_tick_initialized_init:
    .word OSi_TickInitialized
.L_tick_counter_init:
    .word OSi_TickCounter
.L_timer_control_init:
    .word REG_TM0CNT_H
.L_timer_count_init:
    .word REG_TM0CNT_L
.L_count_up_tick_init:
    .word OSi_CountUpTick
.L_tick_reset_requested_init:
    .word OSi_TickResetRequested
.size OS_InitTick, . - OS_InitTick
