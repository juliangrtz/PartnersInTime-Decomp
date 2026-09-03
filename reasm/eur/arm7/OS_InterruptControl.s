/* NitroSDK CPSR interrupt-control leaf functions. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_InterruptControl, "ax", %progbits
.balign 4

.global OS_EnableInterrupts
.type OS_EnableInterrupts, %function
OS_EnableInterrupts:
    mrs r0, cpsr
    bic r1, r0, #0x80
    msr cpsr_c, r1
    and r0, r0, #0x80
    bx lr
.size OS_EnableInterrupts, . - OS_EnableInterrupts

.global OS_DisableInterrupts
.type OS_DisableInterrupts, %function
OS_DisableInterrupts:
    mrs r0, cpsr
    orr r1, r0, #0x80
    msr cpsr_c, r1
    and r0, r0, #0x80
    bx lr
.size OS_DisableInterrupts, . - OS_DisableInterrupts

.global OS_RestoreInterrupts
.type OS_RestoreInterrupts, %function
OS_RestoreInterrupts:
    mrs r1, cpsr
    bic r2, r1, #0x80
    orr r2, r2, r0
    msr cpsr_c, r2
    and r0, r1, #0x80
    bx lr
.size OS_RestoreInterrupts, . - OS_RestoreInterrupts

.global OS_DisableInterrupts_IrqAndFiq
.type OS_DisableInterrupts_IrqAndFiq, %function
OS_DisableInterrupts_IrqAndFiq:
    mrs r0, cpsr
    orr r1, r0, #0xC0
    msr cpsr_c, r1
    and r0, r0, #0xC0
    bx lr
.size OS_DisableInterrupts_IrqAndFiq, . - OS_DisableInterrupts_IrqAndFiq

.global OS_RestoreInterrupts_IrqAndFiq
.type OS_RestoreInterrupts_IrqAndFiq, %function
OS_RestoreInterrupts_IrqAndFiq:
    mrs r1, cpsr
    bic r2, r1, #0xC0
    orr r2, r2, r0
    msr cpsr_c, r2
    and r0, r1, #0xC0
    bx lr
.size OS_RestoreInterrupts_IrqAndFiq, . - OS_RestoreInterrupts_IrqAndFiq

.global OS_GetProcessorMode
.type OS_GetProcessorMode, %function
OS_GetProcessorMode:
    mrs r0, cpsr
    and r0, r0, #0x1F
    bx lr
.size OS_GetProcessorMode, . - OS_GetProcessorMode
