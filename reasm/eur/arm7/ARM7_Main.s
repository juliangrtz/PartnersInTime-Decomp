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
.extern ARM7_MainLoopThumbThunk
.extern func_037FB7A8
.extern func_037FB860
.extern func_037FC188
.extern func_037FC7E8
.extern func_037FDC90
.extern func_037FDD48
.extern func_037FDE08
.extern func_037FE1C4
.extern func_037FE580
.extern func_037FF0CC
.extern func_03802ABC
.extern func_03802B48
.extern func_03803B48
.extern func_03804244
.extern func_03805ACC
.extern func_038069F8
ARM7_Main:
    push {r4, lr}
    bl func_037FC7E8
    bl func_037FC188
    bl func_037F8228
    bl func_037FE1C4
    bl func_037F83C0
    mov r4, r0
    mov r0, #6
    bl func_037FF0CC
    bl func_037FE580
    mov r0, #1
    ldr r1, .L_autoload_start
    bl func_037FB860
    mov r0, #1
    bl func_037FB7A8

    ldr r1, .L_dispstat
    ldrh r0, [r1]
    ldrh r0, [r1]
    orr r0, r0, #8
    strh r0, [r1]

    ldr r1, .L_ime
    ldrh r0, [r1]
    mov r0, #1
    strh r0, [r1]
    bl func_037FDC90
    mvn r0, #0
    bl func_03802ABC
    mov r0, #15
    bl func_03802B48
    mov r0, #12
    bl func_027F6124
    mov r0, r4
    bl func_03805ACC
    mov r0, #2
    bl func_03804244

.L_main_loop:
    bl ARM7_MainLoopThumbThunk
    bl func_037FDE08
    cmp r0, #0
    beq .L_skip_optional_work
    bl func_037FDD48
.L_skip_optional_work:
    bl func_038069F8
    bl func_03803B48
    b .L_main_loop

.L_autoload_start:
    .word ARM7_Autoload000_Start
.L_dispstat:
    .word REG_DISPSTAT
.L_ime:
    .word REG_IME

.size ARM7_Main, . - ARM7_Main
