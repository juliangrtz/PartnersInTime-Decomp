#include <nns/snd.h>

extern NNSFndList data_02060ec8;
extern NNSFndList data_02060ed4;
extern NNSiSndSeqPlayer data_02060ee0[16];
extern NNSiSndPlayer data_02061320[32];
extern const s16 data_0204fb34[128];

extern void func_0203129c(void *heap);
extern void func_020311dc(void *heap);
extern void func_0202f554(NNSSndHandle *handle);
extern void func_0203c1f4(u32 sequence, int volume);
extern void func_0203c26c(u32 sequence);
extern void func_0203c238(u32 sequence);
extern void func_0203c154(u32 sequence, u32 tracks, u32 mask);
extern u32 func_0203c570(void);
extern int func_0203c51c(u32 command);
extern u32 func_0203ccc4(void);
extern void func_0203c20c(u32 sequence);

void NNSi_SndPlayerInit(void)
{
    int index;
    NNSiSndSeqPlayer *sequence;
    NNSiSndPlayer *player;
    int player_index;
    NNS_FndInitList(&data_02060ed4, 20);
    NNS_FndInitList(&data_02060ec8, 20);
    sequence = data_02060ee0;
    for (index = 0; index < 16; ++index, ++sequence) {
        sequence->state = 0;
        sequence->index = index;
        NNS_FndAppendListObject(&data_02060ec8, sequence);
    }
    player = data_02061320;
    for (player_index = 0; player_index < 32; ++player_index, ++player) {
        NNS_FndInitList(&player->sequences, 12);
        NNS_FndInitList(&player->heaps, 0);
        player->volume = 127;
        player->max_sequences = 1;
        player->channel_mask = 0;
    }
}

void NNSi_SndPlayerMain(void)
{
    u32 active_mask = func_0203ccc4();
    NNSiSndSeqPlayer *sequence = NNS_FndGetNextListObject(&data_02060ed4, 0);
    while (sequence) {
        NNSiSndSeqPlayer *next = NNS_FndGetNextListObject(&data_02060ed4, sequence);
        if (!sequence->started && func_0203c51c(sequence->command)) {
            sequence->started = 1;
        }
        if (sequence->started && !(active_mask & (1 << sequence->index))) {
            NNSi_SndPlayerFreeSequence(sequence);
        } else {
            int external_volume;
            int initial_volume;
            int player_volume;
            int volume;
            int fade;
            NNSi_SndFaderUpdate(&sequence->fader);
            external_volume = data_0204fb34[sequence->external_volume];
            initial_volume = data_0204fb34[sequence->initial_volume];
            player_volume = data_0204fb34[sequence->owner->volume];
            fade = data_0204fb34[NNSi_SndFaderGet(&sequence->fader) >> 8];
            volume = (player_volume + (initial_volume + external_volume)) + fade;
            if (volume < -723) {
                volume = -723;
            } else if (volume > 0) {
                volume = 0;
            }
            if (volume != sequence->volume_db) {
                func_0203c1f4(sequence->index, volume);
                sequence->volume_db = volume;
            }
            if (sequence->state == 2 && NNSi_SndFaderIsFinished(&sequence->fader)) {
                NNSi_SndPlayerForceStop(sequence);
            }
            if (sequence->start_pending) {
                func_0203c20c(sequence->index);
                sequence->start_pending = 0;
            }
        }
        sequence = next;
    }
}

NNSiSndSeqPlayer *NNSi_SndPlayerAlloc(NNSSndHandle *handle, int index, int priority)
{
    NNSiSndPlayer *player = &data_02061320[index];
    NNSiSndSeqPlayer *sequence;
    if (handle->player) {
        func_0202f554(handle);
    }
    if (player->sequences.count >= player->max_sequences) {
        sequence = NNS_FndGetNextListObject(&player->sequences, 0);
        if (!sequence) {
            return 0;
        }
        if (priority < sequence->priority) {
            return 0;
        }
        NNSi_SndPlayerForceStop(sequence);
    }
    sequence = NNSi_SndPlayerAllocSequence(priority);
    if (!sequence) {
        return 0;
    }
    NNSi_SndPlayerInsert(player, sequence);
    sequence->handle = handle;
    handle->player = sequence;
    return sequence;
}

void NNSi_SndPlayerReleaseSequence(NNSiSndSeqPlayer *sequence)
{
    NNSi_SndPlayerFreeSequence(sequence);
}

void NNSi_SndPlayerPrepareSequence(NNSiSndSeqPlayer *sequence)
{
    NNSiSndPlayer *player = sequence->owner;
    func_0203c238(sequence->index);
    if (player->channel_mask) {
        func_0203c154(sequence->index, 0xffff, player->channel_mask);
    }
    NNSi_SndPlayerResetSequence(sequence);
    sequence->command = func_0203c570();
    sequence->start_pending = 1;
    sequence->state = 1;
}

