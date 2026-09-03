/* NitroSDK Thumb supervisor-call wrapper. */

.syntax unified
.arch armv4t
.thumb
.section .pit_SVC_SoftReset, "ax", %progbits
.balign 2

.global SVC_SoftReset
.type SVC_SoftReset, %function
.thumb_func
SVC_SoftReset:
    svc #0
    bx lr

.size SVC_SoftReset, . - SVC_SoftReset
