/* Publish hardware status and sequence variables to ARM9 shared work. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_SharedWork, "ax", %progbits
.balign 4

.extern SND_IsCaptureActive
.extern SND_IsChannelActive
.extern SNDi_SharedWork

.global SND_UpdateSharedWork
.type SND_UpdateSharedWork, %function
SND_UpdateSharedWork:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r5, #0
    mov r4, r5
    ldr r0, .L_shared_work_status
    ldr r0, [r0]
    cmp r0, #0
    beq .L_update_shared_done
    mov r7, r5
    mov r6, #1
.L_channel_status_loop:
    mov r0, r7
    bl SND_IsChannelActive
    cmp r0, #0
    orrne r0, r5, r6, lsl r7
    lslne r0, r0, #16
    lsrne r5, r0, #16
    add r7, r7, #1
    cmp r7, #16
    blt .L_channel_status_loop
    mov r0, #0
    bl SND_IsCaptureActive
    cmp r0, #0
    orrne r0, r4, #1
    lslne r0, r0, #16
    lsrne r4, r0, #16
    mov r0, #1
    bl SND_IsCaptureActive
    cmp r0, #0
    orrne r0, r4, #2
    lslne r0, r0, #16
    lsrne r4, r0, #16
    ldr r0, .L_shared_work_status
    ldr r1, [r0]
    strh r5, [r1, #8]
    ldr r0, [r0]
    strh r4, [r0, #0xa]
.L_update_shared_done:
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.L_shared_work_status:
    .word SNDi_SharedWork
.size SND_UpdateSharedWork, . - SND_UpdateSharedWork

.global SND_SetPlayerGlobalVariable
.type SND_SetPlayerGlobalVariable, %function
SND_SetPlayerGlobalVariable:
    ldr r2, .L_shared_work_global
    ldr r2, [r2]
    add r0, r2, r0, lsl #1
    add r0, r0, #0x200
    strh r1, [r0, #0x60]
    bx lr
.L_shared_work_global:
    .word SNDi_SharedWork
.size SND_SetPlayerGlobalVariable, . - SND_SetPlayerGlobalVariable

.global SND_SetPlayerLocalVariable
.type SND_SetPlayerLocalVariable, %function
SND_SetPlayerLocalVariable:
    ldr r3, .L_shared_work_local
    ldr r12, [r3]
    mov r3, #0x24
    mla r3, r0, r3, r12
    add r0, r3, r1, lsl #1
    strh r2, [r0, #0x20]
    bx lr
.L_shared_work_local:
    .word SNDi_SharedWork
.size SND_SetPlayerLocalVariable, . - SND_SetPlayerLocalVariable
