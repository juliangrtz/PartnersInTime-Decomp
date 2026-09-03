/* Parse touch-panel PXI command words and enqueue valid operations. */

.syntax unified
.arch armv4t
.arm
.section .pit_TP_AnalyzeCommand, "ax", %progbits
.balign 4

.extern SPIi_ReturnResult
.extern SPIi_SetEntry
.extern SPIi_SetTouchPanelCallbackValue
.extern TPi_Work

.global TP_AnalyzeCommand
.type TP_AnalyzeCommand, %function
TP_AnalyzeCommand:
    stmdb sp!, {r4, lr}
    sub sp, sp, #8
    ands r1, r0, #0x02000000
    beq .L_store_fragment
    mov r4, #0
    mov r3, r4
    ldr r1, .L_touch_work
.L_clear_fragments:
    mov r2, r4, lsl #1
    strh r3, [r1, r2]
    add r4, r4, #1
    cmp r4, #16
    blt .L_clear_fragments
.L_store_fragment:
    and r1, r0, #0x000f0000
    mov r1, r1, lsr #16
    mov r1, r1, lsl #1
    ldr r2, .L_touch_work
    strh r0, [r2, r1]
    ands r0, r0, #0x01000000
    beq .L_analyze_done
    ldrh r1, [r2]
    and r0, r1, #0xff00
    mov r0, r0, lsl #8
    mov r4, r0, lsr #16
    cmp r4, #3
    addls pc, pc, r4, lsl #2
    b .L_unknown_command
    b .L_sample_once
    b .L_start_auto_sampling
    b .L_stop_auto_sampling
    b .L_set_callback_value
.L_set_callback_value:
    and r0, r1, #0xff
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    bl SPIi_SetTouchPanelCallbackValue
    b .L_analyze_done
.L_sample_once:
    mov r0, #0
    mov r1, r4
    mov r2, r0
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_analyze_done
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_analyze_done
.L_start_auto_sampling:
    ldr r0, [r2, #32]
    cmp r0, #0
    beq .L_validate_auto_sampling
    mov r0, r4
    mov r1, #3
    bl SPIi_ReturnResult
    b .L_analyze_done
.L_validate_auto_sampling:
    and r0, r1, #0xff
    mov r0, r0, lsl #16
    movs r3, r0, lsr #16
    beq .L_invalid_auto_sampling
    cmp r3, #4
    bls .L_check_auto_interval
.L_invalid_auto_sampling:
    mov r0, r4
    mov r1, #2
    bl SPIi_ReturnResult
    b .L_analyze_done
.L_check_auto_interval:
    ldrh r1, [r2, #2]
    ldr r0, .L_vblank_ratio
    cmp r1, r0
    bcc .L_queue_auto_start
    mov r0, r4
    mov r1, #2
    bl SPIi_ReturnResult
    b .L_analyze_done
.L_queue_auto_start:
    str r1, [sp]
    mov r0, #0
    mov r1, r4
    mov r2, #2
    bl SPIi_SetEntry
    cmp r0, #0
    movne r1, #1
    ldrne r0, .L_touch_work
    strne r1, [r0, #32]
    bne .L_analyze_done
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_analyze_done
.L_stop_auto_sampling:
    ldr r0, [r2, #32]
    cmp r0, #2
    beq .L_queue_auto_stop
    mov r0, r4
    mov r1, #3
    bl SPIi_ReturnResult
    b .L_analyze_done
.L_queue_auto_stop:
    mov r0, #0
    mov r1, r4
    mov r2, r0
    bl SPIi_SetEntry
    cmp r0, #0
    movne r1, #3
    ldrne r0, .L_touch_work
    strne r1, [r0, #32]
    bne .L_analyze_done
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_analyze_done
.L_unknown_command:
    mov r0, r4
    mov r1, #1
    bl SPIi_ReturnResult
.L_analyze_done:
    add sp, sp, #8
    ldmia sp!, {r4, lr}
    bx lr
.L_touch_work:
    .word TPi_Work
.L_vblank_ratio:
    .word 0x00000107
.size TP_AnalyzeCommand, . - TP_AnalyzeCommand
