/* NitroSDK ARM7 static-module parameters consumed by resident startup. */

.syntax unified
.arch armv4t
.section .pit_ARM7_ModuleParams, "a", %progbits
.balign 4

.global ARM7_ModuleParams
.type ARM7_ModuleParams, %object
.extern ARM7_AutoloadDataStart
.extern ARM7_AutoloadListEnd
.extern ARM7_AutoloadListStart
ARM7_ModuleParams:
    .word ARM7_AutoloadListStart
    .word ARM7_AutoloadListEnd
    .word ARM7_AutoloadDataStart
    .word ARM7_AutoloadDataStart
    .word ARM7_AutoloadDataStart
    .word 0

.size ARM7_ModuleParams, . - ARM7_ModuleParams
