#include <nns/snd.h>

void NNSi_SndFaderInit(NNSiSndFader *fader)
{
    fader->origin = fader->target = 0;
    fader->position = fader->duration = 0;
}

void NNSi_SndFaderSet(NNSiSndFader *fader, int target, int duration)
{
    fader->origin = NNSi_SndFaderGet(fader);
    fader->target = target;
    fader->duration = duration;
    fader->position = 0;
}

int NNSi_SndFaderGet(NNSiSndFader *fader)
{
    if (fader->position >= fader->duration) {
        return fader->target;
    }
    return fader->origin + (fader->target - fader->origin) * fader->position / fader->duration;
}

void NNSi_SndFaderUpdate(NNSiSndFader *fader)
{
    if (fader->position < fader->duration) {
        ++fader->position;
    }
}

int NNSi_SndFaderIsFinished(NNSiSndFader *fader)
{
    return fader->position >= fader->duration;
}
