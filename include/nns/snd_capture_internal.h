#ifndef NNS_SND_CAPTURE_INTERNAL_H
#define NNS_SND_CAPTURE_INTERNAL_H

#include <nns/snd_capture.h>
#include <nitro/snd_command.h>

extern int data_02061870;
extern int data_02061874;
extern OsMessageQueue data_02061878;
extern void *data_0206188c[8];
extern NNSiSndCapture data_020618ac;
extern OsThread data_02061900;
extern NNSiSndCaptureMessage data_02061994[8];
/* The capture thread stack ends at the following archive-state symbol. */
struct NNSSndArc;
extern struct NNSSndArc *data_02061e34;
extern void DC_InvalidateRange(const void *address, u32 size);
extern void DC_FlushRange(const void *address, u32 size);
extern void func_0203b7a0(u32 value, void *destination, u32 size);
extern void func_020395d0(OsThread *thread, void (*entry)(void *), void *argument, void *stack, u32 size, u32 priority);
extern void OS_WakeupThreadDirect(OsThread *thread);
extern void func_0203c120(u32 channels, u32 captures, u32 alarms, u32 flags);
extern void func_0203c0b0(u32 channels, u32 captures, u32 alarms, u32 flags);
extern u32 func_0203c570(void);
extern void func_0203c5b0(u32 command);
extern void func_0203c020(u32 alarm, u32 first, u32 period, SoundCallback callback, void *argument);
extern void func_0203c064(u32 capture, u32 format, u32 buffer, u32 words, u32 loop, u32 input, u32 output);
extern void func_0203bf10(u32 channel, u32 format, u32 buffer, u32 loop, u32 loop_start, u32 words, u32 volume, u32 shift, u32 timer, u32 pan);
extern void func_0203bf94(u32 channels, u32 volume, u32 shift);
extern void func_0203be58(u32 left, u32 right, u32 channel1, u32 channel3);

static inline NNSiSndCapture *NNSi_SndCaptureGetWork(void)
{
    return (NNSiSndCapture *)(u32)&data_020618ac;
}

static inline void NNSi_SndCaptureClear(void *buffer, u32 size)
{
    volatile u32 zero = 0;
    func_0203b7a0(zero, buffer, size);
}

#endif
