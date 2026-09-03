/* Reassemble and dispatch power-management commands received over PXI. */

.syntax unified
.arch armv4t
.arm
.section .pit_PM_AnalyzeCommand, "ax", %progbits
.balign 4

.extern PM_GetLEDPattern
.extern PM_SetLEDPattern
.extern PMi_Work
.extern SPIi_ReturnResult
.extern SPIi_SetEntry

.global PM_AnalyzeCommand
.type PM_AnalyzeCommand, %function
PM_AnalyzeCommand:
    stmdb sp!, {r4, lr}
    sub sp, sp, #8
    ands r1, r0, #0x02000000
    beq .L_store_fragment
    mov r4, #0
    mov r3, r4
    ldr r1, .L_pm_work
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
    ldr ip, .L_pm_work
    strh r0, [ip, r1]
    ands r0, r0, #0x01000000
    beq .L_done
    ldrh r3, [ip]
    and r0, r3, #0xff00
    mov r0, r0, lsl #8
    mov r4, r0, lsr #16
    sub r0, r4, #96
    cmp r0, #7
    addls pc, pc, r0, lsl #2
    b .L_bad_command
    b .L_ping
    b .L_sleep
    b .L_bad_command
    b .L_utility
    b .L_set_register
    b .L_get_register
    b .L_set_led_pattern
    b .L_get_led_pattern
.L_ping:
    mov r0, #96
    mov r1, #0
    bl SPIi_ReturnResult
    b .L_done
.L_sleep:
    ldrh r1, [ip, #2]
    ldr r0, .L_halfword_mask
    and r0, r1, r0
    str r0, [sp]
    mov r0, #3
    mov r1, r4
    mov r2, #2
    and r3, r3, #0xff
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_done
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_done
.L_set_register:
    ldrh r1, [ip, #2]
    ldr r0, .L_halfword_mask
    and r0, r1, r0
    str r0, [sp]
    mov r0, #3
    mov r1, r4
    mov r2, #2
    and r3, r3, #0xff
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_done
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_done
.L_get_register:
    mov r0, #3
    mov r1, r4
    mov r2, #1
    ldr ip, .L_halfword_mask
    and r3, r3, ip
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_done
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_done
.L_utility:
    mov r0, #3
    mov r1, r4
    mov r2, #1
    and lr, r3, #0xff
    ldrh ip, [ip, #2]
    ldr r3, .L_halfword_mask
    and r3, ip, r3
    orr r3, r3, lr, lsl #16
    bl SPIi_SetEntry
    cmp r0, #0
    bne .L_done
    mov r0, r4
    mov r1, #4
    bl SPIi_ReturnResult
    b .L_done
.L_set_led_pattern:
    and r0, r3, #0xff
    bl PM_SetLEDPattern
    mov r0, #102
    mov r1, #0
    bl SPIi_ReturnResult
    b .L_done
.L_get_led_pattern:
    bl PM_GetLEDPattern
    mov r1, r0
    mov r0, #103
    mov r1, r1, lsl #16
    mov r1, r1, lsr #16
    bl SPIi_ReturnResult
    b .L_done
.L_bad_command:
    mov r0, r4
    mov r1, #1
    bl SPIi_ReturnResult
.L_done:
    add sp, sp, #8
    ldmia sp!, {r4, lr}
    bx lr
.L_pm_work:
    .word PMi_Work
.L_halfword_mask:
    .word 0x0000ffff
.size PM_AnalyzeCommand, . - PM_AnalyzeCommand
