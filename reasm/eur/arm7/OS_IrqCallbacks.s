/* NitroSDK callback-backed VBlank, timer, and DMA IRQ dispatch. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_IrqCallbacks, "ax", %progbits
.balign 4

.extern HW_INTR_CHECK_BUF
.extern HW_VBLANK_COUNT_BUF
.extern OS_DisableIrqMask
.extern OSi_IrqCallbackInfo
.extern OSi_IrqCallbackInfoIndex

.global OSi_IrqVBlank
.type OSi_IrqVBlank, %function
OSi_IrqVBlank:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_irq_callback_info_vblank
    ldr r2, [r0, #0x60]
    ldr r1, .L_vblank_count
    ldr r0, [r1]
    add r0, r0, #1
    str r0, [r1]
    cmp r2, #0
    beq .L_vblank_callback_done
    mov lr, pc
    bx r2
.L_vblank_callback_done:
    ldr r1, .L_intr_check_vblank
    ldr r0, [r1]
    orr r0, r0, #1
    str r0, [r1]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_irq_callback_info_vblank:
    .word OSi_IrqCallbackInfo
.L_vblank_count:
    .word HW_VBLANK_COUNT_BUF
.L_intr_check_vblank:
    .word HW_INTR_CHECK_BUF
.size OSi_IrqVBlank, . - OSi_IrqVBlank

.global OSi_IrqTimer3
.type OSi_IrqTimer3, %function
OSi_IrqTimer3:
    mov r0, #7
    ldr r12, .L_irq_callback_timer3
    bx r12
.L_irq_callback_timer3:
    .word OSi_IrqCallback
.size OSi_IrqTimer3, . - OSi_IrqTimer3

.global OSi_IrqTimer2
.type OSi_IrqTimer2, %function
OSi_IrqTimer2:
    mov r0, #6
    ldr r12, .L_irq_callback_timer2
    bx r12
.L_irq_callback_timer2:
    .word OSi_IrqCallback
.size OSi_IrqTimer2, . - OSi_IrqTimer2

.global OSi_IrqTimer1
.type OSi_IrqTimer1, %function
OSi_IrqTimer1:
    mov r0, #5
    ldr r12, .L_irq_callback_timer1
    bx r12
.L_irq_callback_timer1:
    .word OSi_IrqCallback
.size OSi_IrqTimer1, . - OSi_IrqTimer1

.global OSi_IrqTimer0
.type OSi_IrqTimer0, %function
OSi_IrqTimer0:
    mov r0, #4
    ldr r12, .L_irq_callback_timer0
    bx r12
.L_irq_callback_timer0:
    .word OSi_IrqCallback
.size OSi_IrqTimer0, . - OSi_IrqTimer0

.global OSi_IrqDma3
.type OSi_IrqDma3, %function
OSi_IrqDma3:
    mov r0, #3
    ldr r12, .L_irq_callback_dma3
    bx r12
.L_irq_callback_dma3:
    .word OSi_IrqCallback
.size OSi_IrqDma3, . - OSi_IrqDma3

.global OSi_IrqDma2
.type OSi_IrqDma2, %function
OSi_IrqDma2:
    mov r0, #2
    ldr r12, .L_irq_callback_dma2
    bx r12
.L_irq_callback_dma2:
    .word OSi_IrqCallback
.size OSi_IrqDma2, . - OSi_IrqDma2

.global OSi_IrqDma1
.type OSi_IrqDma1, %function
OSi_IrqDma1:
    mov r0, #1
    ldr r12, .L_irq_callback_dma1
    bx r12
.L_irq_callback_dma1:
    .word OSi_IrqCallback
.size OSi_IrqDma1, . - OSi_IrqDma1

.global OSi_IrqDma0
.type OSi_IrqDma0, %function
OSi_IrqDma0:
    mov r0, #0
    ldr r12, .L_irq_callback_dma0
    bx r12
.L_irq_callback_dma0:
    .word OSi_IrqCallback
.size OSi_IrqDma0, . - OSi_IrqDma0

.global OSi_IrqCallback
.type OSi_IrqCallback, %function
OSi_IrqCallback:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r3, #1
    mov r2, r0, lsl #1
    ldr r1, .L_irq_callback_index
    ldrh r1, [r1, r2]
    mov r5, r3, lsl r1
    mov r1, #12
    mul r4, r0, r1
    ldr r2, .L_irq_callback_info
    ldr r1, [r2, r4]
    mov r0, #0
    str r0, [r2, r4]
    cmp r1, #0
    beq .L_callback_done
    ldr r0, .L_irq_callback_argument
    ldr r0, [r0, r4]
    mov lr, pc
    bx r1
.L_callback_done:
    ldr r1, .L_intr_check_callback
    ldr r0, [r1]
    orr r0, r0, r5
    str r0, [r1]
    ldr r0, .L_irq_callback_enabled
    ldr r0, [r0, r4]
    cmp r0, #0
    bne .L_irq_callback_return
    mov r0, r5
    bl OS_DisableIrqMask
.L_irq_callback_return:
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_irq_callback_index:
    .word OSi_IrqCallbackInfoIndex
.L_irq_callback_info:
    .word OSi_IrqCallbackInfo
.L_irq_callback_argument:
    .word OSi_IrqCallbackInfo + 8
.L_intr_check_callback:
    .word HW_INTR_CHECK_BUF
.L_irq_callback_enabled:
    .word OSi_IrqCallbackInfo + 4
.size OSi_IrqCallback, . - OSi_IrqCallback

.global OS_IrqDummy
.type OS_IrqDummy, %function
OS_IrqDummy:
    bx lr
.size OS_IrqDummy, . - OS_IrqDummy
