#include <nns/snd_capture_internal.h>

int NNS_SndCaptureStartReverb(void *buffer, u32 size, int format, int rate, int volume)
{
    NNS_SndCaptureStopReverb(0);
    if (NNSi_SndCaptureGetWork()->active) return 0;
    NNSi_SndCaptureClear(buffer, size);
    DC_FlushRange(buffer, size);
    return NNSi_SndCaptureStart(0, buffer, (u8 *)buffer + (size >> 1), size >> 1, format, 0, 0, 1, rate, volume, 0, 127, 0, 0, 0);
}

void NNS_SndCaptureStopReverb(int frames)
{
    NNSiSndCapture *capture = NNSi_SndCaptureGetWork();
    if (!capture->active) return;
    if (capture->type != 0) return;
    if (frames == 0) NNSi_SndCaptureStop();
    else {
        NNSi_SndFaderSet(&capture->fader, 0, frames);
        capture->stopping = 1;
    }
}

int NNS_SndCaptureStartEffect(void *buffer, u32 size, int format, int rate, int blocks, NNSSndCaptureCallback callback, void *argument)
{
    NNS_SndCaptureStopEffect();
    if (NNSi_SndCaptureGetWork()->active) return 0;
    NNSi_SndCaptureClear(buffer, size);
    DC_FlushRange(buffer, size);
    return NNSi_SndCaptureStart(1, buffer, (u8 *)buffer + (size >> 1), size >> 1, format, 0, 0, 1, rate, 127, 0, 127, blocks, callback, argument);
}

