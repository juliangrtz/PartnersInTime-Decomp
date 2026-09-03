/* Detect cartridge removal, notify ARM9, and terminate safely when requested. */

.syntax unified
.arch armv4t
.arm
.section .pit_CARD_PullOut, "ax", %progbits
.balign 4

.extern CARDi_BootMode
.extern CARDi_CardDetectMode
.extern CARDi_IsPulledOutResult
.extern CARDi_NextPollVBlank
.extern CARDi_PullOutCallbackInitialized
.extern CARDi_PullOutMessagePending
.extern CARDi_PulledOut
.extern CARDi_RomIdPrimary
.extern CARDi_RomIdSourceSelect
.extern HW_MAIN_MEM_SYSTEM
.extern HW_VBLANK_COUNT_BUF
.extern OS_Terminate
.extern PXI_Init
.extern PXI_IsCallbackReady
.extern PXI_SendWordByFifo
.extern PXI_SetFifoRecvCallback
.extern REG_IF
.extern SND_BeginSleep
.extern SVC_WaitByLoop_Thunk
.extern WVR_Shutdown

.global CARDi_SendPxi
.type CARDi_SendPxi, %function
CARDi_SendPxi:
    stmdb sp!, {r4-r7, lr}
    sub sp, sp, #4
    mov r7, r0
    mov r6, r1
    mov r5, #0xe
    mov r4, #0
    b .L_send_try
.L_send_wait:
    mov r0, r6
    bl SVC_WaitByLoop_Thunk
.L_send_try:
    mov r0, r5
    mov r1, r7
    mov r2, r4
    bl PXI_SendWordByFifo
    cmp r0, #0
    bne .L_send_wait
    add sp, sp, #4
    ldmia sp!, {r4-r7, lr}
    bx lr
.size CARDi_SendPxi, . - CARDi_SendPxi

.global CARDi_TerminateForPullOut
.type CARDi_TerminateForPullOut, %function
CARDi_TerminateForPullOut:
    stmdb sp!, {lr}
    sub sp, sp, #4
    bl SND_BeginSleep
    bl WVR_Shutdown
    bl OS_Terminate
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size CARDi_TerminateForPullOut, . - CARDi_TerminateForPullOut

.global CARD_CheckPullOut_Polling
.type CARD_CheckPullOut_Polling, %function
CARD_CheckPullOut_Polling:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_pulled_out
    ldr r0, [r0]
    cmp r0, #0
    bne .L_poll_done
    ldr r0, .L_boot_mode
    ldrh r0, [r0]
    cmp r0, #2
    beq .L_poll_done
    ldr r1, .L_next_poll
    ldr r3, [r1]
    mvn r0, #0
    cmp r3, r0
    ldreq r0, .L_vblank_count
    ldreq r0, [r0]
    addeq r0, r0, #10
    streq r0, [r1]
    beq .L_poll_done
    ldr r2, .L_vblank_count
    ldr r0, [r2]
    cmp r0, r3
    blo .L_poll_done
    ldr r0, [r2]
    add r0, r0, #10
    str r0, [r1]
    bl CARD_IsPulledOut
    cmp r0, #0
    beq .L_clear_pending
    mov r1, #1
    ldr r0, .L_pulled_out
    str r1, [r0]
    ldr r0, .L_message_pending
    ldr r0, [r0]
    cmp r0, #0
    bne .L_poll_done
.L_clear_pending:
    mov r1, #0
    ldr r0, .L_message_pending
    str r1, [r0]
    ldr r0, .L_pulled_out
    ldr r0, [r0]
    cmp r0, #0
    beq .L_poll_done
    mov r0, #0x11
    mov r1, #100
    bl CARDi_SendPxi
.L_poll_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_pulled_out:
    .word CARDi_PulledOut
.L_boot_mode:
    .word CARDi_BootMode
.L_next_poll:
    .word CARDi_NextPollVBlank
.L_vblank_count:
    .word HW_VBLANK_COUNT_BUF
.L_message_pending:
    .word CARDi_PullOutMessagePending
.size CARD_CheckPullOut_Polling, . - CARD_CheckPullOut_Polling

