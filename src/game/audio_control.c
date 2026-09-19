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

void GameAudio_ClearDelayedEffects(void)
{
    int i;
    for (i = 3; i >= 0; --i) {
        data_0205ad98[i].sequence = 0;
        data_0205ad98[i].volume = -1;
        data_0205ad98[i].frames = 0;
    }
}

void GameAudio_UpdateDelayedEffects(void)
{
    GameAudioDelayedEffect *effect = data_0205ad98;
    int i;
    for (i = 3; i >= 0; --i) {
        if (effect->sequence && --effect->frames < 0) {
            GameAudio_PlayEffect(effect->sequence, effect->volume);
            effect->sequence = 0;
            effect->volume = -1;
            effect->frames = 0;
        }
        ++effect;
    }
}

void GameAudio_QueueEffect(s16 sequence, int delay, s16 volume)
{
    GameAudioDelayedEffect *effect = data_0205ad98;
    int i = 3;
    for (;;) {
        if (!effect->sequence) {
            effect->sequence = sequence;
            effect->volume = volume;
            effect->frames = delay;
            return;
        }
        ++effect;
        if (--i < 0) return;
    }
}

void GameAudio_PlayEffect(int sequence, int volume)
{
    NNS_SndStopSeqArcIdx(data_0205ad5c, sequence, 0);
    NNS_SndArcPlayerStartSeqArc(&data_0205ad78[0], data_0205ad5c, sequence);
    if (volume != -1) NNS_SndPlayerMoveVolume(&data_0205ad78[0], volume, 0);
}

void GameAudio_UpdateMusicFade(void)
{
    GameAudioFade *fade = &data_0205ad88;
    if (!fade->state) return;
    if (fade->state == 3) return;
    switch (fade->state) {
        case 1: {
            fade->current += fade->step;
            if (fade->current > fade->volume) {
                fade->current = fade->volume;
                fade->state = 0;
            }
            break;
        }
        case 2: {
            fade->current -= fade->step;
            if (fade->current < 0) {
                fade->current = 0;
                fade->state = 3;
            }
            break;
        }
    }
    NNS_SndPlayerSetTrackVolume(&data_0205ad24, 0xffff, (fade->current >> 12) & 0xfff);
}
