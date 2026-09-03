/* NitroSDK Thumb supervisor-call wrapper. */

.syntax unified
.arch armv4t
.thumb
.section .pit_SVC_Halt, "ax", %progbits
.balign 2

.global SVC_Halt
.type SVC_Halt, %function
.thumb_func
SVC_Halt:
    svc #6
    bx lr

.size SVC_Halt, . - SVC_Halt
