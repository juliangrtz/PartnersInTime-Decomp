/* ARM7 runtime initialization and cooperative main loop. */

.syntax unified
.arch armv4t
.arm
.section .pit_ARM7_Main, "ax", %progbits
.balign 4

.global ARM7_Main
.type ARM7_Main, %function
.extern ARM7_Autoload000_Start
.extern REG_DISPSTAT
.extern REG_IME
.extern func_027F6124
.extern func_037F8228
.extern func_037F83C0
.extern SVC_Halt_Thunk
.extern OS_EnableIrqMask
.extern OS_SetIrqFunction
.extern OS_InitThread
.extern OS_Init
.extern OS_EnableInterrupts
.extern OS_IsResetOccurred
.extern OS_ResetSystem
.extern PXI_Init
.extern PAD_InitXYButton
.extern func_037FF0CC
.extern FS_Init
.extern CARD_SetThreadPriority
.extern CARD_CheckPullOut_Polling
.extern SPI_Init
.extern WVR_Init
.extern func_038069F8
ARM7_Main:
    push {r4, lr}
    bl OS_Init
    bl OS_InitThread
    bl func_037F8228
    bl PXI_Init
    bl func_037F83C0
    mov r4, r0
    mov r0, #6
    bl func_037FF0CC
    bl PAD_InitXYButton
    mov r0, #1
    ldr r1, .L_autoload_start
    bl OS_SetIrqFunction
    mov r0, #1
    bl OS_EnableIrqMask

    ldr r1, .L_dispstat
    ldrh r0, [r1]
    ldrh r0, [r1]
    orr r0, r0, #8
    strh r0, [r1]

    ldr r1, .L_ime
    ldrh r0, [r1]
    mov r0, #1
    strh r0, [r1]
    bl OS_EnableInterrupts
    mvn r0, #0
    bl FS_Init
    mov r0, #15
    bl CARD_SetThreadPriority
    mov r0, #12
    bl func_027F6124
    mov r0, r4
    bl WVR_Init
    mov r0, #2
    bl SPI_Init

.L_main_loop:
    bl SVC_Halt_Thunk
    bl OS_IsResetOccurred
    cmp r0, #0
    beq .L_skip_optional_work
    bl OS_ResetSystem
.L_skip_optional_work:
    bl func_038069F8
    bl CARD_CheckPullOut_Polling
    b .L_main_loop

.L_autoload_start:
    .word ARM7_Autoload000_Start
.L_dispstat:
    .word REG_DISPSTAT
.L_ime:
    .word REG_IME

.size ARM7_Main, . - ARM7_Main
