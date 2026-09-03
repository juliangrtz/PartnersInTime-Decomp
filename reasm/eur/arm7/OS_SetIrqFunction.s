/* NitroSDK interrupt-handler and callback registration. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_SetIrqFunction, "ax", %progbits
.balign 4

.global OS_SetIrqFunction
.type OS_SetIrqFunction, %function
.extern OS_IRQTable
.extern OSi_IrqCallbackInfo
.extern OSi_VBlankCallbackInfo
OS_SetIrqFunction:
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r9, #0
    mov r7, r9
    ldr lr, .L_irq_table
    ldr r4, .L_vblank_callback_info
    ldr r6, .L_irq_callback_info
    mov r12, r9
    mov r3, #1
    mov r2, #12
.L_next_irq:
    ands r5, r0, #1
    beq .L_advance

    mov r8, r7
    cmp r9, #8
    blt .L_check_timer
    cmp r9, #11
    suble r5, r9, #8
    mlale r8, r5, r2, r6
    ble .L_install_callback

.L_check_timer:
    cmp r9, #3
    blt .L_check_vblank
    cmp r9, #6
    addle r5, r9, #1
    mlale r8, r5, r2, r6
    ble .L_install_callback

.L_check_vblank:
    cmp r9, #0
    moveq r8, r4
    strne r1, [lr, r9, lsl #2]

.L_install_callback:
    cmp r8, #0
    strne r1, [r8]
    strne r12, [r8, #8]
    strne r3, [r8, #4]

.L_advance:
    lsr r0, r0, #1
    add r9, r9, #1
    cmp r9, #0x19
    blt .L_next_irq

    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bx lr

.L_irq_table:
    .word OS_IRQTable
.L_vblank_callback_info:
    .word OSi_VBlankCallbackInfo
.L_irq_callback_info:
    .word OSi_IrqCallbackInfo

.size OS_SetIrqFunction, . - OS_SetIrqFunction
