/* Initialize and access the three ARM7 NitroSDK memory arenas. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Arena, "ax", %progbits
.balign 4

.extern ARM7_Autoload000_End
.extern ARM7_Autoload001_End
.extern HW_ARM7SystemStack
.extern OSi_ArenaInitialized

.global OS_SetArenaLo
.type OS_SetArenaLo, %function
OS_SetArenaLo:
    mov r0, r0, lsl #2
    add r0, r0, #0x02700000
    add r0, r0, #0x000FF000
    str r1, [r0, #0xDA0]
    bx lr
.size OS_SetArenaLo, . - OS_SetArenaLo

.global OS_SetArenaHi
.type OS_SetArenaHi, %function
OS_SetArenaHi:
    mov r0, r0, lsl #2
    add r0, r0, #0x02700000
    add r0, r0, #0x000FF000
    str r1, [r0, #0xDC4]
    bx lr
.size OS_SetArenaHi, . - OS_SetArenaHi

.global OSi_GetInitArenaLo
.type OSi_GetInitArenaLo, %function
OSi_GetInitArenaLo:
    cmp r0, #1
    beq .L_init_lo_subpriv
    cmp r0, #7
    beq .L_init_lo_wram_sub
    cmp r0, #8
    beq .L_init_lo_wram_subpriv
    b .L_init_lo_none
.L_init_lo_subpriv:
    ldr r0, .L_autoload001_end
    bx lr
.L_init_lo_wram_sub:
    ldr r0, .L_autoload000_end_lo
    cmp r0, #0x03800000
    movhi r0, #0x03800000
    bx lr
.L_init_lo_wram_subpriv:
    mov r0, #0x03800000
    ldr r1, .L_autoload000_end_lo
    cmp r1, #0x03800000
    movhi r0, r1
    bx lr
.L_init_lo_none:
    mov r0, #0
    bx lr
.L_autoload001_end:
    .word ARM7_Autoload001_End
.L_autoload000_end_lo:
    .word ARM7_Autoload000_End
.size OSi_GetInitArenaLo, . - OSi_GetInitArenaLo

.global OSi_GetInitArenaHi
.type OSi_GetInitArenaHi, %function
OSi_GetInitArenaHi:
    cmp r0, #1
    beq .L_init_hi_subpriv
    cmp r0, #7
    beq .L_init_hi_wram_sub
    cmp r0, #8
    beq .L_init_hi_wram_subpriv
    b .L_init_hi_none
.L_init_hi_subpriv:
    ldr r0, .L_subpriv_end
    bx lr
.L_init_hi_wram_sub:
    mov r0, #0x03800000
    bx lr
.L_init_hi_wram_subpriv:
    ldr r1, .L_system_stack_size
    ldr r0, .L_system_stack
    sub r1, r0, r1
    ldr r0, .L_irq_stack_size
    cmp r0, #0
    moveq r0, #0x03800000
    bxeq lr
    cmp r0, #0
    rsblt r0, r0, #0x03800000
    subge r0, r1, r0
    bx lr
.L_init_hi_none:
    mov r0, #0
    bx lr
.L_subpriv_end:
    .word 0x027FF000
.L_system_stack_size:
    .word 0x400
.L_system_stack:
    .word HW_ARM7SystemStack
.L_irq_stack_size:
    .word 0x400
.size OSi_GetInitArenaHi, . - OSi_GetInitArenaHi

.global OS_GetArenaLo
.type OS_GetArenaLo, %function
OS_GetArenaLo:
    mov r0, r0, lsl #2
    add r0, r0, #0x02700000
    add r0, r0, #0x000FF000
    ldr r0, [r0, #0xDA0]
    bx lr
.size OS_GetArenaLo, . - OS_GetArenaLo

.global OS_GetArenaHi
.type OS_GetArenaHi, %function
OS_GetArenaHi:
    mov r0, r0, lsl #2
    add r0, r0, #0x02700000
    add r0, r0, #0x000FF000
    ldr r0, [r0, #0xDC4]
    bx lr
.size OS_GetArenaHi, . - OS_GetArenaHi

.global OS_InitArena
.type OS_InitArena, %function
OS_InitArena:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_arena_initialized
    ldr r0, [r1]
    cmp r0, #0
    bne .L_init_done
    mov r0, #1
    str r0, [r1]

    bl OSi_GetInitArenaHi
    mov r1, r0
    mov r0, #1
    bl OS_SetArenaHi
    mov r0, #1
    bl OSi_GetInitArenaLo
    mov r1, r0
    mov r0, #1
    bl OS_SetArenaLo

    mov r0, #7
    bl OSi_GetInitArenaHi
    mov r1, r0
    mov r0, #7
    bl OS_SetArenaHi
    mov r0, #7
    bl OSi_GetInitArenaLo
    mov r1, r0
    mov r0, #7
    bl OS_SetArenaLo

    mov r0, #8
    bl OSi_GetInitArenaHi
    mov r1, r0
    mov r0, #8
    bl OS_SetArenaHi
    mov r0, #8
    bl OSi_GetInitArenaLo
    mov r1, r0
    mov r0, #8
    bl OS_SetArenaLo

.L_init_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_arena_initialized:
    .word OSi_ArenaInitialized
.size OS_InitArena, . - OS_InitArena