.global CARD_IsCardIreqLo
.type CARD_IsCardIreqLo, %function
CARD_IsCardIreqLo:
    mov r2, #1
    mov r0, r2
    ldr r1, .L_interrupt_flags
    ldr r1, [r1]
    ands r1, r1, #0x00100000
    movne r0, #0
    ldrne r1, .L_pulled_out_result
    strne r2, [r1]
    bx lr
.L_interrupt_flags:
    .word REG_IF
.L_pulled_out_result:
    .word CARDi_IsPulledOutResult
.size CARD_IsCardIreqLo, . - CARD_IsCardIreqLo

.global CARD_CompareCardID
.type CARD_CompareCardID, %function
CARD_CompareCardID:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_rom_id_select
    ldrh r0, [r0]
    cmp r0, #0
    ldreq r0, .L_primary_rom_id
    ldrne r0, .L_alternate_rom_id
    ldr r0, [r0]
    str r0, [sp]
    bl CARDi_ReadRomID
    ldr r1, [sp]
    cmp r0, r1
    moveq r0, #1
    movne r0, #0
    cmp r0, #0
    moveq r2, #1
    movne r2, #0
    ldr r1, .L_compare_result
    str r2, [r1]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_rom_id_select:
    .word CARDi_RomIdSourceSelect
.L_primary_rom_id:
    .word CARDi_RomIdPrimary
.L_alternate_rom_id:
    .word HW_MAIN_MEM_SYSTEM
.L_compare_result:
    .word CARDi_IsPulledOutResult
.size CARD_CompareCardID, . - CARD_CompareCardID

.global CARD_IsPulledOut
.type CARD_IsPulledOut, %function
CARD_IsPulledOut:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, .L_is_pulled_result
    ldr r0, [r0]
    cmp r0, #0
    bne .L_read_pulled_result
    ldr r0, .L_card_detect_mode
    ldrb r0, [r0]
    ands r0, r0, #1
    beq .L_check_card_irq
    bl CARD_CompareCardID
    b .L_read_pulled_result
.L_check_card_irq:
    bl CARD_IsCardIreqLo
.L_read_pulled_result:
    ldr r0, .L_is_pulled_result
    ldr r0, [r0]
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_is_pulled_result:
    .word CARDi_IsPulledOutResult
.L_card_detect_mode:
    .word CARDi_CardDetectMode
.size CARD_IsPulledOut, . - CARD_IsPulledOut

.global CARDi_PulledOutCallback
.type CARDi_PulledOutCallback, %function
CARDi_PulledOutCallback:
    stmdb sp!, {lr}
    sub sp, sp, #4
    and r0, r1, #0x3f
    cmp r0, #1
    bne .L_pullout_terminate
    bl CARDi_TerminateForPullOut
    b .L_pullout_callback_done
.L_pullout_terminate:
    bl OS_Terminate
.L_pullout_callback_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size CARDi_PulledOutCallback, . - CARDi_PulledOutCallback

.global CARD_InitPulledOutCallback
.type CARD_InitPulledOutCallback, %function
CARD_InitPulledOutCallback:
    stmdb sp!, {r4, r5, lr}
    sub sp, sp, #4
    ldr r0, .L_pullout_initialized
    ldr r1, [r0]
    cmp r1, #0
    bne .L_pullout_init_done
    mov r1, #1
    str r1, [r0]
    bl PXI_Init
    mov r5, #0xe
    mov r4, #0
.L_wait_for_pullout_callback:
    mov r0, r5
    mov r1, r4
    bl PXI_IsCallbackReady
    cmp r0, #0
    beq .L_wait_for_pullout_callback
    mov r0, #0xe
    ldr r1, .L_pullout_callback
    bl PXI_SetFifoRecvCallback
.L_pullout_init_done:
    add sp, sp, #4
    ldmia sp!, {r4, r5, lr}
    bx lr
.L_pullout_initialized:
    .word CARDi_PullOutCallbackInitialized
.L_pullout_callback:
    .word CARDi_PulledOutCallback
.size CARD_InitPulledOutCallback, . - CARD_InitPulledOutCallback
