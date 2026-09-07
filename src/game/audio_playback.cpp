extern "C" {
#include <game/audio.h>
#include <nitro/fx.h>

static inline int IsHandleValid(NNSSndHandle *handle)
{
    return handle->player != 0;
}

void GameAudio_ProcessLoading(void)
{
    if (data_0205ade8.active) GameAudio_ProcessLoader();
}

void GameAudio_PlayMusic(int sequence, u8 bank, u32 bytes_per_frame)
{
    if (!sequence) {
        GameAudio_StopMusic(0);
        return;
    }
    if (!data_0205ade8.active) {
        if (bank == data_0205ad68.bank && sequence != data_0205ad4c[bank]) GameAudio_StopMusic(0);
        GameAudio_PrepareMusic(bank, (s16)sequence, 1, bytes_per_frame);
    }
}

void GameAudio_LoadMusic(int sequence, u8 bank, u32 bytes_per_frame)
{
    if (sequence && !data_0205ade8.active) {
        if (bank == data_0205ad68.bank && sequence != data_0205ad4c[bank]) GameAudio_StopMusic(0);
        GameAudio_PrepareMusic(bank, (s16)sequence, 0, bytes_per_frame);
    }
}

void GameAudio_SetMusic(int sequence)
{
    if (sequence == data_0205ad4c[0]) { GameAudio_StartLoadedMusic(sequence, 0); return; }
    if (sequence == data_0205ad4c[1]) { GameAudio_StartLoadedMusic(sequence, 1); return; }
    GameAudio_PlayMusic(sequence, 0, 3072);
}

void GameAudio_StopMusic(int frames)
{
    if (!frames) frames = 8;
    NNS_SndStopSeq(&data_0205ad24, frames);
}

void GameAudio_FadeMusic(int out, int frames)
{
    GameAudioFade *fade = &data_0205ad88;
    if (out) {
        fade->state = 2;
        fade->current = fade->volume;
        if (frames == -1) frames = 20;
    } else {
        fade->state = 1;
        fade->current = 0;
        if (frames == -1) frames = 60;
    }
    fade->step = FX_Div(fade->volume, frames << 12);
}

int GameAudio_GetMusicState(void)
{
    if (IsHandleValid(&data_0205ad24)) return data_0205ad24.player->state;
    return 0;
}

u32 GameAudio_GetMusicBanks(void)
{
    return (data_0205ad4c[0] & 0xffff) | (data_0205ad4c[1] << 16);
}

void GameAudio_ClearMusicBank(int bank)
{
    NNSSndHeap *heap;
    int level;
    if (data_0205ad68.bank == bank) NNS_SndStopSeq(&data_0205ad24, 0);
    heap = bank == 0 ? data_0205ad38 : data_0205ad20;
    level = NNS_SndHeapGetCurrentLevel(heap);
    NNS_SndHeapLoadState(heap, level);
    data_0205ad4c[bank] = -1;
}

void GameAudio_PlayEffectDelayed(s16 sequence, int delay, s16 volume)
{
    if (delay) GameAudio_QueueEffect(sequence, delay, volume);
    else GameAudio_PlayEffect(sequence, volume);
}

void GameAudio_PlayEffectOnce(int sequence, int volume)
{
    if (!NNS_SndCountPlayingSeqArcIdx(data_0205ad5c, sequence)) {
        NNS_SndArcPlayerStartSeqArc(data_0205ad78, data_0205ad5c, sequence);
        if (volume != -1) NNS_SndPlayerMoveVolume(data_0205ad78, volume, 0);
    }
}

int GameAudio_PlayEffectOnFreeHandle(int sequence, int volume)
{
    int i;
    for (i = 3; i >= 1; i = (s16)(i - 1)) if (!IsHandleValid(&data_0205ad78[i])) break;
    if (i <= 0) return -1;
    NNS_SndArcPlayerStartSeqArc(&data_0205ad78[i], data_0205ad5c, sequence);
    if (volume != -1) NNS_SndPlayerMoveVolume(&data_0205ad78[i], volume, 0);
    return i;
}

void GameAudio_StopEffect(int sequence)
{
    NNS_SndStopSeqArcIdx(data_0205ad5c, sequence, 10);
}

void GameAudio_StopEffects(void)
{
    NNS_SndStopSeqArc(data_0205ad5c, 10);
    GameAudio_ClearDelayedEffects();
}

u32 GameAudio_GetEffectMask(void)
{
    u32 mask = 0;
    int i;
    for (i = 3; i >= 0; i = (s16)(i - 1)) if (IsHandleValid(&data_0205ad78[i])) mask |= 1 << i;
    return mask;
}

void GameAudio_PlayVoice(s16 sequence, int volume)
{
    int index;
    int variable;
    if (data_0205ad58 >= 0 && sequence) {
        GameAudio_StopVoice(sequence);
        data_0205ad14 ^= 1;
        if (data_0205ad14) {
            index = 0;
            data_0205ad44[0] = sequence;
            variable = 15;
        } else {
            index = 1;
            data_0205ad44[1] = sequence;
            variable = 14;
        }
        NNS_SndPlayerWriteGlobalVariable(variable, sequence);
        NNS_SndStopSeqArcIdx(data_0205ad58, index, 0);
        NNS_SndArcPlayerStartSeqArc(&data_0205ad2c, data_0205ad58, index);
        if (volume != -1) NNS_SndPlayerMoveVolume(&data_0205ad2c, volume, 0);
    }
}

}
