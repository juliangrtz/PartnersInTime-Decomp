#include <nns/snd_archive.h>

int NNS_SndArcLoadGroup(int index, NNSSndHeap *heap)
{
    return NNSi_SndArcLoadGroup(index, heap) == 0;
}

int NNS_SndArcLoadSeqArc(int index, NNSSndHeap *heap)
{
    return NNSi_SndArcLoadSeqArc(index, 255, heap, 1, 0) == 0;
}

int NNS_SndArcLoadBankEx(int index, u32 flags, NNSSndHeap *heap)
{
    return NNSi_SndArcLoadBank(index, flags, heap, 1, 0) == 0;
}

int NNSi_SndArcLoadGroup(int index, NNSSndHeap *heap)
{
    NNSSndArcGroupInfo *info;
    u32 item_index;
    NNSSndArcGroupItem *item;
    int result;
    info = NNS_SndArcGetGroupInfo(index);
    if (!info) return 1;
    for (item_index = 0; item_index < info->count; item_index++) {
        item = &info->items[item_index];
        switch (item->type) {
        case 0:
            result = NNSi_SndArcLoadSeq(item->index, item->flags, heap, 1, 0);
            if (result) return result;
            break;
        case 3:
            result = NNSi_SndArcLoadSeqArc(item->index, item->flags, heap, 1, 0);
            if (result) return result;
            break;
        case 1:
            result = NNSi_SndArcLoadBank(item->index, item->flags, heap, 1, 0);
            if (result) return result;
            break;
        case 2:
            result = NNSi_SndArcLoadWaveArc(item->index, item->flags, heap, 1, 0);
            if (result) return result;
            break;
        }
    }
    return 0;
}

int NNSi_SndArcLoadSeq(int index, u32 flags, NNSSndHeap *heap, int register_file, void **output)
{
    NNSSndArcSeqInfo *info;
    void *sequence;
    int result;
    info = NNS_SndArcGetSeqInfo(index);
    if (!info) return 2;
    result = NNSi_SndArcLoadBank(info->bank, flags, heap, register_file, 0);
    if (result) return result;
    if (flags & 1) {
        sequence = NNSi_SndArcLoadSeqFile(info->file, heap, register_file);
        if (!sequence) return 6;
    } else {
        sequence = NNS_SndArcGetFileAddress(info->file);
    }
    if (output) *output = sequence;
    return 0;
}

int NNSi_SndArcLoadSeqArc(int index, u32 flags, NNSSndHeap *heap, int register_file, void **output)
{
    NNSSndArcSeqArcInfo *info;
    void *sequence;
    info = NNS_SndArcGetSeqArcInfo(index);
    if (!info) return 3;
    if (flags & 8) {
        sequence = NNSi_SndArcLoadSeqArcFile(info->file, heap, register_file);
        if (!sequence) return 7;
    } else {
        sequence = NNS_SndArcGetFileAddress(info->file);
    }
    if (output) *output = sequence;
    return 0;
}

int NNSi_SndArcLoadBank(int index, u32 flags, NNSSndHeap *heap, int register_file, SoundBank **output)
{
    NNSSndArcBankInfo *info;
    SoundBank *bank;
    int slot;
    info = NNS_SndArcGetBankInfo(index);
    if (!info) return 4;
    if (flags & 2) {
        bank = NNSi_SndArcLoadBankFile(info->file, heap, register_file);
        if (!bank) return 8;
    } else {
        bank = NNS_SndArcGetFileAddress(info->file);
    }
    for (slot = 0; slot < 4; slot++) {
        if (info->wave_archives[slot] != 65535) {
            NNSSndArcWaveArcInfo *wave_info;
            SoundWaveArchive *wave;
            int result;
            wave_info = NNS_SndArcGetWaveArcInfo(info->wave_archives[slot]);
            if (!wave_info) return 5;
            result = NNSi_SndArcLoadWaveArc(info->wave_archives[slot], flags, heap, register_file, &wave);
            if (result) return result;
            if ((wave_info->flags & 1) && (flags & 4)) {
                if (!NNSi_SndArcLoadWaves(wave, bank, slot, wave_info->file, heap)) return 9;
            }
            if (bank && wave) func_0203d2d0(bank, slot, wave);
        }
    }
    if (output) *output = bank;
    return 0;
}

int NNSi_SndArcLoadWaveArc(int index, u32 flags, NNSSndHeap *heap, int register_file, SoundWaveArchive **output)
{
    NNSSndArcWaveArcInfo *info;
    SoundWaveArchive *wave;
    info = NNS_SndArcGetWaveArcInfo(index);
    if (!info) return 5;
    if (flags & 4) {
        if (info->flags & 1) wave = NNSi_SndArcLoadWaveArcHeader(info->file, heap, register_file);
        else wave = NNSi_SndArcLoadWaveArcFile(info->file, heap, register_file);
        if (!wave) return 9;
    } else {
        wave = NNS_SndArcGetFileAddress(info->file);
    }
    if (output) *output = wave;
    return 0;
}
