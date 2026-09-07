#include <game/audio.h>

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