void NNSi_SndPlayerStopSequence(NNSiSndSeqPlayer *sequence, int frames)
{
    if (!sequence || !sequence->state) {
        return;
    }
    if (!frames) {
        NNSi_SndPlayerForceStop(sequence);
        return;
    }
    NNSi_SndFaderSet(&sequence->fader, 0, frames);
    NNSi_SndPlayerSetPriority(sequence, 0);
    sequence->state = 2;
}

void *NNSi_SndPlayerAllocHeap(int index, NNSiSndSeqPlayer *sequence)
{
    NNSiSndPlayer *player = &data_02061320[index];
    NNSiSndSeqHeap *heap = NNS_FndGetNextListObject(&player->heaps, 0);
    if (!heap) {
        return 0;
    }
    NNS_FndRemoveListObject(&player->heaps, heap);
    heap->player = sequence;
    sequence->heap = heap;
    func_020311dc(heap->heap);
    return heap->heap;
}

void NNSi_SndPlayerResetSequence(NNSiSndSeqPlayer *sequence)
{
    sequence->paused = 0;
    sequence->started = 0;
    sequence->start_pending = 0;
    sequence->sequence = 0;
    sequence->volume_db = 0;
    sequence->initial_volume = 127;
    sequence->external_volume = 127;
    NNSi_SndFaderInit(&sequence->fader);
    NNSi_SndFaderSet(&sequence->fader, 127 << 8, 1);
}

void NNSi_SndPlayerInsert(NNSiSndPlayer *player, NNSiSndSeqPlayer *sequence)
{
    NNSiSndSeqPlayer *next = NNS_FndGetNextListObject(&player->sequences, 0);
    while (next) {
        if ((u32)sequence->priority < (u32)next->priority) {
            break;
        }
        next = NNS_FndGetNextListObject(&player->sequences, next);
    }
    NNS_FndInsertListObject(&player->sequences, next, sequence);
    sequence->owner = player;
}

void NNSi_SndPlayerInsertActive(NNSiSndSeqPlayer *sequence)
{
    NNSiSndSeqPlayer *next = NNS_FndGetNextListObject(&data_02060ed4, 0);
    while (next) {
        if ((u32)sequence->priority < (u32)next->priority) {
            break;
        }
        next = NNS_FndGetNextListObject(&data_02060ed4, next);
    }
    NNS_FndInsertListObject(&data_02060ed4, next, sequence);
}

void NNSi_SndPlayerForceStop(NNSiSndSeqPlayer *sequence)
{
    if (sequence->state == 2) {
        func_0203c1f4(sequence->index, -723);
    }
    func_0203c26c(sequence->index);
    NNSi_SndPlayerFreeSequence(sequence);
}

NNSiSndSeqPlayer *NNSi_SndPlayerAllocSequence(int priority)
{
    NNSiSndSeqPlayer *sequence = NNS_FndGetNextListObject(&data_02060ec8, 0);
    if (!sequence) {
        sequence = NNS_FndGetNextListObject(&data_02060ed4, 0);
        if (priority < sequence->priority) {
            return 0;
        }
        NNSi_SndPlayerForceStop(sequence);
    }
    NNS_FndRemoveListObject(&data_02060ec8, sequence);
    sequence->priority = priority;
    NNSi_SndPlayerInsertActive(sequence);
    return sequence;
}

void NNSi_SndPlayerFreeSequence(NNSiSndSeqPlayer *sequence)
{
    NNSiSndPlayer *owner;
    if (sequence->handle) {
        sequence->handle->player = 0;
        sequence->handle = 0;
    }
    owner = sequence->owner;
    NNS_FndRemoveListObject(&owner->sequences, sequence);
    sequence->owner = 0;
    if (sequence->heap) {
        NNS_FndAppendListObject(&owner->heaps, sequence->heap);
        sequence->heap->player = 0;
        sequence->heap = 0;
    }
    NNS_FndRemoveListObject(&data_02060ed4, sequence);
    NNS_FndAppendListObject(&data_02060ec8, sequence);
    sequence->state = 0;
}

void NNSi_SndPlayerDisposeHeap(NNSiSndSeqHeap *heap)
{
    if (!heap->heap) {
        return;
    }
    func_0203129c(heap->heap);
    if (heap->player) {
        heap->player->heap = 0;
    } else {
        NNS_FndRemoveListObject(&data_02061320[heap->player_index].heaps, heap);
    }
}

void NNSi_SndPlayerSetPriority(NNSiSndSeqPlayer *sequence, u8 priority)
{
    NNSiSndPlayer *owner = sequence->owner;
    if (owner) {
        NNS_FndRemoveListObject(&owner->sequences, sequence);
        sequence->owner = 0;
    }
    NNS_FndRemoveListObject(&data_02060ed4, sequence);
    sequence->priority = priority;
    if (owner) {
        NNSi_SndPlayerInsert(owner, sequence);
    }
    NNSi_SndPlayerInsertActive(sequence);
}
