#include <nns/snd_capture_effect.h>

extern void MIi_CpuCopyFast(const void *source, void *destination, u32 size);

void NNSi_SndCaptureEffectMono(s16 *left, s16 *right, u32 size)
{
    u32 count = size >> 1;
    u32 i = 0;
    for (; i < count; ++i)
        left[i] = (left[i] + right[i] + 1) >> 1;
    MIi_CpuCopyFast(left, right, size);
}
