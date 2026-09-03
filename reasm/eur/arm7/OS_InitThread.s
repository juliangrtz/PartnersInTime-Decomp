/* Initialize the ARM7 NitroSDK scheduler and its bootstrap thread. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_InitThread, "ax", %progbits
.balign 4

.global OS_InitThread
.type OS_InitThread, %function
.extern ARM7_Autoload000_Start
.extern HW_ARM7SystemStack
.extern HW_THREADINFO_SUBP
.extern OS_SetSwitchThreadCallback
.extern OSi_CurrentThreadPtr
.extern OSi_MainThread
.extern OSi_ThreadInfo
.extern OSi_ThreadInitialized
OS_InitThread:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_initialized
    ldr r1, [r0]
    cmp r1, #0
    bne .L_done

    mov r1, #1
    str r1, [r0]
    mov r1, #0
    mov r3, r1
    ldr r2, .L_thread_info
.L_clear_entries:
    add r0, r2, r1, lsl #2
    str r3, [r0, #0x14]
    add r1, r1, #1
    cmp r1, #0x10
    blt .L_clear_entries

    ldr r1, .L_current_field
    ldr r0, .L_current_pointer
    str r1, [r0]
    mov r1, #0x10
    ldr r0, .L_main_thread
    str r1, [r0, #0x54]
    str r3, [r0, #0x50]
    mov r1, #1
    str r1, [r0, #0x48]
    str r3, [r0, #0x4C]
    str r3, [r0, #0x58]
    str r0, [r2, #0x14]
    str r0, [r2, #0x0C]
    str r0, [r2, #8]

    ldr r2, .L_stack_size_a
    cmp r2, #0
    ldreq r0, .L_autoload_start
    subeq r2, r0, r2
    ldrne r1, .L_stack_size_b
    ldrne r0, .L_system_stack
    subne r0, r0, r1
    subne r2, r0, r2
    ldr r1, .L_stack_size_b
    ldr r0, .L_system_stack
    sub r3, r0, r1
    ldr r1, .L_main_thread
    str r3, [r1, #0x6C]
    str r2, [r1, #0x68]
    mov r0, #0
    str r0, [r1, #0x70]
    ldr r2, .L_stack_bottom_magic
    str r2, [r3, #-4]
    ldr r3, .L_stack_top_magic
    ldr r2, [r1, #0x68]
    str r3, [r2]
    strh r0, [r1, #0x74]

    mov r2, #0x10
    ldr r1, .L_thread_info
    strh r2, [r1, #2]
    strh r0, [r1]
    strh r0, [r1, #4]
    ldr r2, .L_threadinfo_subp
    str r1, [r2]
    bl OS_SetSwitchThreadCallback

.L_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr

.L_initialized:
    .word OSi_ThreadInitialized
.L_thread_info:
    .word OSi_ThreadInfo
.L_current_field:
    .word OSi_ThreadInfo + 8
.L_current_pointer:
    .word OSi_CurrentThreadPtr
.L_main_thread:
    .word OSi_MainThread
.L_stack_size_a:
    .word 0x400
.L_autoload_start:
    .word ARM7_Autoload000_Start
.L_stack_size_b:
    .word 0x400
.L_system_stack:
    .word HW_ARM7SystemStack
.L_stack_bottom_magic:
    .word 0xD73BFDF7
.L_stack_top_magic:
    .word 0xFBDD37BB
.L_threadinfo_subp:
    .word HW_THREADINFO_SUBP

.size OS_InitThread, . - OS_InitThread
