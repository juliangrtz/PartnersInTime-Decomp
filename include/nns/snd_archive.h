#ifndef NNS_SND_ARCHIVE_H
#define NNS_SND_ARCHIVE_H

#include <nns/snd.h>
#include <nitro/fs.h>

typedef struct NNSSndArcHeader {
    u32 signature;
    u16 byte_order;
    u16 version;
    u32 file_size;
    u16 header_size;
    u16 block_count;
    u32 symbol_offset;
    u32 symbol_size;
    u32 info_offset;
    u32 info_size;
    u32 fat_offset;
    u32 fat_size;
    u32 file_offset;
    u32 file_block_size;
} NNSSndArcHeader;

typedef struct NNSSndArcFatEntry {
    u32 offset;
    u32 size;
    void *address;
    u32 reserved;
} NNSSndArcFatEntry;

typedef struct NNSSndArcFat {
    u32 signature;
    u32 size;
    u32 count;
    NNSSndArcFatEntry entries[1];
} NNSSndArcFat;

typedef struct NNSSndArcInfo {
    u32 signature;
    u32 size;
    u32 sequence;
    u32 sequence_archive;
    u32 bank;
    u32 wave_archive;
    u32 player;
    u32 group;
    u32 stream_player;
    u32 stream;
} NNSSndArcInfo;

typedef struct NNSSndArc {
    NNSSndArcHeader header;
    int file_open;
    FsFile file;
    FsFileId file_id;
    NNSSndArcFat *fat;
    void *symbols;
    NNSSndArcInfo *info;
} NNSSndArc;

typedef char NNSSndArcSizeCheck[(sizeof(NNSSndArc) == 140) ? 1 : -1];

extern NNSSndArc *data_02061e34;

void NNSi_SndArcDisposeSymbols(void *memory, u32 size, u32 archive, u32 argument);
void NNSi_SndArcDisposeFat(void *memory, u32 size, u32 archive, u32 argument);
void NNSi_SndArcDisposeInfo(void *memory, u32 size, u32 archive, u32 argument);
void NNS_SndArcSetFileAddress(u32 file, void *address);
void *NNS_SndArcGetFileAddress(u32 file);
FsFileId NNS_SndArcGetFileID(void);
int NNS_SndArcReadFile(u32 file, void *buffer, u32 size, u32 offset);
u32 NNS_SndArcGetFileSize(u32 file);
u32 NNS_SndArcGetFileOffset(u32 file);
void *NNS_SndArcGetGroupInfo(int index);
void *NNS_SndArcGetStrmPlayerInfo(int index);
void *NNS_SndArcGetPlayerInfo(int index);
void *NNS_SndArcGetStrmInfo(int index);
void *NNS_SndArcGetWaveArcInfo(int index);
void *NNS_SndArcGetBankInfo(int index);
void *NNS_SndArcGetSeqArcInfo(int index);
void *NNS_SndArcGetSeqInfo(int index);
NNSSndArc *NNS_SndArcGetCurrent(void);
NNSSndArc *NNS_SndArcSetCurrent(NNSSndArc *archive);
int NNSi_SndArcLoadHeader(NNSSndArc *archive, NNSSndHeap *heap, int load_symbols);
void NNS_SndArcInit(NNSSndArc *archive, const char *path, NNSSndHeap *heap, int load_symbols);

#endif
