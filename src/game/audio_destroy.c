/*
 * Audio teardown (ARM9 resident, 0x02028040-0x020280EC).
 *
 * Shuts the audio subsystem down and releases its heap.
 */

#include <game/audio.h>

void GameAudio_Destroy(void)
{
    NNS_SndHeapClear(data_0205ad38);
    NNS_SndHeapClear(data_0205ad20);
    NNS_SndHeapClear(data_0205ad64);
    if (data_0205ad64) {
        NNS_SndHeapDestroy(data_0205ad64);
        data_0205ad64 = 0;
    }
    if (data_0205ad38) {
        NNS_SndHeapDestroy(data_0205ad38);
        data_0205ad38 = 0;
    }
    if (data_0205ad20) {
        NNS_SndHeapDestroy(data_0205ad20);
        data_0205ad20 = 0;
    }
}
