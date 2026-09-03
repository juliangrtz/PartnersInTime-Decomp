/* Execute queued power-management commands under the SPI exception lock. */

.syntax unified
.arch armv4t
.arm
.section .pit_PM_ExecuteProcess, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern PMi_DoSleep
.extern PMi_GetRegister
.extern PMi_KeyPattern
.extern PMi_SetLED
.extern PMi_SetRegister
.extern PMi_SwitchUtilityProc
.extern PMi_TriggerBL
.extern PMi_Work
.extern SPIi_CheckException
.extern SPIi_GetException
.extern SPIi_ReleaseException
.extern SPIi_ReturnResult

.global PM_ExecuteProcess
.type PM_ExecuteProcess, %function
PM_ExecuteProcess:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    bl OS_DisableInterrupts
    mov r5, r0
    mov r0, #3
    bl SPIi_CheckException
    cmp r0, #0
    bne .L_acquire_exception
    mov r0, r5
    bl OS_RestoreInterrupts
    ldr r0, [r4, #4]
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_done
.L_acquire_exception:
    mov r0, #3
    bl SPIi_GetException
    mov r0, r5
    bl OS_RestoreInterrupts
    ldr r1, [r4, #4]
    sub r0, r1, #97
    cmp r0, #5
    addls pc, pc, r0, lsl #2
    b .L_bad_command
    b .L_sleep
    b .L_bad_command
    b .L_utility
    b .L_set_register
    b .L_get_register
    b .L_set_led
.L_sleep:
    mov r1, #1
    ldr r0, .L_pm_work
    str r1, [r0, #32]
    ldr r1, [r4, #8]
    ldr r0, .L_trigger_bl
    strh r1, [r0]
    ldr r1, [r4, #12]
    ldr r0, .L_key_pattern
    strh r1, [r0]
    bl PMi_DoSleep
    b .L_release_exception
.L_set_register:
    mov r1, #4
    ldr r0, .L_pm_work
    str r1, [r0, #32]
    ldr r2, [r4, #8]
    str r2, [r0, #40]
    ldr r1, [r4, #12]
    str r1, [r0, #36]
    mov r0, r2, lsl #16
    mov r0, r0, lsr #16
    and r1, r1, #0xff
    bl PMi_SetRegister
    mov r0, #100
    mov r1, #0
    bl SPIi_ReturnResult
    b .L_release_exception
.L_get_register:
    mov r1, #3
    ldr r0, .L_pm_work
    str r1, [r0, #32]
    ldr r1, [r4, #8]
    str r1, [r0, #40]
    mov r0, r1, lsl #16
    mov r4, r0, lsr #16
    mov r0, r4
    bl PMi_GetRegister
    mov r1, r0
    add r0, r4, #112
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    bl SPIi_ReturnResult
    b .L_release_exception
.L_utility:
    mov r0, #2
    ldr r1, .L_pm_work
    str r0, [r1, #32]
    ldr r0, [r4, #8]
    str r0, [r1, #36]
    bl PMi_SwitchUtilityProc
    mov r0, #99
    mov r1, #0
    bl SPIi_ReturnResult
    b .L_release_exception
.L_set_led:
    ldr r0, [r4, #8]
    bl PMi_SetLED
    b .L_release_exception
.L_bad_command:
    mov r0, r1, lsl #16
    mov r0, r0, lsr #16
    mov r1, #1
    bl SPIi_ReturnResult
.L_release_exception:
    mov r0, #3
    bl SPIi_ReleaseException
.L_done:
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_pm_work:
    .word PMi_Work
.L_trigger_bl:
    .word PMi_TriggerBL
.L_key_pattern:
    .word PMi_KeyPattern
.size PM_ExecuteProcess, . - PM_ExecuteProcess
