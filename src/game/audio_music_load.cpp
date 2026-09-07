extern "C" {
#include <game/audio.h>

extern void MI_CpuFill8(void *destination, u8 value, u32 size);

void GameAudio_PrepareMusic(u8 bank, int sequence, int start, u32 bytes_per_frame)
{
    NNSSndHeap *heap;
    int level;
    int deferred;
    int play;
    if (sequence == data_0205ad4c[bank]) {
        if (start) GameAudio_StartLoadedMusic(sequence, bank);
        return;
    }
    heap = bank ? data_0205ad20 : data_0205ad38;
    data_0205ad4c[bank] = sequence;
    level = NNS_SndHeapGetCurrentLevel(heap);
    NNS_SndHeapLoadState(heap, level);
    MI_CpuFill8(&data_0205ade8, 0, 56);
    GameAudio_InitLoaderQueue();
    deferred = GameAudio_LoadSequenceBank(heap, sequence, &data_0205adb8[bank]);
    play = start != 0;
    data_0205ad28 = bytes_per_frame;
    if (!deferred) GameAudio_StartLoader(play, sequence, bank, 0);
    else GameAudio_StartLoader(2, sequence, bank, play);
}

void GameAudio_LoadCommonBank(void)
{
    NNSSndArcBankInfo *bank;
    NNSSndArcWaveArcInfo *wave;
    NNS_SndArcLoadSeqArc(data_0205ad58, data_0205ad64);
    NNS_SndArcLoadBankEx(data_0205ad54, 2, data_0205ad64);
    bank = (NNSSndArcBankInfo *)NNS_SndArcGetBankInfo(data_0205ad54);
    wave = (NNSSndArcWaveArcInfo *)NNS_SndArcGetWaveArcInfo(bank->wave_archives[0]);
    GameAudio_LoadWaveArchive(wave->file, data_0205ad64, 0, 1, 0);
}

}
