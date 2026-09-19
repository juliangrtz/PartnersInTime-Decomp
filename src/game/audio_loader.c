/*
 * Audio file loading (ARM9 resident, 0x02026524-0x02026DD8).
 *
 * Sequences, banks and wave archives are files, so playing music means reading
 * them first. Requests are appended to the loader's queue and served one at a
 * time; each carries the destination buffer and a callback fired when the file
 * is in place. The read buffer is allocated from the sound heap, and the loaded
 * range is flushed out of the data cache before the sound hardware reads it.
 */

#include <game/audio.h>

extern void DC_StoreRange(const void *data, u32 size);

int GameAudio_LoadWave(SoundWaveArchive *archive, u32 index, u32 file, NNSSndHeap *heap, int deferred)
{
    u32 size;
    void *buffer;
    u32 offset;
    if (func_0203cd6c(archive, index)) return 0;
    size = GameAudio_GetWaveSize(archive, index);
    buffer = GameAudio_AllocReadBuffer(heap, size, GameAudio_DisposeWave, (u32)archive, index);
    offset = archive->offsets[archive->count + index];
    if (deferred) GameAudio_QueueRead(buffer, size, file, offset, 0, 0);
    else {
        NNS_SndArcReadFile(file, buffer, size, offset);
        DC_StoreRange(buffer, size);
    }
    func_0203cdb4(archive, index, buffer);
    return 1;
}

/* Commands live in the buffers that will be overwritten by the file read.
 * Copy each field before advancing the intrusive queue. */
static inline void CopyLoadCommand(GameAudioLoadCommand *destination, const GameAudioLoadCommand *source)
{
    destination->next = source->next;
    destination->destination = source->destination;
    destination->file = source->file;
    destination->size = source->size;
    destination->offset = source->offset;
    destination->callback = source->callback;
    destination->argument = source->argument;
}

u32 GameAudio_GetWaveSize(const SoundWaveArchive *archive, u32 index)
{
    u32 count = func_0203cdec(archive);
    u32 start = archive->offsets[archive->count + index];
    u32 end;
    if (index < count - 1) end = (archive->offsets + (archive->count + index))[1];
    else end = archive->file_size;
    return end - start;
}

void *GameAudio_AllocReadBuffer(NNSSndHeap *heap, u32 size,
                              NNSSndHeapDisposeCallback dispose, u32 user0, u32 user1)
{
    if (size < sizeof(GameAudioLoadCommand)) size = sizeof(GameAudioLoadCommand);
    return NNS_SndHeapAlloc(heap, size + 32, dispose, user0, user1);
}

void GameAudio_InitLoaderQueue(void)
{
    data_0205ad30 = (GameAudioLoadCommand *)&data_0205ad70;
    data_0205ad70.tail = &data_0205ad30;
    data_0205ad70.next = (GameAudioLoadCommand *)-1;
}

void GameAudio_QueueRead(GameAudioLoadCommand *buffer, u32 size, u32 file, u32 offset,
                        void (*callback)(GameAudioLoadCommand *), void *argument)
{
    buffer->file = file;
    buffer->destination = (u8 *)buffer;
    buffer->size = size;
    buffer->offset = offset;
    buffer->callback = callback;
    buffer->argument = argument;
    *data_0205ad70.tail = buffer;
    buffer->next = (GameAudioLoadCommand *)&data_0205ad70;
    data_0205ad70.tail = &buffer->next;
}

void GameAudio_StartLoader(u8 mode, int sequence, u8 bank, int start)
{
    GameAudioLoadCommand *source = data_0205ad30;
    GameAudioLoader *loader = &data_0205ade8;
    CopyLoadCommand(&loader->command, source);
    loader->next = loader->command.next;
    loader->mode = mode;
    loader->sequence = sequence;
    loader->bank = bank;
    loader->offset = 0;
    loader->active = 1;
    loader->start = start;
}

void GameAudio_ProcessLoader(void)
{
    if (!GameAudio_ProcessLoadChunks()) return;
    if (data_0205ade8.active) return;
    switch (data_0205ade8.mode) {
    case 1:
        GameAudio_StartLoadedMusic(data_0205ade8.sequence, data_0205ade8.bank);
        MI_CpuFill8(&data_0205ade8, 0, sizeof(data_0205ade8));
        GameAudio_InitLoaderQueue();
        break;
    case 2: {
        int sequence;
        u8 bank;
        int start;
        bank = data_0205ade8.bank;
        sequence = data_0205ade8.sequence;
        start = data_0205ade8.start;
        MI_CpuFill8(&data_0205ade8, 0, sizeof(data_0205ade8));
        GameAudio_InitLoaderQueue();
        GameAudio_LoadDeferredWaves(sequence, bank);
        GameAudio_StartLoader((u8)start, sequence, bank, 0);
        break;
    }
    case 0:
        MI_CpuFill8(&data_0205ade8, 0, sizeof(data_0205ade8));
        GameAudio_InitLoaderQueue();
        break;
    }
}

int GameAudio_ProcessLoadChunks(void)
{
    GameAudioLoader *loader;
    u32 size;
    u32 done = 0;
    loader = &data_0205ade8;
    for (;;) {
        u8 *destination;
        u32 offset;
        size = data_0205ad28 - done;
        if (loader->command.size - loader->offset < size)
            size = loader->command.size - loader->offset;
        destination = loader->command.destination + loader->offset;
        NNS_SndArcReadFile(loader->command.file, destination, size,
                           loader->offset + loader->command.offset);
        DC_StoreRange(destination, size);
        offset = loader->offset + size;
        loader->offset = offset;
        if (offset < loader->command.size) goto incomplete;
        loader->offset = 0;
        if (loader->command.callback) loader->command.callback(&loader->command);
        CopyLoadCommand(&loader->command, loader->next);
        loader->next = loader->command.next;
        if (loader->command.next == (GameAudioLoadCommand *)-1) {
            loader->active = 0;
            break;
        }
        if (size < data_0205ad28) {
            done += size;
            if (done < data_0205ad28) continue;
        }
        break;
    }
    return 1;
incomplete:
    return 0;
}

