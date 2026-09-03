/* Maintain LED blink-pattern selection and advance custom patterns. */

.syntax unified
.arch armv4t
.arm
.section .pit_PM_Blink, "ax", %progbits
.balign 4

.extern PMi_BlinkCounter
.extern PMi_BlinkPatternData
.extern PMi_BlinkPatternNo
.extern PMi_LEDStatus
.extern SPIi_SetEntry
.extern _u32_div_f

.global PM_GetLEDPattern
.type PM_GetLEDPattern, %function
PM_GetLEDPattern:
    ldr r0, .L_get_pattern_no
    ldr r0, [r0]
    bx lr
.L_get_pattern_no:
    .word PMi_BlinkPatternNo
.size PM_GetLEDPattern, . - PM_GetLEDPattern

.global PM_SetLEDPattern
.type PM_SetLEDPattern, %function
PM_SetLEDPattern:
    cmp r0, #15
    ldrle r1, .L_set_pattern_no
    strle r0, [r1]
    movle r1, #0
    ldrle r0, .L_set_blink_counter
    strle r1, [r0]
    bx lr
.L_set_pattern_no:
    .word PMi_BlinkPatternNo
.L_set_blink_counter:
    .word PMi_BlinkCounter
.size PM_SetLEDPattern, . - PM_SetLEDPattern

.global PM_SelfBlinkProc
.type PM_SelfBlinkProc, %function
PM_SelfBlinkProc:
    stmdb sp!, {r4-r6, lr}
    ldr r0, .L_blink_pattern_no
    ldr r3, [r0]
    cmp r3, #0
    bne .L_check_fixed_pattern
    mov r0, #3
    mov r1, #102
    mov r2, #1
    mov r3, r2
    bl SPIi_SetEntry
    cmp r0, #0
    beq .L_blink_done
    mov r0, #1
    bl PM_SetLEDPattern
    b .L_blink_done
.L_check_fixed_pattern:
    cmp r3, #4
    bge .L_custom_pattern
    ldr r0, .L_led_status
    ldr r0, [r0]
    cmp r3, r0
    beq .L_blink_done
    mov r0, #3
    mov r1, #102
    mov r2, #1
    bl SPIi_SetEntry
    b .L_blink_done
.L_custom_pattern:
    ldr r6, .L_blink_pattern_data
    sub r1, r3, #4
    mov r0, #12
    mul r5, r1, r0
    add r4, r6, r5
    ldr r0, .L_blink_counter
    ldr r0, [r0]
    ldrh r1, [r4, #10]
    bl _u32_div_f
    ldr r5, [r6, r5]
    ldr lr, [r4, #4]
    mov r3, #0
    mov r2, #0x80000000
    mov ip, r3, lsr r0
    rsb r1, r0, #32
    orr ip, ip, r2, lsl r1
    sub r1, r0, #32
    orr ip, ip, r2, lsr r1
    and r0, lr, r2, lsr r0
    and r1, r5, ip
    cmp r0, r3
    cmpeq r1, r3
    movne r3, #1
    moveq r3, #2
    ldr r0, .L_blink_counter
    ldr r1, [r0]
    add ip, r1, #1
    str ip, [r0]
    ldrh r2, [r4, #8]
    ldrh r1, [r4, #10]
    mul r1, r2, r1
    cmp ip, r1
    movhs r1, #0
    strhs r1, [r0]
    ldr r0, .L_led_status
    ldr r0, [r0]
    cmp r3, r0
    beq .L_blink_done
    mov r0, #3
    mov r1, #102
    mov r2, #1
    bl SPIi_SetEntry
.L_blink_done:
    ldmia sp!, {r4-r6, lr}
    bx lr
.L_blink_pattern_no:
    .word PMi_BlinkPatternNo
.L_led_status:
    .word PMi_LEDStatus
.L_blink_pattern_data:
    .word PMi_BlinkPatternData
.L_blink_counter:
    .word PMi_BlinkCounter
.size PM_SelfBlinkProc, . - PM_SelfBlinkProc
