#include <nns/snd_archive.h>

int NNS_SndArcPlayerSetup(NNSSndHeap *heap)
{
    int player;
    NNSSndArcPlayerInfo *info;
    int index;
    NNS_SndArcGetCurrent();
    for (player = 0; player < 32; player++) {
        info = NNS_SndArcGetPlayerInfo(player);
        if (info) {
            NNS_SndPlayerSetPlayableSeqCount(player, info->max_sequences);
            NNS_SndPlayerSetAllocatableChannel(player, info->channel_mask);
            if (info->heap_size && heap) {
                for (index = 0; index < info->max_sequences; index++) {
                    if (!NNS_SndPlayerCreateHeap(player, heap, info->heap_size)) return 0;
                }
            }
        }
    }
    return 1;
}

int NNS_SndArcPlayerStartSeq(NNSSndHandle *handle, int index)
{
    NNSSndArcSeqInfo *info = NNS_SndArcGetSeqInfo(index);
    if (!info) return 0;
    return NNSi_SndArcStartSeq(handle, info->player, info->bank, info->player_priority, info, index);
}

int NNS_SndArcPlayerStartSeqArc(NNSSndHandle *handle, int archive, int index)
{
    NNSSndArcSeqArcInfo *info;
    NNSSndSeqArcFile *file;
    NNSSndSeqArcEntry *entry;
    info = NNS_SndArcGetSeqArcInfo(archive);
    if (!info) return 0;
    file = NNS_SndArcGetFileAddress(info->file);
    if (!file) return 0;
    entry = NNSi_SndSeqArcGetEntry(file, index);
    if (!entry) return 0;
    return NNSi_SndArcStartSeqArc(handle, entry->player, entry->bank, entry->player_priority, entry, file, archive, index);
}

int NNSi_SndArcStartSeq(NNSSndHandle *handle, int player, int bank, int priority, NNSSndArcSeqInfo *info, int index)
{
    NNSSndSeqFile *file;
    SoundBank *bank_data;
    NNSSndHeap *heap;
    NNSiSndSeqPlayer *sequence;
    sequence = NNSi_SndPlayerAlloc(handle, player, priority);
    if (!sequence) return 0;
    heap = NNSi_SndPlayerAllocHeap(player, sequence);
    if (NNSi_SndArcLoadBank(bank, 6, heap, 0, &bank_data)) {
        NNSi_SndPlayerReleaseSequence(sequence);
        return 0;
    }
    if (NNSi_SndArcLoadSeq(index, 1, heap, 0, (void **)&file)) {
        NNSi_SndPlayerReleaseSequence(sequence);
        return 0;
    }
    NNSi_SndPlayerPrepareSequence(sequence, (u8 *)file + file->data_offset, 0, bank_data);
    NNSi_SndPlayerSetInitialVolume(handle, info->volume);
    NNS_SndPlayerSetChannelPriority(handle, info->channel_priority);
    NNSi_SndPlayerSetSeqNo(handle, index);
    return 1;
}

int NNSi_SndArcStartSeqArc(NNSSndHandle *handle, int player, int bank, int priority, NNSSndSeqArcEntry *entry, NNSSndSeqArcFile *file, int archive, int index)
{
    SoundBank *bank_data;
    NNSiSndSeqPlayer *sequence;
    NNSSndHeap *heap;
    sequence = NNSi_SndPlayerAlloc(handle, player, priority);
    if (!sequence) return 0;
    heap = NNSi_SndPlayerAllocHeap(player, sequence);
    if (NNSi_SndArcLoadBank(bank, 6, heap, 0, &bank_data)) {
        NNSi_SndPlayerReleaseSequence(sequence);
        return 0;
    }
    NNSi_SndPlayerPrepareSequence(sequence, (u8 *)file + file->data_offset, entry->offset, bank_data);
    NNSi_SndPlayerSetInitialVolume(handle, entry->volume);
    NNS_SndPlayerSetChannelPriority(handle, entry->channel_priority);
    NNSi_SndPlayerSetSeqArcNo(handle, archive, index);
    return 1;
}
