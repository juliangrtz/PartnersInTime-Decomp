/* NitroSDK ARM7 autoload-completion hook from the NDS header. */

.syntax unified
.arch armv4t
.arm
.section .pit_ARM7_AutoloadDone, "ax", %progbits
.balign 4

.global ARM7_AutoloadDone
.type ARM7_AutoloadDone, %function
ARM7_AutoloadDone:
    bx lr

.size ARM7_AutoloadDone, . - ARM7_AutoloadDone
