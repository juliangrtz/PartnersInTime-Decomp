/* Stop one ARM7 DMA channel using NitroSDK's register shutdown sequence. */

.syntax unified
.arch armv4t
.arm
.section .pit_MI_StopDma, "ax", %progbits
.balign 4

.global MI_StopDma
.type MI_StopDma, %function
.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern REG_DMA0SAD
MI_StopDma:
    stmdb sp!, {r4, lr}
    mov r4, r0
    bl OS_DisableInterrupts

    mov r1, #6
    mul r1, r4, r1
    add r1, r1, #5
    mov r1, r1, lsl #1
    add r1, r1, #0x04000000
    ldrh r2, [r1, #0xB0]
    bic r2, r2, #0x3200
    strh r2, [r1, #0xB0]
    ldrh r2, [r1, #0xB0]
    bic r2, r2, #0x8000
    strh r2, [r1, #0xB0]
    ldrh r2, [r1, #0xB0]
    ldrh r1, [r1, #0xB0]

    cmp r4, #0
    bne .L_restore_interrupts
    mov r1, #12
    mul r3, r4, r1
    ldr r1, .L_dma_registers
    add r4, r3, r1
    mov r2, #0
    add r1, r3, #0x04000000
    str r2, [r1, #0xB0]
    str r2, [r4, #4]
    ldr r1, .L_dma_control_default
    str r1, [r4, #8]

.L_restore_interrupts:
    bl OS_RestoreInterrupts
    ldmia sp!, {r4, lr}
    bx lr

.L_dma_registers:
    .word REG_DMA0SAD
.L_dma_control_default:
    .word 0x81400001

.size MI_StopDma, . - MI_StopDma
