/* Configure the autoloaded WM service and manage wireless power state. */

.syntax unified
.arch armv4t
.arm
.section .pit_WVR_Control, "ax", %progbits
.balign 4

.extern MI_CpuFill8
.extern PM_SetLEDPattern
.extern PMi_SetLED
.extern WMSP_Init
.extern WVRi_Callback
.extern WVRi_Initialized
.extern WVRi_ShutdownPending
.extern WVRi_SystemWork
.extern WVRi_WmspSystemWork
.extern WVRi_WmspWork

.global WVRi_InitWmsp
.type WVRi_InitWmsp, %function
WVRi_InitWmsp:
    stmdb sp!, {lr}
    sub sp, sp, #84
    ldr r1, .L_wmsp_work_0
    str r1, [sp]
    ldr r1, .L_wmsp_work_1
    str r1, [sp, #4]
    mov r1, #0x600
    str r1, [sp, #8]
    mov r3, #4
    str r3, [sp, #12]
    mov r1, #0
    str r1, [sp, #32]
    mov r2, #8
    str r2, [sp, #36]
    str r0, [sp, #40]
    ldr r0, .L_system_work
    str r0, [sp, #44]
    mov r0, #0x1c0
    str r0, [sp, #48]
    mov r1, #3
    str r1, [sp, #52]
    mov r0, #64
    str r0, [sp, #28]
    str r1, [sp, #60]
    str r3, [sp, #76]
    mov r0, #5
    str r0, [sp, #68]
    mov r0, #7
    str r0, [sp, #56]
    str r2, [sp, #72]
    mov r0, #9
    str r0, [sp, #64]
    add r0, sp, #0
    add r1, sp, #52
    bl WMSP_Init
    add sp, sp, #84
    ldmia sp!, {lr}
    bx lr
.L_wmsp_work_0:
    .word WVRi_WmspWork
.L_wmsp_work_1:
    .word WVRi_WmspWork
.L_system_work:
    .word WVRi_WmspSystemWork
.size WVRi_InitWmsp, . - WVRi_InitWmsp

.global WVR_Shutdown
.type WVR_Shutdown, %function
WVR_Shutdown:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_powcnt2
    ldrh r0, [r1]
    bic r0, r0, #2
    strh r0, [r1]
    mov r0, #1
    bl PM_SetLEDPattern
    mov r0, #1
    bl PMi_SetLED
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_powcnt2:
    .word 0x04000304
.size WVR_Shutdown, . - WVR_Shutdown

.global WVR_Init
.type WVR_Init, %function
WVR_Init:
    stmdb sp!, {r4, lr}
    mov r4, r0
    mov r1, #0
    ldr r0, .L_shutdown_pending
    str r1, [r0]
    ldr r0, .L_callback
    str r4, [r0]
    ldr r0, .L_system_state
    mov r2, #120
    bl MI_CpuFill8
    mov r0, r4
    bl WVRi_InitWmsp
    mov r1, #3
    ldr r0, .L_initialized
    strb r1, [r0]
    ldmia sp!, {r4, lr}
    bx lr
.L_shutdown_pending:
    .word WVRi_ShutdownPending
.L_callback:
    .word WVRi_Callback
.L_system_state:
    .word WVRi_SystemWork
.L_initialized:
    .word WVRi_Initialized
.size WVR_Init, . - WVR_Init
