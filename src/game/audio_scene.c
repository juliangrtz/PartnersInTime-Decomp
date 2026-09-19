/*
 * Audio scene reset (ARM9 resident, 0x02026470-0x020264A8).
 *
 * Drops the audio state a scene left behind when the next one starts.
 */

#include <game/audio.h>

extern void NNS_SndPlayerSetAllocatableChannel(int player, u32 channels);

void GameAudio_ResetScene(void)
{
    GameAudio_Destroy();
    GameAudio_Init(0, 0x4a000, 0x1000);
    NNS_SndPlayerSetAllocatableChannel(0, 0xfff5);
}
