/* Initialize the ARM7 Game Pak service and install its final PXI callback. */

.syntax unified
.arch armv4t
.arm
.section .pit_CTRDG_Init, "ax", %progbits
.balign 4

.global CTRDG_Init
.type CTRDG_Init, %function
.extern CTRDGi_Initialized
.extern PXI_Init
.extern PXI_SetFifoRecvCallback
.extern func_0380698C
.extern func_03806BB8
.extern func_03806BE4
.extern func_03806C1C
CTRDG_Init:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_initialized
    ldr r1, [r0]
    cmp r1, #0
    bne .L_done
    mov r1, #1
    str r1, [r0]
    bl func_0380698C
    bl PXI_Init
    mov r0, #13
    ldr r1, .L_initial_callback
    bl PXI_SetFifoRecvCallback
    bl func_03806C1C
    mov r0, #13
    ldr r1, .L_runtime_callback
    bl PXI_SetFifoRecvCallback
.L_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_initialized:
    .word CTRDGi_Initialized
.L_initial_callback:
    .word func_03806BE4
.L_runtime_callback:
    .word func_03806BB8

.size CTRDG_Init, . - CTRDG_Init
