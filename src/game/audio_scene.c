#include <game/audio.h>

extern void NNS_SndPlayerSetAllocatableChannel(int player, u32 channels);

void GameAudio_ResetScene(void)
{
    GameAudio_Destroy();
    GameAudio_Init(0, 0x4a000, 0x1000);
    NNS_SndPlayerSetAllocatableChannel(0, 0xfff5);
}
