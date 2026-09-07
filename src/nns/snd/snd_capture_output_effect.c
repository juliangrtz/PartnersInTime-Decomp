#include <nns/snd_capture_effect.h>

extern NNSiSndCaptureEffect data_02059c9c;
extern s32 data_02059cb4[24][2];
extern void SND_SetSurroundDecay(int decay);
extern void MIi_CpuClear16(u16 value, void *destination, u32 size);
extern void MIi_CpuCopyFast(const void *source, void *destination, u32 size);

static inline int ClampSample(int sample)
{
    if (sample < -32768) sample = -32768;
    else if (sample > 32767) sample = 32767;
    return sample;
}

int NNS_SndCaptureStartOutputEffect(void *buffer, u32 size, int type)
{
    NNS_SndCaptureChangeOutputEffect(type);
    return NNS_SndCaptureStartEffect(buffer, size, 0, 32000, 2,
                                      NNSi_SndCaptureEffectCallback, &data_02059c9c);
}

void NNS_SndCaptureStopOutputEffect(void)
{
    if (data_02059c9c.type == 1) SND_SetSurroundDecay(0);
    NNS_SndCaptureStopEffect();
}

void NNS_SndCaptureChangeOutputEffect(int type)
{
    u32 interrupts;
    if (type == data_02059c9c.type) return;
    if (data_02059c9c.type == 1) SND_SetSurroundDecay(0);
    interrupts = OS_DisableInterrupts();
    {
        volatile u16 zero = 0;
        MIi_CpuClear16(zero, data_02059cb4, sizeof(data_02059cb4));
    }
    data_02059c9c.type = type;
    switch (type) {
    case 1: data_02059c9c.process = NNSi_SndCaptureEffectSurround; break;
    case 2: data_02059c9c.process = NNSi_SndCaptureEffectHeadphone; break;
    case 3: data_02059c9c.process = (NNSiSndCaptureEffectProcess)NNSi_SndCaptureEffectMono; break;
    case 0: data_02059c9c.process = NNSi_SndCaptureEffectNone; break;
    default: data_02059c9c.process = NNSi_SndCaptureEffectNone; break;
    }
    OS_RestoreInterrupts(interrupts);
    if (type == 1) SND_SetSurroundDecay(12288);
}

void NNSi_SndCaptureEffectCallback(void *left, void *right, u32 size, int format, void *argument)
{
    NNSiSndCaptureEffect *effect = argument;
    if (effect->before) effect->before(left, right, size, format, effect->before_argument);
    effect->process(left, right, size, effect);
    if (effect->after) effect->after(left, right, size, format, effect->after_argument);
    DC_FlushRange(left, size);
    DC_FlushRange(right, size);
}

void NNSi_SndCaptureEffectNone(s16 *left, s16 *right, u32 size, NNSiSndCaptureEffect *effect)
{
}

void NNSi_SndCaptureEffectSurround(s16 *left, s16 *right, u32 size, NNSiSndCaptureEffect *effect)
{
    u32 count = size >> 1;
    int i = 0;
    s16 last[4];
    s16 *l;
    s16 *r;
    for (; i < 2; ++i)
        last[i] = ClampSample((left + count + i)[-2] - (right + count + i)[-2]);
    l = &left[count - 1];
    r = &right[count - 1];
    while (l >= left + 2) {
        int difference = l[-2] - r[-2];
        int new_left = *l + difference;
        int new_right = *r - difference;
        if (difference >= 0) {
            if (new_left < 32767) *l = new_left;
            else *l = 32767;
            if (new_right > -32768) *r = new_right;
            else *r = -32768;
        } else {
            if (new_left > -32768) *l = new_left;
            else *l = -32768;
            if (new_right < 32767) *r = new_right;
            else *r = 32767;
        }
        --l;
        --r;
    }
    {
        int j;
        for (j = 1; j >= 0; --j) {
            int l = left[j] + effect->history[0][j];
            left[j] = ClampSample(l);
            {
                int r = right[j] - effect->history[0][j];
                right[j] = ClampSample(r);
            }
        }
    }
    {
        int k;
        for (k = 0; k < 2; ++k) effect->history[0][k] = last[k];
    }
}
