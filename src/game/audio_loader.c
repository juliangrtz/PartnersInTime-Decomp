#include <game/audio.h>

extern void DC_StoreRange(const void *data, u32 size);

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
