/* NitroSDK Thumb supervisor-call wrapper. */

.syntax unified
.arch armv4t
.thumb
.section .pit_SVC_WaitByLoop, "ax", %progbits
.balign 2

.global SVC_WaitByLoop
.type SVC_WaitByLoop, %function
.thumb_func
SVC_WaitByLoop:
    svc #3
    bx lr

.size SVC_WaitByLoop, . - SVC_WaitByLoop
