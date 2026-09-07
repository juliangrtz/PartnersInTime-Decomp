#include <game/audio.h>

extern void DC_StoreRange(const void *data, u32 size);

int GameAudio_LoadSequenceBank(NNSSndHeap *heap, int sequence, GameAudioMusicFiles *files)
{
    NNSSndArcBankInfo *bank;
    int deferred = 0;
    int i;
    NNSSndArcSeqInfo *info;
    files->sequence = 0;
    files->bank = 0;
    for (i = 0; i < 4; ++i) files->waves[i] = 0;
    info = NNS_SndArcGetSeqInfo(sequence);
    {
        u32 file = (u16)info->file;
        u32 size = NNS_SndArcGetFileSize(file);
        void *buffer = GameAudio_AllocReadBuffer(heap, size, GameAudio_DisposeSequence,
                                                (u32)&data_0205ae5c, file);
        GameAudio_QueueRead(buffer, size, file, 0, 0, 0);
        files->sequence = buffer;
    }
    bank = NNS_SndArcGetBankInfo(info->bank);
    {
        u32 file = (u16)bank->file;
        u32 size = NNS_SndArcGetFileSize(file);
        void *buffer = GameAudio_AllocReadBuffer(heap, size, GameAudio_DisposeBank,
                                                (u32)&data_0205ae5c, file);
        GameAudio_QueueRead(buffer, size, file, 0, 0, 0);
        files->bank = buffer;
    }
    for (i = 0; i < 4; ++i) {
        if (bank->wave_archives[i] != 0xffff) {
            NNSSndArcWaveArcInfo *wave = NNS_SndArcGetWaveArcInfo(bank->wave_archives[i]);
            u32 file = (u16)*(u32 *)wave;
            if (wave->flags & 1) {
                files->waves[i] = GameAudio_LoadWaveArchive(file, heap, 1, 0, 1);
                deferred = 1;
            } else {
                u32 size = NNS_SndArcGetFileSize(file);
                void *buffer = GameAudio_AllocReadBuffer(heap, size, GameAudio_DisposeWaveArchive,
                                                        (u32)&data_0205ae5c, file);
                GameAudio_QueueRead(buffer, size, file, 0, 0, 0);
                files->waves[i] = buffer;
            }
        }
    }
    return deferred;
}

void GameAudio_LoadDeferredWaves(int sequence, int slot)
{
    GameAudioMusicFiles *files = &data_0205adb8[slot];
    NNSSndArcSeqInfo *info = NNS_SndArcGetSeqInfo(sequence);
    NNSSndArcBankInfo *bank = NNS_SndArcGetBankInfo(info->bank);
    NNSSndHeap *heap = slot == 0 ? data_0205ad38 : data_0205ad20;
    int i;
    for (i = 0; i < 4; ++i) {
        if (bank->wave_archives[i] != 0xffff) {
            NNSSndArcWaveArcInfo *wave = NNS_SndArcGetWaveArcInfo(bank->wave_archives[i]);
            u32 file = (u16)*(u32 *)wave;
            if (wave->flags & 1)
                GameAudio_LoadBankWaves(files->waves[i], files->bank, i, file, heap);
        }
    }
}

void *GameAudio_LoadWaveArchive(u32 file, NNSSndHeap *heap, int force, int register_file, int deferred)
{
    SoundWaveArchive *wave = NNS_SndArcGetFileAddress(file);
    u32 size;
    u32 table_size;
    if (wave && !force) return wave;
    NNS_SndArcReadFile(file, &data_0205ae20, 60, 0);
    table_size = data_0205ae20.count << 2;
    size = (table_size << 1) + 60;
    wave = NNS_SndHeapAlloc(heap, size + 32, GameAudio_DisposeWaveHeader, (u32)&data_0205ae5c, file);
    if (!deferred) {
        NNS_SndArcReadFile(file, wave, table_size + 60, 0);
        MI_CpuCopy8(wave->offsets, &wave->offsets[wave->count], table_size);
        MI_CpuFill8(wave->offsets, 0, table_size);
        DC_StoreRange(wave, size);
    } else {
        GameAudio_QueueRead((GameAudioLoadCommand *)wave, table_size + 60, file, 0,
                            GameAudio_InitWaveTable, (void *)table_size);
    }
    if (register_file) NNS_SndArcSetFileAddress(file, wave);
    return wave;
}

void GameAudio_InitWaveTable(GameAudioLoadCommand *command)
{
    SoundWaveArchive *wave = (SoundWaveArchive *)command->destination;
    u32 size = (u32)command->argument;
    MI_CpuCopy8(wave->offsets, &wave->offsets[wave->count], size);
    MI_CpuFill8(wave->offsets, 0, size);
}

