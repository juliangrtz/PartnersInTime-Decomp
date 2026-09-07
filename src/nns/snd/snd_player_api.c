#include <nns/snd.h>

extern NNSFndList data_02060ed4;
extern NNSiSndSeqPlayer data_02060ee0[16];
extern NNSiSndPlayer data_02061320[32];
/* Native symbols identify the two columns in the 36-byte player table. */
extern u8 data_02061338[];
extern u8 data_0206133c[];
extern const s16 data_0204fb34[128];

extern void func_0203c188(u32 player, u32 tracks, int volume);
extern void func_0203c1dc(u32 player, u32 priority);
extern void func_0203c1b0(u32 index, s16 value);

void NNS_SndPlayerSetPlayableSeqCount(int player, int count)
{
    *(u32 *)(data_02061338 + sizeof(NNSiSndPlayer) * player) = (u16)count;
}

void NNS_SndPlayerSetAllocatableChannel(int player, u32 mask)
{
    *(u32 *)(data_0206133c + sizeof(NNSiSndPlayer) * player) = mask;
}

int NNS_SndPlayerCreateHeap(int player, void *parent, u32 size)
{
    NNSiSndSeqHeap *heap = NNS_SndHeapAlloc(parent, size + sizeof(NNSiSndSeqHeap), NNSi_SndPlayerDisposeHeap, 0, 0);
    NNSSndHeap *created;
    if (!heap) {
        return 0;
    }
    heap->player = 0;
    heap->player_index = player;
    heap->heap = 0;
    created = NNS_SndHeapCreate(heap + 1, size);
    if (!created) {
        return 0;
    }
    heap->heap = created;
    NNS_FndAppendListObject(&data_02061320[player].heaps, heap);
    return 1;
}

void NNS_SndStopSeq(NNSSndHandle *handle, int frames)
{
    NNSi_SndPlayerStopSequence(handle->player, frames);
}

void NNS_SndStopSeqArc(int archive, int frames)
{
    int index;
    NNSiSndSeqPlayer *sequence = data_02060ee0;
    for (index = 0; index < 16; ++index, ++sequence) {
        if (sequence->state && sequence->sequence_kind == 2 && sequence->archive_or_sequence == archive) {
            NNSi_SndPlayerStopSequence(sequence, frames);
        }
    }
}

void NNS_SndStopSeqArcIdx(int archive, int archive_index, int frames)
{
    int index;
    NNSiSndSeqPlayer *sequence = data_02060ee0;
    for (index = 0; index < 16; ++index, ++sequence) {
        if (sequence->state && sequence->sequence_kind == 2 &&
            sequence->archive_or_sequence == archive && sequence->archive_sequence == archive_index) {
            NNSi_SndPlayerStopSequence(sequence, frames);
        }
    }
}

void NNS_SndHandleInit(NNSSndHandle *handle)
{
    handle->player = 0;
}

void NNS_SndHandleReleaseSeq(NNSSndHandle *handle)
{
    if (handle->player) {
        handle->player->handle = 0;
        handle->player = 0;
    }
}

int NNS_SndCountPlayingSeqArcIdx(int archive, int index)
{
    int count = 0;
    NNSiSndSeqPlayer *sequence = NNS_FndGetNextListObject(&data_02060ed4, 0);
    for (; sequence; sequence = NNS_FndGetNextListObject(&data_02060ed4, sequence)) {
        if (sequence->sequence_kind == 2 && sequence->archive_or_sequence == archive &&
            sequence->archive_sequence == index) {
            ++count;
        }
    }
    return count;
}

void NNSi_SndPlayerSetInitialVolume(NNSSndHandle *handle, u8 volume)
{
    if (handle->player) {
        handle->player->initial_volume = volume;
    }
}

void NNS_SndPlayerMoveVolume(NNSSndHandle *handle, int volume, int frames)
{
    NNSiSndSeqPlayer *sequence = handle->player;
    if (!sequence || sequence->state == 2) {
        return;
    }
    NNSi_SndFaderSet(&sequence->fader, volume << 8, frames);
}

void NNS_SndPlayerSetChannelPriority(NNSSndHandle *handle, u32 priority)
{
    if (handle->player) {
        func_0203c1dc(handle->player->index, priority);
    }
}

void NNS_SndPlayerSetTrackVolume(NNSSndHandle *handle, u32 tracks, int volume)
{
    if (handle->player) {
        func_0203c188(handle->player->index, tracks, data_0204fb34[volume]);
    }
}

void NNSi_SndPlayerSetSeqNo(NNSSndHandle *handle, int sequence)
{
    if (handle->player) {
        handle->player->sequence_kind = 1;
        handle->player->archive_or_sequence = sequence;
    }
}

void NNSi_SndPlayerSetSeqArcNo(NNSSndHandle *handle, int archive, int sequence)
{
    if (handle->player) {
        handle->player->sequence_kind = 2;
        handle->player->archive_or_sequence = archive;
        handle->player->archive_sequence = sequence;
    }
}

int NNS_SndPlayerWriteGlobalVariable(u32 index, s16 value)
{
    func_0203c1b0(index, value);
    return 1;
}
