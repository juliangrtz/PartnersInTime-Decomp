/* Initialize and register a new NitroSDK ARM7 thread. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_CreateThread, "ax", %progbits
.balign 4

.extern MIi_CpuClear32
.extern OS_DisableInterrupts
.extern OS_ExitThread
.extern OS_InitContext
.extern OS_RestoreInterrupts
.extern OSi_GetUnusedThreadId
.extern OSi_InsertThreadToList
.extern OSi_ThreadTable

.global OS_CreateThread
.type OS_CreateThread, %function
OS_CreateThread:
    stmdb sp!, {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #4
    mov r9, r0
    mov r8, r1
    mov r7, r2
    mov r6, r3
    bl OS_DisableInterrupts
    mov r4, r0
    bl OSi_GetUnusedThreadId
    cmp r0, #0
    bge .L_thread_id_available
    mov r0, r4
    bl OS_RestoreInterrupts
    b .L_create_thread_return
.L_thread_id_available:
    ldr r1, [sp, #0x24]
    str r1, [r9, #0x54]
    str r0, [r9, #0x50]
    mov r1, #0
    str r1, [r9, #0x48]
    str r1, [r9, #0x58]
    ldr r1, .L_thread_table
    str r9, [r1, r0, lsl #2]
    mov r0, r9
    bl OSi_InsertThreadToList
    str r6, [r9, #0x6C]
    ldr r0, [sp, #0x20]
    sub r5, r6, r0
    str r5, [r9, #0x68]
    mov r2, #0
    str r2, [r9, #0x70]
    ldr r1, .L_stack_bottom_magic
    ldr r0, [r9, #0x6C]
    str r1, [r0, #-4]
    ldr r1, .L_stack_top_magic
    ldr r0, [r9, #0x68]
    str r1, [r0]
    strh r2, [r9, #0x74]
    mov r0, r9
    mov r1, r8
    sub r2, r6, #4
    bl OS_InitContext
    str r7, [r9, #4]
    ldr r0, .L_exit_thread
    str r0, [r9, #0x3C]
    mov r0, #0
    str r0, [sp]
    ldr r0, [sp]
    add r1, r5, #4
    ldr r2, [sp, #0x20]
    sub r2, r2, #8
    bl MIi_CpuClear32
    mov r0, #0
    str r0, [r9, #0x5C]
    str r0, [r9, #0x60]
    str r0, [r9, #0x64]
    mov r0, r4
    bl OS_RestoreInterrupts
.L_create_thread_return:
    add sp, sp, #4
    ldmia sp!, {r4, r5, r6, r7, r8, r9, lr}
    bx lr
.L_thread_table:
    .word OSi_ThreadTable
.L_stack_bottom_magic:
    .word 0xD73BFDF7
.L_stack_top_magic:
    .word 0xFBDD37BB
.L_exit_thread:
    .word OS_ExitThread
.size OS_CreateThread, . - OS_CreateThread
