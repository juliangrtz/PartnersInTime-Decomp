/* Copy and clear every descriptor in the NitroSDK ARM7 autoload table. */

.syntax unified
.arch armv4t
.arm
.section .pit_ARM7_LoadAutoloads, "ax", %progbits
.balign 4

.global ARM7_LoadAutoloads
.type ARM7_LoadAutoloads, %function
.extern ARM7_AutoloadDone
.extern ARM7_ModuleParams
ARM7_LoadAutoloads:
    ldr r0, .L_module_params
    ldr r1, [r0]
    ldr r2, [r0, #4]
    ldr r3, [r0, #8]
.Ldescriptor:
    cmp r1, r2
    beq .Ldone
    ldr r4, [r1], #4
    ldr r5, [r1], #4
    add r6, r4, r5
.Lcopy:
    cmp r4, r6
    ldrmi r7, [r3], #4
    strmi r7, [r4], #4
    bmi .Lcopy

    ldr r5, [r1], #4
    add r6, r4, r5
    mov r7, #0
.Lclear:
    cmp r4, r6
    strlo r7, [r4], #4
    blo .Lclear
    beq .Ldescriptor

.Ldone:
    b ARM7_AutoloadDone

.L_module_params:
    .word ARM7_ModuleParams

.size ARM7_LoadAutoloads, . - ARM7_LoadAutoloads
