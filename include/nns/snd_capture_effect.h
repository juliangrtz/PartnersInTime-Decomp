#ifndef NNS_SND_CAPTURE_EFFECT_H
#define NNS_SND_CAPTURE_EFFECT_H

#include <nns/snd_capture.h>

typedef struct NNSiSndCaptureEffect NNSiSndCaptureEffect;
typedef void (*NNSiSndCaptureEffectProcess)(s16 *left, s16 *right, u32 size, NNSiSndCaptureEffect *effect);
struct NNSiSndCaptureEffect {
    int type;
    NNSiSndCaptureEffectProcess process;
    NNSSndCaptureCallback before;
    void *before_argument;
    NNSSndCaptureCallback after;
    void *after_argument;
    s32 history[24][2];
};

void NNSi_SndCaptureEffectMono(s16 *left, s16 *right, u32 size);
void NNSi_SndCaptureEffectHeadphone(s16 *left, s16 *right, u32 size, NNSiSndCaptureEffect *effect);
void NNSi_SndCaptureEffectSurround(s16 *left, s16 *right, u32 size, NNSiSndCaptureEffect *effect);
void NNSi_SndCaptureEffectNone(s16 *left, s16 *right, u32 size, NNSiSndCaptureEffect *effect);
void NNSi_SndCaptureEffectCallback(void *left, void *right, u32 size, int format, void *argument);
void NNS_SndCaptureChangeOutputEffect(int type);
void NNS_SndCaptureStopOutputEffect(void);
int NNS_SndCaptureStartOutputEffect(void *buffer, u32 size, int type);

#endif
