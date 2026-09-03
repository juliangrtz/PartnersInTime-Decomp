/* Apply LED states and power-management utility operations. */

.syntax unified
.arch armv4t
.arm
.section .pit_PM_Utility, "ax", %progbits
.balign 4

.extern OS_Terminate
.extern PM_SetLEDPattern
.extern PMi_LEDStatus
.extern PMi_ResetControl
.extern PMi_SetControl
.extern SND_BeginSleep

.global PMi_SetLED
.type PMi_SetLED, %function
PMi_SetLED:
    stmdb sp!, {r4, lr}
    mov r4, r0
    cmp r4, #1
    beq .L_led_on
    cmp r4, #2
    beq .L_led_blink
    cmp r4, #3
    beq .L_led_fast_blink
    b .L_invalid_led
.L_led_on:
    mov r0, #16
    bl PMi_ResetControl
    b .L_store_led
.L_led_fast_blink:
    mov r0, #48
    bl PMi_SetControl
    b .L_store_led
.L_led_blink:
    mov r0, #32
    bl PMi_ResetControl
    mov r0, #16
    bl PMi_SetControl
    b .L_store_led
.L_invalid_led:
    bl OS_Terminate
.L_store_led:
    ldr r0, .L_led_status
    str r4, [r0]
    ldmia sp!, {r4, lr}
    bx lr
.L_led_status:
    .word PMi_LEDStatus
.size PMi_SetLED, . - PMi_SetLED

.global PMi_SwitchUtilityProc
.type PMi_SwitchUtilityProc, %function
PMi_SwitchUtilityProc:
    stmdb sp!, {lr}
    sub sp, sp, #4
    cmp r0, #15
    addls pc, pc, r0, lsl #2
    b .L_utility_done
    b .L_utility_done
    b .L_utility_led_on
    b .L_utility_led_fast_blink
    b .L_utility_led_blink
    b .L_utility_set_sound
    b .L_utility_reset_sound
    b .L_utility_set_amp
    b .L_utility_reset_amp
    b .L_utility_set_both
    b .L_utility_reset_both
    b .L_utility_set_lcd
    b .L_utility_reset_lcd
    b .L_utility_set_power2
    b .L_utility_reset_power2
    b .L_utility_sleep_start
    b .L_utility_reset_power64
.L_utility_led_on:
    mov r0, #1
    bl PM_SetLEDPattern
    mov r0, #1
    bl PMi_SetLED
    b .L_utility_done
.L_utility_led_fast_blink:
    mov r0, #3
    bl PM_SetLEDPattern
    mov r0, #3
    bl PMi_SetLED
    b .L_utility_done
.L_utility_led_blink:
    mov r0, #2
    bl PM_SetLEDPattern
    mov r0, #2
    bl PMi_SetLED
    b .L_utility_done
.L_utility_set_sound:
    mov r0, #4
    bl PMi_SetControl
    b .L_utility_done
.L_utility_reset_sound:
    mov r0, #4
    bl PMi_ResetControl
    b .L_utility_done
.L_utility_set_amp:
    mov r0, #8
    bl PMi_SetControl
    b .L_utility_done
.L_utility_reset_amp:
    mov r0, #8
    bl PMi_ResetControl
    b .L_utility_done
.L_utility_set_both:
    mov r0, #12
    bl PMi_SetControl
    b .L_utility_done
.L_utility_reset_both:
    mov r0, #12
    bl PMi_ResetControl
    b .L_utility_done
.L_utility_set_lcd:
    mov r0, #1
    bl PMi_SetControl
    b .L_utility_done
.L_utility_reset_lcd:
    mov r0, #1
    bl PMi_ResetControl
    b .L_utility_done
.L_utility_set_power2:
    mov r0, #2
    bl PMi_SetControl
    b .L_utility_done
.L_utility_reset_power2:
    mov r0, #2
    bl PMi_ResetControl
    b .L_utility_done
.L_utility_reset_power64:
    mov r0, #64
    bl PMi_ResetControl
    b .L_utility_done
.L_utility_sleep_start:
    bl SND_BeginSleep
    mov r0, #1
    bl PMi_ResetControl
    mov r0, #64
    bl PMi_SetControl
.L_utility_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size PMi_SwitchUtilityProc, . - PMi_SwitchUtilityProc
