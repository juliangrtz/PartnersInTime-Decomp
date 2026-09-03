/* Main ARM7 sound-driver initialization and update loop. */

.syntax unified
.arch armv4t
.arm
.section .pit_SND_MainThread, "ax", %progbits
.balign 4

.extern SND_AlarmInit
.extern SND_CalcRandom
.extern SND_CommandProc
.extern SND_Enable
.extern SND_ExChannelInit
.extern SND_ExChannelMain
.extern SND_InitIntervalTimer
.extern SND_SeqInit
.extern SND_SeqMain
.extern SND_SetMasterVolume
.extern SND_SetOutputSelector
.extern SND_StartIntervalTimer
.extern SND_UpdateExChannel
.extern SND_UpdateSharedWork
.extern SND_WaitForIntervalTimer

.global SndThread
.type SndThread, %function
SndThread:
    push {r4, r5, r6, lr}
    bl SND_InitIntervalTimer
    bl SND_ExChannelInit
    bl SND_SeqInit
    bl SND_AlarmInit
    bl SND_Enable
    mov r0, #0
    mov r1, r0
    mov r2, r0
    mov r3, r0
    bl SND_SetOutputSelector
    mov r0, #0x7F
    bl SND_SetMasterVolume
    bl SND_StartIntervalTimer
    mov r4, #1
    mov r5, #0
.L_sound_thread_loop:
    mov r6, r5
    bl SND_WaitForIntervalTimer
    cmp r0, #1
    beq .L_sound_thread_tick
    cmp r0, #2
    b .L_sound_thread_update
.L_sound_thread_tick:
    mov r6, r4
.L_sound_thread_update:
    bl SND_UpdateExChannel
    bl SND_CommandProc
    mov r0, r6
    bl SND_SeqMain
    mov r0, r6
    bl SND_ExChannelMain
    bl SND_UpdateSharedWork
    bl SND_CalcRandom
    b .L_sound_thread_loop
.size SndThread, . - SndThread
