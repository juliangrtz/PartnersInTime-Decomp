/* ARM entry veneer for the Thumb routine used once per ARM7 main-loop pass. */

.syntax unified
.arch armv4t
.arm
.section .pit_ARM7_MainLoopThumbThunk, "ax", %progbits
.balign 4

.global ARM7_MainLoopThumbThunk
.type ARM7_MainLoopThumbThunk, %function
.extern ARM7_MainLoopThumbTarget
ARM7_MainLoopThumbThunk:
    ldr r12, .L_thumb_target
    bx r12
.L_thumb_target:
    .word ARM7_MainLoopThumbTarget + 1

.size ARM7_MainLoopThumbThunk, . - ARM7_MainLoopThumbThunk
