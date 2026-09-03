/* Wait for an ARM7 DMA channel and restore channel zero's safe defaults. */

.syntax unified
.arch armv4t
.arm
.section .pit_MI_WaitDma, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern REG_DMA0SAD

.global MI_WaitDma
.type MI_WaitDma, %function
MI_WaitDma:
    push {r4, lr}
    mov r4, r0
    bl OS_DisableInterrupts
    mov r1, #3
    mul r1, r4, r1
    add r2, r1, #2
    ldr r1, .L_dma_registers_wait
    add r2, r1, r2, lsl #2
.L_wait_busy:
    ldr r1, [r2]
    ands r1, r1, #0x80000000
    bne .L_wait_busy
    cmp r4, #0
    bne .L_wait_done
    mov r1, #0x0C
    mul r3, r4, r1
    ldr r1, .L_dma_registers_wait
    add r4, r3, r1
    mov r2, #0
    add r1, r3, #0x04000000
    str r2, [r1, #0xB0]
    str r2, [r4, #4]
    ldr r1, .L_dma_safe_control_wait
    str r1, [r4, #8]
.L_wait_done:
    bl OS_RestoreInterrupts
    pop {r4, lr}
    bx lr
.size MI_WaitDma, . - MI_WaitDma
.L_dma_registers_wait:
    .word REG_DMA0SAD
.L_dma_safe_control_wait:
    .word 0x81400001
