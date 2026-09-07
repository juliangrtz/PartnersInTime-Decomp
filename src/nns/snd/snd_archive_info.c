#include <nns/snd_archive.h>

typedef struct NNSiSndArcOffsetTable {
    u32 count;
    u32 entries[1];
} NNSiSndArcOffsetTable;

void *NNS_SndArcGetSeqInfo(int index)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcInfo *info = archive->info;
    u32 offset = info->sequence;
    NNSiSndArcOffsetTable *table = !offset ? 0 : (NNSiSndArcOffsetTable *)((u8 *)info + offset);
    if (!table) {
        return 0;
    }
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= table->count) {
        return 0;
    }
    offset = table->entries[index];
    info = archive->info;
    return !offset ? 0 : (u8 *)info + offset;
}

void *NNS_SndArcGetSeqArcInfo(int index)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcInfo *info = archive->info;
    u32 offset = info->sequence_archive;
    NNSiSndArcOffsetTable *table = !offset ? 0 : (NNSiSndArcOffsetTable *)((u8 *)info + offset);
    if (!table) {
        return 0;
    }
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= table->count) {
        return 0;
    }
    offset = table->entries[index];
    info = archive->info;
    return !offset ? 0 : (u8 *)info + offset;
}

void *NNS_SndArcGetBankInfo(int index)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcInfo *info = archive->info;
    u32 offset = info->bank;
    NNSiSndArcOffsetTable *table = !offset ? 0 : (NNSiSndArcOffsetTable *)((u8 *)info + offset);
    if (!table) {
        return 0;
    }
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= table->count) {
        return 0;
    }
    offset = table->entries[index];
    info = archive->info;
    return !offset ? 0 : (u8 *)info + offset;
}

void *NNS_SndArcGetWaveArcInfo(int index)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcInfo *info = archive->info;
    u32 offset = info->wave_archive;
    NNSiSndArcOffsetTable *table = !offset ? 0 : (NNSiSndArcOffsetTable *)((u8 *)info + offset);
    if (!table) {
        return 0;
    }
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= table->count) {
        return 0;
    }
    offset = table->entries[index];
    info = archive->info;
    return !offset ? 0 : (u8 *)info + offset;
}

void *NNS_SndArcGetStrmInfo(int index)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcInfo *info = archive->info;
    u32 offset = info->stream;
    NNSiSndArcOffsetTable *table = !offset ? 0 : (NNSiSndArcOffsetTable *)((u8 *)info + offset);
    if (!table) {
        return 0;
    }
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= table->count) {
        return 0;
    }
    offset = table->entries[index];
    info = archive->info;
    return !offset ? 0 : (u8 *)info + offset;
}

void *NNS_SndArcGetPlayerInfo(int index)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcInfo *info = archive->info;
    u32 offset = info->player;
    NNSiSndArcOffsetTable *table = !offset ? 0 : (NNSiSndArcOffsetTable *)((u8 *)info + offset);
    if (!table) {
        return 0;
    }
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= table->count) {
        return 0;
    }
    offset = table->entries[index];
    info = archive->info;
    return !offset ? 0 : (u8 *)info + offset;
}

void *NNS_SndArcGetStrmPlayerInfo(int index)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcInfo *info = archive->info;
    u32 offset = info->stream_player;
    NNSiSndArcOffsetTable *table = !offset ? 0 : (NNSiSndArcOffsetTable *)((u8 *)info + offset);
    if (!table) {
        return 0;
    }
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= table->count) {
        return 0;
    }
    offset = table->entries[index];
    info = archive->info;
    return !offset ? 0 : (u8 *)info + offset;
}

void *NNS_SndArcGetGroupInfo(int index)
{
    NNSSndArc *archive = data_02061e34;
    NNSSndArcInfo *info = archive->info;
    u32 offset = info->group;
    NNSiSndArcOffsetTable *table = !offset ? 0 : (NNSiSndArcOffsetTable *)((u8 *)info + offset);
    if (!table) {
        return 0;
    }
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= table->count) {
        return 0;
    }
    offset = table->entries[index];
    info = archive->info;
    return !offset ? 0 : (u8 *)info + offset;
}
