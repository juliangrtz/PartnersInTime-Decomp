/* Enter ARM7 sleep, configure wake sources, and restore runtime state. */

.syntax unified
.arch armv4t
.arm
.section .pit_PM_Sleep, "ax", %progbits
.balign 4

.extern EXIi_SelectRcnt
.extern EXIi_SetBitRcnt0L
.extern OS_DisableInterrupts
.extern OS_DisableIrqMask
.extern OS_EnableIrqMask
.extern OS_RestoreInterrupts
.extern OS_SetIrqMask
.extern PM_SetLEDPattern
.extern PMi_GetRegister
.extern PMi_KeyPattern
.extern PMi_ResetControl
.extern PMi_SendPxiCommand
.extern PMi_SetControl
.extern PMi_SetLED
.extern PMi_SetRegister
.extern PMi_SwitchUtilityProc
.extern PMi_TriggerBL
.extern PMi_Work
.extern SND_BeginSleep
.extern SND_EndSleep
.extern SVC_Stop

.global PMi_DoSleep
.type PMi_DoSleep, %function
PMi_DoSleep:
    stmdb sp!, {r4-r8, lr}
    ldr r1, .L_ime
    ldrh r4, [r1]
    mov r0, #0
    strh r0, [r1]
    bl OS_DisableInterrupts
    mov r8, r0
    mvn r0, #0xfe000000
    bl OS_DisableIrqMask
    mov r7, r0
    mov r0, #0
    bl PMi_GetRegister
    mov r6, r0
    mov r0, #2
    bl PM_SetLEDPattern
    mov r0, #2
    bl PMi_SetLED
    mov r0, #2
    bl PMi_SetLED
    bl SND_BeginSleep
    mov r0, #1
    bl PMi_ResetControl
    ldr r0, .L_trigger_bl
    ldrh r0, [r0]
    ands r0, r0, #1
    beq .L_check_cartridge
    ldr r0, .L_key_pattern
    ldrh r0, [r0]
    orr r1, r0, #0x4000
    ldr r0, .L_keycnt
    strh r1, [r0]
    mov r0, #0x1000
    bl OS_EnableIrqMask
.L_check_cartridge:
    ldr r0, .L_trigger_bl
    ldrh r0, [r0]
    ands r0, r0, #4
    beq .L_check_hinge
    mov r0, #0x00400000
    bl OS_EnableIrqMask
.L_check_hinge:
    ldr r0, .L_trigger_bl
    ldrh r0, [r0]
    ands r0, r0, #2
    beq .L_check_card
    ldr r0, .L_rcnt
    ldrh r5, [r0]
    mov r0, #0x8000
    bl EXIi_SelectRcnt
    mov r0, #0x40
    mov r1, r0
    bl EXIi_SetBitRcnt0L
    mov r0, #0x100
    mov r1, r0
    bl EXIi_SetBitRcnt0L
    mov r0, #0x80
    bl OS_EnableIrqMask
.L_check_card:
    ldr r0, .L_trigger_bl
    ldrh r0, [r0]
    ands r0, r0, #8
    beq .L_check_spi
    mov r0, #0x00100000
    bl OS_EnableIrqMask
.L_check_spi:
    ldr r0, .L_trigger_bl
    ldrh r0, [r0]
    ands r0, r0, #16
    beq .L_enter_stop
    mov r0, #0x2000
    bl OS_EnableIrqMask
.L_enter_stop:
    mov r0, r8
    bl OS_RestoreInterrupts
    ldr r1, .L_ime
    ldrh r0, [r1]
    mov r0, #1
    strh r0, [r1]
    bl PMi_Stop
    mov r0, #0
    mov r1, r6
    bl PMi_SetRegister
    ldr r0, .L_trigger_bl
    ldrh r1, [r0]
    ands r0, r1, #32
    movne r0, #6
    moveq r0, #7
    ands r1, r1, #64
    movne r6, #4
    moveq r6, #5
    bl PMi_SwitchUtilityProc
    mov r0, r6
    bl PMi_SwitchUtilityProc
    ldr r0, .L_rcnt
    strh r5, [r0]
    mov r0, #1
    bl PMi_SetControl
    bl SND_EndSleep
    mov r1, #0
    ldr r0, .L_pm_work
    str r1, [r0, #32]
    mov r0, #98
    mov r2, r1
    bl PMi_SendPxiCommand
    bl OS_DisableInterrupts
    mov r0, r7
    bl OS_SetIrqMask
    mov r0, r8
    bl OS_RestoreInterrupts
    ldr r1, .L_ime
    ldrh r0, [r1]
    strh r4, [r1]
    ldmia sp!, {r4-r8, lr}
    bx lr
.L_ime:
    .word 0x04000208
.L_trigger_bl:
    .word PMi_TriggerBL
.L_key_pattern:
    .word PMi_KeyPattern
.L_keycnt:
    .word 0x04000132
.L_rcnt:
    .word 0x04000134
.L_pm_work:
    .word PMi_Work
.size PMi_DoSleep, . - PMi_DoSleep

.global PMi_Stop
.type PMi_Stop, %function
PMi_Stop:
    ldr ip, .L_svc_stop
    bx ip
.L_svc_stop:
    /* ELF marks the external Thumb function odd as a callable pointer. */
    .word SVC_Stop
.size PMi_Stop, . - PMi_Stop
