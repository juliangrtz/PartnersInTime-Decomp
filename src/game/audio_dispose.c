#include <game/audio.h>
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
