/* Nestable NitroSDK scheduler suppression counter. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_SchedulerControl, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OSi_RescheduleCount

.global OS_EnableScheduler
.type OS_EnableScheduler, %function
OS_EnableScheduler:
    stmdb sp!, {r4, lr}
    bl OS_DisableInterrupts
    mov r4, #0
    ldr r1, .L_reschedule_count_enable
    ldr r3, [r1]
    cmp r3, #0
    subne r2, r3, #1
    strne r2, [r1]
    movne r4, r3
    bl OS_RestoreInterrupts
    mov r0, r4
    ldmia sp!, {r4, lr}
    bx lr
.L_reschedule_count_enable:
    .word OSi_RescheduleCount
.size OS_EnableScheduler, . - OS_EnableScheduler

.global OS_DisableScheduler
.type OS_DisableScheduler, %function
OS_DisableScheduler:
    stmdb sp!, {r4, lr}
    bl OS_DisableInterrupts
    ldr r2, .L_reschedule_count_disable
    ldr r3, [r2]
    mvn r1, #0
    cmp r3, r1
    addlo r1, r3, #1
    strlo r1, [r2]
    movlo r4, r3
    bl OS_RestoreInterrupts
    mov r0, r4
    ldmia sp!, {r4, lr}
    bx lr
.L_reschedule_count_disable:
    .word OSi_RescheduleCount
.size OS_DisableScheduler, . - OS_DisableScheduler
