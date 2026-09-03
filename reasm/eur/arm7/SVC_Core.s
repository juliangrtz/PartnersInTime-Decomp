/* NitroSDK Thumb supervisor-call wrappers. */

.syntax unified
.arch armv4t
.thumb
.section .pit_SVC_Core, "ax", %progbits
.balign 2

.global SVC_Stop
.type SVC_Stop, %function
.thumb_func
SVC_Stop:
    svc #7
    bx lr
.size SVC_Stop, . - SVC_Stop

.global SVC_SoundBias
.type SVC_SoundBias, %function
.thumb_func
SVC_SoundBias:
    svc #8
    bx lr
.size SVC_SoundBias, . - SVC_SoundBias

.global SVC_SoundBiasSet
.type SVC_SoundBiasSet, %function
.thumb_func
SVC_SoundBiasSet:
    adds r1, r0, #0
    movs r0, #1
    svc #8
    bx lr
.size SVC_SoundBiasSet, . - SVC_SoundBiasSet

.global SVC_SoundBiasReset
.type SVC_SoundBiasReset, %function
.thumb_func
SVC_SoundBiasReset:
    adds r1, r0, #0
    movs r0, #0
    svc #8
    bx lr
.size SVC_SoundBiasReset, . - SVC_SoundBiasReset

.global SVC_Div
.type SVC_Div, %function
.thumb_func
SVC_Div:
    svc #9
    bx lr
.size SVC_Div, . - SVC_Div

.global SVC_DivRem
.type SVC_DivRem, %function
.thumb_func
SVC_DivRem:
    svc #9
    adds r0, r1, #0
    bx lr
.size SVC_DivRem, . - SVC_DivRem

.global SVC_CpuSet
.type SVC_CpuSet, %function
.thumb_func
SVC_CpuSet:
    svc #11
    bx lr
.size SVC_CpuSet, . - SVC_CpuSet

.global SVC_CpuFastSet
.type SVC_CpuFastSet, %function
.thumb_func
SVC_CpuFastSet:
    svc #12
    bx lr
.size SVC_CpuFastSet, . - SVC_CpuFastSet

.global SVC_Sqrt
.type SVC_Sqrt, %function
.thumb_func
SVC_Sqrt:
    svc #13
    bx lr
.size SVC_Sqrt, . - SVC_Sqrt

.global SVC_GetCRC16
.type SVC_GetCRC16, %function
.thumb_func
SVC_GetCRC16:
    svc #14
    bx lr
.size SVC_GetCRC16, . - SVC_GetCRC16

.global SVC_IsDebugger
.type SVC_IsDebugger, %function
.thumb_func
SVC_IsDebugger:
    svc #15
    bx lr
.size SVC_IsDebugger, . - SVC_IsDebugger

.global SVC_BitUnPack
.type SVC_BitUnPack, %function
.thumb_func
SVC_BitUnPack:
    svc #16
    bx lr
.size SVC_BitUnPack, . - SVC_BitUnPack

.global SVC_LZ77UnCompWram
.type SVC_LZ77UnCompWram, %function
.thumb_func
SVC_LZ77UnCompWram:
    svc #17
    bx lr
.size SVC_LZ77UnCompWram, . - SVC_LZ77UnCompWram

.global SVC_LZ77UnCompVramWithReadCB
.type SVC_LZ77UnCompVramWithReadCB, %function
.thumb_func
SVC_LZ77UnCompVramWithReadCB:
    svc #18
    bx lr
.size SVC_LZ77UnCompVramWithReadCB, . - SVC_LZ77UnCompVramWithReadCB

.global SVC_HuffUnCompWithReadCB
.type SVC_HuffUnCompWithReadCB, %function
.thumb_func
SVC_HuffUnCompWithReadCB:
    svc #19
    bx lr
.size SVC_HuffUnCompWithReadCB, . - SVC_HuffUnCompWithReadCB

.global SVC_RLUnCompWram
.type SVC_RLUnCompWram, %function
.thumb_func
SVC_RLUnCompWram:
    svc #20
    bx lr
.size SVC_RLUnCompWram, . - SVC_RLUnCompWram

.global SVC_RLUnCompVramWithReadCB
.type SVC_RLUnCompVramWithReadCB, %function
.thumb_func
SVC_RLUnCompVramWithReadCB:
    svc #21
    bx lr
.size SVC_RLUnCompVramWithReadCB, . - SVC_RLUnCompVramWithReadCB

.global SVC_GetSineTable
.type SVC_GetSineTable, %function
.thumb_func
SVC_GetSineTable:
    svc #26
    bx lr
.size SVC_GetSineTable, . - SVC_GetSineTable

.global SVC_GetPitchTable
.type SVC_GetPitchTable, %function
.thumb_func
SVC_GetPitchTable:
    svc #27
    bx lr
.size SVC_GetPitchTable, . - SVC_GetPitchTable

.global SVC_GetVolumeTable
.type SVC_GetVolumeTable, %function
.thumb_func
SVC_GetVolumeTable:
    svc #28
    bx lr
.size SVC_GetVolumeTable, . - SVC_GetVolumeTable
