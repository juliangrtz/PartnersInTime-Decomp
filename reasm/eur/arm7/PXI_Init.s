/* Public PXI initialization entry forwarding to the FIFO implementation. */

.syntax unified
.arch armv4t
.arm
.section .pit_PXI_Init, "ax", %progbits
.balign 4

.global PXI_Init
.type PXI_Init, %function
.extern PXIi_InitFifo
PXI_Init:
    ldr r12, .L_init_fifo
    bx r12
.L_init_fifo:
    .word PXIi_InitFifo

.size PXI_Init, . - PXI_Init
