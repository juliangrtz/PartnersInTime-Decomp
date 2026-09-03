/* ARM interworking veneer for the Thumb SVC_Halt function. */

.syntax unified
.arch armv4t
.arm
.section .pit_SVC_Halt_Thunk, "ax", %progbits
.balign 4

.global SVC_Halt_Thunk
.type SVC_Halt_Thunk, %function
.extern SVC_Halt
SVC_Halt_Thunk:
    ldr r12, .L_thumb_target
    bx r12
.L_thumb_target:
    /* ELF marks .thumb_func symbols odd when used as callable pointers. */
    .word SVC_Halt

.size SVC_Halt_Thunk, . - SVC_Halt_Thunk
