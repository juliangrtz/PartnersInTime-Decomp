#include <nns/snd_archive.h>
#include <nitro/snd_bank.h>
#include <nitro/os_sync.h>

extern SoundWaveArchive data_02061e38;
extern void DC_StoreRange(const void *data, u32 size);
extern void func_0203be8c(u32 start, u32 end);
extern void func_0203beb8(u32 start, u32 end);
extern void func_0203bee4(u32 start, u32 end);

void *NNSi_SndArcLoadFile(u32 file, NNSSndHeapDisposeCallback dispose, u32 user0, u32 user1, NNSSndHeap *heap)
{
    u32 size;
    void *memory;
    size = NNS_SndArcGetFileSize(file);
    if (!size) return 0;
    if (!heap) return 0;
    memory = NNS_SndHeapAlloc(heap, size + 32, dispose, user0, user1);
    if (!memory) return 0;
    if (size != NNS_SndArcReadFile(file, memory, size, 0)) return 0;
    DC_StoreRange(memory, size);
    return memory;
}

void *NNSi_SndArcLoadSeqFile(u32 file, NNSSndHeap *heap, int register_file)
{
    void *memory = NNS_SndArcGetFileAddress(file);
    if (!memory) {
        NNSSndArc *archive = register_file ? NNS_SndArcGetCurrent() : 0;
        memory = NNSi_SndArcLoadFile(file, NNSi_SndArcDisposeSequence, (u32)archive, file, heap);
        if (register_file && memory) NNS_SndArcSetFileAddress(file, memory);
    }
    return memory;
}

void *NNSi_SndArcLoadSeqArcFile(u32 file, NNSSndHeap *heap, int register_file)
{
    void *memory = NNS_SndArcGetFileAddress(file);
    if (!memory) {
        NNSSndArc *archive = register_file ? NNS_SndArcGetCurrent() : 0;
        memory = NNSi_SndArcLoadFile(file, NNSi_SndArcDisposeSequence, (u32)archive, file, heap);
        if (register_file && memory) NNS_SndArcSetFileAddress(file, memory);
    }
    return memory;
}

SoundBank *NNSi_SndArcLoadBankFile(u32 file, NNSSndHeap *heap, int register_file)
{
    SoundBank *memory = NNS_SndArcGetFileAddress(file);
    if (!memory) {
        NNSSndArc *archive = register_file ? NNS_SndArcGetCurrent() : 0;
        memory = NNSi_SndArcLoadFile(file, NNSi_SndArcDisposeBank, (u32)archive, file, heap);
        if (register_file && memory) NNS_SndArcSetFileAddress(file, memory);
    }
    return memory;
}

SoundWaveArchive *NNSi_SndArcLoadWaveArcFile(u32 file, NNSSndHeap *heap, int register_file)
{
    SoundWaveArchive *memory = NNS_SndArcGetFileAddress(file);
    if (!memory) {
        NNSSndArc *archive = register_file ? NNS_SndArcGetCurrent() : 0;
        memory = NNSi_SndArcLoadFile(file, NNSi_SndArcDisposeWaveArchive, (u32)archive, file, heap);
        if (register_file && memory) NNS_SndArcSetFileAddress(file, memory);
    }
    return memory;
}

SoundWaveArchive *NNSi_SndArcLoadWaveArcHeader(u32 file, NNSSndHeap *heap, int register_file)
{
    SoundWaveArchive *wave;
    u32 size;
    u32 table_size;
    wave = NNS_SndArcGetFileAddress(file);
    if (!wave) {
        NNSSndArc *archive;
        int bytes_read;
        if (NNS_SndArcReadFile(file, &data_02061e38, 60, 0) != 60) return 0;
        table_size = data_02061e38.count << 2;
        size = (table_size << 1) + 60;
        if (!heap) return 0;
        archive = register_file ? NNS_SndArcGetCurrent() : 0;
        wave = NNS_SndHeapAlloc(heap, size + 32, NNSi_SndArcDisposeWaveHeader, (u32)archive, file);
        if (!wave) return 0;
        bytes_read = NNS_SndArcReadFile(file, wave, table_size + 60, 0);
        if (bytes_read != table_size + 60) return 0;
        MI_CpuCopy8(wave->offsets, &wave->offsets[wave->count], table_size);
        MI_CpuFill8(wave->offsets, 0, table_size);
        DC_StoreRange(wave, size);
        if (register_file) NNS_SndArcSetFileAddress(file, wave);
    }
    return wave;
}

void NNSi_SndArcUncacheFile(void *memory, NNSSndArc *archive, u32 file)
{
    NNSSndArc *previous;
    u32 interrupts;
    if (!archive) return;
    interrupts = OS_DisableInterrupts();
    previous = NNS_SndArcSetCurrent(archive);
    if (memory == NNS_SndArcGetFileAddress(file)) NNS_SndArcSetFileAddress(file, 0);
    NNS_SndArcSetCurrent(previous);
    OS_RestoreInterrupts(interrupts);
}

void NNSi_SndArcDisposeSequence(void *memory, u32 size, u32 archive, u32 file)
{
    NNSi_SndArcUncacheFile(memory, (NNSSndArc *)archive, file);
    func_0203bee4((u32)memory, (u32)memory + size);
}

void NNSi_SndArcDisposeBank(void *memory, u32 size, u32 archive, u32 file)
{
    NNSi_SndArcUncacheFile(memory, (NNSSndArc *)archive, file);
    func_0203beb8((u32)memory, (u32)memory + size);
    func_0203d230(memory);
}

void NNSi_SndArcDisposeWaveArchive(void *memory, u32 size, u32 archive, u32 file)
{
    NNSi_SndArcUncacheFile(memory, (NNSSndArc *)archive, file);
    func_0203be8c((u32)memory, (u32)memory + size);
    func_0203d1e4(memory);
}

void NNSi_SndArcDisposeWaveHeader(void *memory, u32 size, u32 archive, u32 file)
{
    NNSi_SndArcUncacheFile(memory, (NNSSndArc *)archive, file);
    func_0203d1e4(memory);
}

void NNSi_SndArcDisposeWave(void *memory, u32 size, u32 archive, u32 index)
{
    SoundWaveArchive *wave = (SoundWaveArchive *)archive;
    if (memory == func_0203cd6c(wave, index)) func_0203cdb4(wave, index, 0);
    func_0203be8c((u32)memory, (u32)memory + size);
}

