/* NitroSDK ARM7 interrupt dispatch and thread-switch path. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_IrqHandler, "ax", %progbits
.balign 4

.global OS_IrqHandler
.type OS_IrqHandler, %function
.extern OS_IRQCheckFlag
.extern OS_IRQTable
.extern OSi_ThreadInfo
OS_IrqHandler:
    stmdb sp!, {lr}
    mov r12, #0x04000000
    add r12, r12, #0x210
    ldr r1, [r12, #-8]
    cmp r1, #0
    ldmiaeq sp!, {pc}

    ldm r12, {r1, r2}
    ands r1, r1, r2
    ldmiaeq sp!, {pc}

    mov r3, #1
    mov r0, #0
.L_find_irq:
    ands r2, r1, r3, lsl r0
    addeq r0, r0, #1
    beq .L_find_irq

    str r2, [r12, #4]
    ldr r1, .L_irq_table
    ldr r0, [r1, r0, lsl #2]
    ldr lr, .L_irq_return_address
    bx r0

.L_irq_table:
    .word OS_IRQTable
.L_irq_return_address:
    .word .L_irq_return

.L_irq_return:
    ldr r12, .L_irq_check_flag
    ldrh r3, [r12]
    mov r0, #0
    cmp r3, #0
    beq .L_check_scheduler

    strh r0, [r12]
    ldr r12, .L_thread_info
    mov r1, #1
    strh r1, [r12]
    ldr r12, [r12, #0xC]
    mov r2, #1
.L_mark_threads:
    cmp r12, #0
    beq .L_check_scheduler
    ldr r0, [r12, #0x50]
    tst r3, r1, lsl r0
    strne r2, [r12, #0x48]
    ldr r12, [r12, #0x4C]
    b .L_mark_threads

.L_check_scheduler:
    ldr r12, .L_thread_info
    ldrh r1, [r12]
    cmp r1, #0
    ldreq pc, [sp], #4

    mov r1, #0
    strh r1, [r12]
    mov r3, #0xD2
    msr cpsr_c, r3
    add r2, r12, #0xC
    ldr r1, [r2]
.L_find_runnable_thread:
    cmp r1, #0
    ldrhne r0, [r1, #0x48]
    cmpne r0, #1
    ldrne r1, [r1, #0x4C]
    bne .L_find_runnable_thread

    cmp r1, #0
    bne .L_switch_thread
.L_no_runnable_thread:
    mov r3, #0x92
    msr cpsr_c, r3
    ldr pc, [sp], #4

.L_switch_thread:
    ldr r0, [r12, #8]
    cmp r1, r0
    beq .L_no_runnable_thread
    ldr r3, [r12, #0x10]
    cmp r3, #0
    beq .L_save_context
    push {r0, r1, r12}
    mov lr, pc
    bx r3
    pop {r0, r1, r12}

.L_save_context:
    str r1, [r12, #8]
    mrs r2, spsr
    str r2, [r0, #0]!
    ldmib sp!, {r2, r3}
    stmib r0!, {r2, r3}
    ldmib sp!, {r2, r3, r12, lr}
    /* Save the user-bank registers. LLVM rejects this legal ARM7TDMI form. */
    .inst 0xE9E07FFC /* stmib r0!, {r2-r12, sp, lr}^ */
    stmib r0!, {lr}
    mov r3, #0xD3
    msr cpsr_c, r3
    stmib r0!, {sp}
    ldr sp, [r1, #0x44]
    mov r3, #0xD2
    msr cpsr_c, r3
    ldr r2, [r1, #0]!
    msr spsr_fc, r2
    ldr lr, [r1, #0x40]
    /* Restore the user-bank registers; see the matching save above. */
    .inst 0xE9F17FFF /* ldmib r1!, {r0-r12, sp, lr}^ */
    mov r0, r0
    stmda sp!, {r0, r1, r2, r3, r12, lr}
    ldm sp!, {pc}

.L_irq_check_flag:
    .word OS_IRQCheckFlag
.L_thread_info:
    .word OSi_ThreadInfo

.size OS_IrqHandler, . - OS_IrqHandler