#include <nitro/os_sync.h>

extern u8 data_0205b300[];
extern void func_0203be8c(u32 start, u32 end);
extern void func_0203beb8(u32 start, u32 end);
extern void func_0203bee4(u32 start, u32 end);

void GameAudio_DisposeSequence(void *memory, u32 size, u32 archive, u32 file)
{
    GameAudio_UncacheFile(memory, (NNSSndArc *)archive, file);
    func_0203bee4((u32)memory, (u32)memory + size);
}

void GameAudio_DisposeWaveArchive(void *memory, u32 size, u32 archive, u32 file)
{
    GameAudio_UncacheFile(memory, (NNSSndArc *)archive, file);
    func_0203be8c((u32)memory, (u32)memory + size);
    func_0203d1e4(memory);
}

void GameAudio_DisposeBank(void *memory, u32 size, u32 archive, u32 file)
{
    GameAudio_UncacheFile(memory, (NNSSndArc *)archive, file);
    func_0203beb8((u32)memory, (u32)memory + size);
    func_0203d230(memory);
}

void GameAudio_DisposeWaveHeader(void *memory, u32 size, u32 archive, u32 file)
{
    GameAudio_UncacheFile(memory, (NNSSndArc *)archive, file);
    func_0203d1e4(memory);
}

void GameAudio_DisposeWave(void *memory, u32 size, u32 archive, u32 index)
{
    SoundWaveArchive *wave = (SoundWaveArchive *)archive;
    if (wave->header_size && memory == func_0203cd6c(wave, index))
        func_0203cdb4(wave, index, 0);
    func_0203be8c((u32)memory, (u32)memory + size);
}

void GameAudio_UncacheFile(void *memory, NNSSndArc *archive, u32 file)
{
    NNSSndArc *previous;
    u32 interrupts;
    interrupts = OS_DisableInterrupts();
    previous = NNS_SndArcSetCurrent(archive);
    if (memory == NNS_SndArcGetFileAddress(file)) NNS_SndArcSetFileAddress(file, 0);
    NNS_SndArcSetCurrent(previous);
    OS_RestoreInterrupts(interrupts);
}

int GameAudio_StartReverb(int size, int volume)
{
    return NNS_SndCaptureStartReverb(data_0205b300, (size + 1) << 8, 0, 22050, volume);
}

extern void MIi_CpuCopy16(const void *source, void *destination, u32 size);
extern s16 data_02049f2c;
extern const u16 data_02049f80[];
extern s16 data_0205ac94;
extern u16 data_0205ac96[];
extern const u16 data_02049f24[];
extern const u16 data_02049f70[];
extern const u16 data_02049ef4[];
extern const u16 data_02049f3c[];
extern const u16 data_02049f58[];
extern const u16 data_02049f64[];
extern const u16 data_02049f40[];
extern const u16 data_02049efc[];
extern const u16 data_02049f48[];
extern const u16 data_02049f34[];
extern const u16 data_02049f50[];
extern const u16 data_02049f20[];
extern const u16 data_02049f04;
extern const u16 data_02049f18;
extern const u16 data_02049f10;
extern const u16 data_02049f08;
extern const u16 data_02049f0c;
extern const u16 data_02049f1c;
extern const u16 data_02049f00;
extern const u16 data_02049ef8;
extern const u16 data_02049f38;
extern const u16 data_02049f30;
extern const u16 data_02049f28;
extern const u16 data_02049f14;

void GameAudio_LoadMapWaves(s16 map, u32 bytes_per_frame)
{
    u16 count = 0;
    const u16 *additional;
    u16 additional_count;
    count += data_02049f2c;
    MIi_CpuCopy16(data_02049f80, data_0205ac96, count * 2);
    additional = 0;
    additional_count = 0;
    switch (map & 0xffffu) {
    case 0x5001:
        additional = data_02049efc;
        additional_count = data_02049ef8;
        break;
    case 0x7009:
    case 0x700a:
    case 0x700b:
    case 0x700f:
        additional = data_02049f34;
        additional_count = data_02049f30;
        break;
    case 0x8008:
        additional = data_02049f50;
        additional_count = data_02049f28;
        break;
    case 0x201b:
        additional = data_02049ef4;
        additional_count = data_02049f10;
        break;
    case 0x2027:
        additional = data_02049f3c;
        additional_count = data_02049f08;
        break;
    case 0x2028:
        additional = data_02049f58;
        additional_count = data_02049f0c;
        break;
    case 0x2000:
        additional = data_02049f24;
        additional_count = data_02049f04;
        break;
    case 0x2012:
        additional = data_02049f70;
        additional_count = data_02049f18;
        break;
    case 0x2029:
        additional = data_02049f64;
        additional_count = data_02049f1c;
        break;
    case 0x5000:
        additional = data_02049f40;
        additional_count = data_02049f00;
        break;
    case 0x600f:
        additional = data_02049f48;
        additional_count = data_02049f38;
        break;
    case 0x9016:
        additional = data_02049f20;
        additional_count = data_02049f14;
        break;
    }
    if (additional) {
        MIi_CpuCopy16(additional, data_0205ac96 + count, additional_count * 2);
        count += additional_count;
    }
    data_0205ac94 = count;
    GameAudio_LoadWaveList((const GameAudioWaveList *)&data_0205ac94, 1, bytes_per_frame);
}
