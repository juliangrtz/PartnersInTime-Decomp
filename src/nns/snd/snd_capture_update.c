#include <nns/snd_capture_internal.h>

void NNS_SndCaptureCreateThread(int priority)
{
    if (data_02061874) return;
    data_02061870 = 0;
    func_02039d08(&data_02061878, data_0206188c, 8);
    func_020395d0(&data_02061900, NNSi_SndCaptureThread, 0, &data_02061e34, 1024, priority);
    data_02061874 = 1;
    OS_WakeupThreadDirect(&data_02061900);
}

void NNSi_SndCaptureInit(void)
{
    data_02061874 = 0;
    NNSi_SndCaptureGetWork()->active = 0;
}

void NNSi_SndCaptureMain(void)
{
    NNSiSndCapture *capture = NNSi_SndCaptureGetWork();
    int volume;
    NNSiSndFader *fader;
    if (!capture->active) return;
    if (capture->type != 0) return;
    fader = &capture->fader;
    NNSi_SndFaderUpdate(fader);
    if (capture->stopping && NNSi_SndFaderIsFinished(fader)) {
        NNSi_SndCaptureStop();
        return;
    }
    volume = NNSi_SndFaderGet(fader) >> 8;
    if (volume == capture->volume) return;
    func_0203bf94(capture->playing_channels, volume, 0);
    capture->volume = volume;
}

