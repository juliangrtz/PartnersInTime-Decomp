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
