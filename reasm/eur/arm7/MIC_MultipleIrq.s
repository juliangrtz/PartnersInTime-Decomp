/* Install and service the microphone's nested ARM7 interrupt dispatcher. */

.syntax unified
.arch armv4t
.arm
.section .pit_MIC_MultipleIrq, "ax", %progbits
.balign 4

.extern MICi_IrqDispatchTable
.extern MICi_IrqStackInfo
.extern OS_DisableInterrupts
.extern OS_IrqHandler_ThreadSwitch
.extern OS_IRQTable
.extern OS_RestoreInterrupts
.extern OSi_ThreadInfo

.global MICi_MultipleIrqHandler
.type MICi_MultipleIrqHandler, %function
MICi_MultipleIrqHandler:
    mov r12, #0x04000000
    add r1, r12, #0x208
    ldrh r0, [r1]
    tst r0, r0
    bxeq lr
    ldr r3, [r12, #0x210]
    ldr r1, [r12, #0x214]
    ands r2, r1, r3
    bxeq lr
    ldr r0, .L_supported_irqs
    tst r2, r0
    streq r2, [r12, #0x214]
    bxeq lr

    .inst 0xE92D4000 /* stmdb sp!, {lr} */
    mrs r0, spsr
    .inst 0xE92D0001 /* stmdb sp!, {r0} */
    /* Save the interrupted system-bank stack state. */
    .inst 0xE94D6000 /* stmdb sp, {sp, lr}^ */
    sub sp, sp, #8
    mov r0, #0x9f
    msr cpsr_c, r0
    ldr r1, .L_thread_info
    ldrh r0, [r1, #4]
    add r0, r0, #1
    strh r0, [r1, #4]
    ldr r1, .L_irq_stack_info
    cmp r0, #1
    moveq r0, sp
    ldreq sp, [r1, #4]
    streq r0, [r1, #4]
    .inst 0xE92D0008 /* stmdb sp!, {r3} */

    ldr r1, .L_dispatch_table
    ldr r0, [r1]
    tst r0, r2
    strne r0, [r12, #0x214]
    ldrne r0, [r1, #4]
    ldrne r3, .L_irq_table
    ldrne r0, [r3, r0, lsl #2]
    bne .L_call_irq

    mov r3, #1
.L_find_irq:
    ldr r0, [r1, r3, lsl #3]
    tst r0, r2
    addeq r3, r3, #1
    beq .L_find_irq
    str r0, [r12, #0x214]
    add r0, r1, r3, lsl #3
    ldr r2, [r0, #4]
    ldr r3, .L_irq_table
    ldr r0, [r3, r2, lsl #2]
    ldr r2, .L_thread_info
    ldrh r3, [r2, #4]
    cmp r3, #1
    ldreq r2, [r1]
    streq r2, [r12, #0x210]
    moveq r2, #0x1f
    msreq cpsr_c, r2

.L_call_irq:
    ldr r1, [r12, #0x210]
    .inst 0xE92D0002 /* stmdb sp!, {r1} */
    adr lr, .L_after_irq_call
    bx r0

.L_after_irq_call:
    mov r0, #0x9f
    msr cpsr_c, r0
    mov r12, #0x04000000
    .inst 0xE8BD0001 /* ldmia sp!, {r0} */
    ldr r1, [r12, #0x210]
    eor r2, r0, r1
    and r1, r2, r1
    and r0, r2, r0
    .inst 0xE8BD0008 /* ldmia sp!, {r3} */
    orr r3, r3, r1
    bic r3, r3, r0
    str r3, [r12, #0x210]
    ldr r2, .L_thread_info
    ldr r3, .L_irq_stack_info
    ldrh r0, [r2, #4]
    subs r1, r0, #1
    strh r1, [r2, #4]
    moveq r0, sp
    ldreq sp, [r3, #4]
    streq r0, [r3, #4]
    mov r0, #0x92
    msr cpsr_c, r0
    /* Restore the interrupted system-bank stack state. */
    .inst 0xE8DD6000 /* ldmia sp, {sp, lr}^ */
    mov r0, r0
    add sp, sp, #8
    .inst 0xE8BD0001 /* ldmia sp!, {r0} */
    msr spsr_fc, r0
    tst r1, r1
    ldreq r0, .L_thread_switch
    addeq lr, pc, #0
    bxeq r0
    .inst 0xE8BD8000 /* ldmia sp!, {pc} */

.L_supported_irqs:
    .word 0x01df3fff
.L_thread_info:
    .word OSi_ThreadInfo
.L_irq_stack_info:
    .word MICi_IrqStackInfo
.L_dispatch_table:
    .word MICi_IrqDispatchTable
.L_irq_table:
    .word OS_IRQTable
.L_thread_switch:
    .word OS_IrqHandler_ThreadSwitch
.size MICi_MultipleIrqHandler, . - MICi_MultipleIrqHandler

.global MIC_DisableMultipleInterrupt
.type MIC_DisableMultipleInterrupt, %function
MIC_DisableMultipleInterrupt:
    .inst 0xE92D4000 /* stmdb sp!, {lr} */
    sub sp, sp, #4
    ldr r0, .L_disable_irq_vector
    ldr r1, [r0]
    ldr r0, .L_disable_handler
    cmp r1, r0
    bne .L_disable_done
    bl OS_DisableInterrupts
    ldr r1, .L_disable_stack_info
    ldr r2, [r1, #12]
    ldr r1, .L_disable_irq_vector
    str r2, [r1]
    bl OS_RestoreInterrupts
.L_disable_done:
    add sp, sp, #4
    .inst 0xE8BD4000 /* ldmia sp!, {lr} */
    bx lr
.L_disable_irq_vector:
    .word 0x0380fffc
.L_disable_handler:
    .word MICi_MultipleIrqHandler
.L_disable_stack_info:
    .word MICi_IrqStackInfo
.size MIC_DisableMultipleInterrupt, . - MIC_DisableMultipleInterrupt

.global MIC_EnableMultipleInterrupt
.type MIC_EnableMultipleInterrupt, %function
MIC_EnableMultipleInterrupt:
    .inst 0xE92D4000 /* stmdb sp!, {lr} */
    sub sp, sp, #4
    ldr r0, .L_enable_irq_vector
    ldr r2, [r0]
    ldr r0, .L_enable_handler
    cmp r2, r0
    beq .L_enable_done
    mov r1, #0
    ldr r0, .L_enable_stack_info
    str r1, [r0]
    ldr r1, .L_enable_irq_stack_top
    str r1, [r0, #4]
    mov r1, #0x40
    str r1, [r0, #8]
    str r2, [r0, #12]
    bl OS_DisableInterrupts
    ldr r2, .L_enable_handler
    ldr r1, .L_enable_irq_vector
    str r2, [r1]
    bl OS_RestoreInterrupts
.L_enable_done:
    add sp, sp, #4
    .inst 0xE8BD4000 /* ldmia sp!, {lr} */
    bx lr
.L_enable_irq_vector:
    .word 0x0380fffc
.L_enable_handler:
    .word MICi_MultipleIrqHandler
.L_enable_stack_info:
    .word MICi_IrqStackInfo
.L_enable_irq_stack_top:
    .word 0x0380fe80
.size MIC_EnableMultipleInterrupt, . - MIC_EnableMultipleInterrupt

.global MIC_SetIrqFunction
.type MIC_SetIrqFunction, %function
MIC_SetIrqFunction:
    mov r12, #0
    ldr r2, .L_set_irq_table
.L_set_irq_loop:
    ands r3, r0, #1
    strne r1, [r2, r12, lsl #2]
    mov r0, r0, lsr #1
    add r12, r12, #1
    cmp r12, #25
    blt .L_set_irq_loop
    bx lr
.L_set_irq_table:
    .word OS_IRQTable
.size MIC_SetIrqFunction, . - MIC_SetIrqFunction
