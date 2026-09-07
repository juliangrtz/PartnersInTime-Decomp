#include <game/audio.h>

extern const int data_02059b00[];
extern const u16 data_02059afc[2];
extern u8 data_0205af00[1024];

void GameAudio_StopVoice(int sequence)
{
    if ((data_0205ad44[0] == sequence || sequence < 0) && NNS_SndCountPlayingSeqArcIdx(data_0205ad58, 0))
        NNS_SndStopSeqArcIdx(data_0205ad58, 0, 4);
    if ((data_0205ad44[1] == sequence || sequence < 0) && NNS_SndCountPlayingSeqArcIdx(data_0205ad58, 1))
        NNS_SndStopSeqArcIdx(data_0205ad58, 1, 4);
}

void GameAudio_SetOutputEffect(int type)
{
    int effect = data_02059b00[type];
    if (effect != -1) {
        if (data_0205ad1c) NNS_SndCaptureChangeOutputEffect(effect);
        else {
            data_0205ad1c = 1;
            NNS_SndCaptureStopReverb(0);
            NNS_SndCaptureStartOutputEffect(data_0205af00, 1024, effect);
        }
    } else {
        int size;
        int volume;
        data_0205ad1c = 0;
        NNS_SndCaptureStopOutputEffect();
        size = (u8)data_02059afc[0];
        volume = (u8)data_02059afc[1];
        GameAudio_StartReverb(size, volume);
    }
}

void GameAudio_StartStream(int stream)
{
    NNS_SndArcStrmStart(&data_0205ad60, stream, 0);
}

void GameAudio_StopStream(void)
{
    NNS_SndArcStrmStop(&data_0205ad60, 16);
}

int GameAudio_IsStreamPlaying(void)
{
    return data_0205ad60.player != 0;
}

int GameAudio_IsLoading(void)
{
    return data_0205ade8.active;
}
