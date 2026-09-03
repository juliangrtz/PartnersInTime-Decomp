/* Thread-list management, ID allocation, and saved CPU contexts. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_ThreadCore, "ax", %progbits
.balign 4

.extern OSi_ThreadInfo

.global OSi_RemoveThreadFromList
.type OSi_RemoveThreadFromList, %function
OSi_RemoveThreadFromList:
    ldr r1, .L_thread_info_remove
    ldr r2, [r1, #0x0C]
    mov r1, #0
    b .L_test_removed_thread
.L_next_removed_thread:
    mov r1, r2
    ldr r2, [r2, #0x4C]
.L_test_removed_thread:
    cmp r2, #0
    beq .L_unlink_thread
    cmp r2, r0
    bne .L_next_removed_thread
.L_unlink_thread:
    cmp r1, #0
    ldreq r1, [r0, #0x4C]
    ldreq r0, .L_thread_info_remove
    streq r1, [r0, #0x0C]
    ldrne r0, [r0, #0x4C]
    strne r0, [r1, #0x4C]
    bx lr
.L_thread_info_remove:
    .word OSi_ThreadInfo
.size OSi_RemoveThreadFromList, . - OSi_RemoveThreadFromList

.global OSi_InsertThreadToList
.type OSi_InsertThreadToList, %function
OSi_InsertThreadToList:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_thread_info_insert
    ldr r3, [r1, #0x0C]
    mov lr, r3
    mov r12, #0
    b .L_test_insert_position
.L_next_insert_position:
    mov r12, lr
    ldr lr, [lr, #0x4C]
.L_test_insert_position:
    cmp lr, #0
    beq .L_insert_thread
    ldr r2, [lr, #0x54]
    ldr r1, [r0, #0x54]
    cmp r2, r1
    blo .L_next_insert_position
.L_insert_thread:
    cmp r12, #0
    streq r3, [r0, #0x4C]
    ldreq r1, .L_thread_info_insert
    streq r0, [r1, #0x0C]
    ldrne r1, [r12, #0x4C]
    strne r1, [r0, #0x4C]
    strne r0, [r12, #0x4C]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_thread_info_insert:
    .word OSi_ThreadInfo
.size OSi_InsertThreadToList, . - OSi_InsertThreadToList

.global OSi_GetUnusedThreadId
.type OSi_GetUnusedThreadId, %function
OSi_GetUnusedThreadId:
    mov r0, #0
    ldr r2, .L_thread_info_id
    b .L_test_thread_id
.L_next_thread_id:
    add r1, r2, r0, lsl #2
    ldr r1, [r1, #0x14]
    cmp r1, #0
    bxeq lr
    add r0, r0, #1
.L_test_thread_id:
    cmp r0, #0x10
    blt .L_next_thread_id
    mvn r0, #0
    bx lr
.L_thread_info_id:
    .word OSi_ThreadInfo
.size OSi_GetUnusedThreadId, . - OSi_GetUnusedThreadId

.global OS_InitContext
.type OS_InitContext, %function
OS_InitContext:
    add r1, r1, #4
    str r1, [r0, #0x40]
    str r2, [r0, #0x44]
    sub r2, r2, #0x40
    str r2, [r0, #0x38]
    ands r1, r1, #1
    movne r1, #0x3F
    moveq r1, #0x1F
    str r1, [r0]
    mov r1, #0
    str r1, [r0, #4]
    str r1, [r0, #8]
    str r1, [r0, #0x0C]
    str r1, [r0, #0x10]
    str r1, [r0, #0x14]
    str r1, [r0, #0x18]
    str r1, [r0, #0x1C]
    str r1, [r0, #0x20]
    str r1, [r0, #0x24]
    str r1, [r0, #0x28]
    str r1, [r0, #0x2C]
    str r1, [r0, #0x30]
    str r1, [r0, #0x34]
    str r1, [r0, #0x3C]
    bx lr
.size OS_InitContext, . - OS_InitContext

.global OS_SaveContext
.type OS_SaveContext, %function
OS_SaveContext:
    add r1, r0, #0
    mrs r2, cpsr
    str r2, [r1], #4
    mov r0, #0xD3
    msr cpsr_c, r0
    str sp, [r1, #0x40]
    msr cpsr_c, r2
    mov r0, #1
    stmia r1, {r0-r12, sp, lr}
    add r0, pc, #8
    str r0, [r1, #0x3C]
    mov r0, #0
    bx lr
.size OS_SaveContext, . - OS_SaveContext

.global OS_LoadContext
.type OS_LoadContext, %function
OS_LoadContext:
    mrs r1, cpsr
    bic r1, r1, #0x1F
    orr r1, r1, #0xD3
    msr cpsr_c, r1
    ldr r1, [r0], #4
    msr spsr_fsxc, r1
    ldr sp, [r0, #0x40]
    ldr lr, [r0, #0x3C]
    /* LDMIA with user-bank transfer is valid on ARM7TDMI but rejected by LLVM. */
    .inst 0xE8D07FFF
    mov r0, r0
    subs pc, lr, #4
.size OS_LoadContext, . - OS_LoadContext
