#ifndef NNS_SND_ARCHIVE_H
#define NNS_SND_ARCHIVE_H

#include <nns/snd.h>
#include <nitro/fs.h>
#include <nitro/snd_bank.h>

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

typedef struct NNSSndArcSeqInfo {
    u32 file;
    u16 bank;
    u8 volume;
    u8 channel_priority;
    u8 player_priority;
    u8 player;
    u16 reserved;
} NNSSndArcSeqInfo;

typedef struct NNSSndArcSeqArcInfo { u32 file; } NNSSndArcSeqArcInfo;
typedef struct NNSSndArcBankInfo { u32 file; u16 wave_archives[4]; } NNSSndArcBankInfo;
typedef struct NNSSndArcWaveArcInfo {
    u32 file : 24;
    u32 flags : 8;
} NNSSndArcWaveArcInfo;
typedef struct NNSSndArcGroupItem {
    u8 type;
    u8 flags;
    u16 reserved;
    u32 index;
} NNSSndArcGroupItem;
typedef struct NNSSndArcGroupInfo { u32 count; NNSSndArcGroupItem items[1]; } NNSSndArcGroupInfo;

typedef struct NNSSndArcPlayerInfo {
    u8 max_sequences;
    u8 reserved;
    u16 channel_mask;
    u32 heap_size;
} NNSSndArcPlayerInfo;

typedef struct NNSSndSeqFile { u8 header[24]; u32 data_offset; } NNSSndSeqFile;
typedef struct NNSSndSeqArcEntry {
    u32 offset;
    u16 bank;
    u8 volume;
    u8 channel_priority;
    u8 player_priority;
    u8 player;
    u16 reserved;
} NNSSndSeqArcEntry;
typedef struct NNSSndSeqArcFile {
    u8 header[24];
    u32 data_offset;
    u32 count;
    NNSSndSeqArcEntry entries[1];
} NNSSndSeqArcFile;

NNSSndSeqArcEntry *NNSi_SndSeqArcGetEntry(NNSSndSeqArcFile *archive, int index);
int NNSi_SndArcStartSeqArc(NNSSndHandle *handle, int player, int bank, int priority, NNSSndSeqArcEntry *entry, NNSSndSeqArcFile *file, int archive, int index);
int NNSi_SndArcStartSeq(NNSSndHandle *handle, int player, int bank, int priority, NNSSndArcSeqInfo *info, int index);
int NNS_SndArcPlayerStartSeqArc(NNSSndHandle *handle, int archive, int index);
int NNS_SndArcPlayerStartSeq(NNSSndHandle *handle, int index);
int NNS_SndArcPlayerSetup(NNSSndHeap *heap);

int NNSi_SndArcLoadWaveArc(int index, u32 flags, NNSSndHeap *heap, int register_file, SoundWaveArchive **output);
int NNSi_SndArcLoadBank(int index, u32 flags, NNSSndHeap *heap, int register_file, SoundBank **output);
int NNSi_SndArcLoadSeqArc(int index, u32 flags, NNSSndHeap *heap, int register_file, void **output);
int NNSi_SndArcLoadSeq(int index, u32 flags, NNSSndHeap *heap, int register_file, void **output);
int NNSi_SndArcLoadGroup(int index, NNSSndHeap *heap);
int NNS_SndArcLoadBankEx(int index, u32 flags, NNSSndHeap *heap);
int NNS_SndArcLoadSeqArc(int index, NNSSndHeap *heap);
int NNS_SndArcLoadGroup(int index, NNSSndHeap *heap);

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

void *NNSi_SndArcLoadFile(u32 file, NNSSndHeapDisposeCallback dispose, u32 user0, u32 user1, NNSSndHeap *heap);
void *NNSi_SndArcLoadSeqFile(u32 file, NNSSndHeap *heap, int register_file);
void *NNSi_SndArcLoadSeqArcFile(u32 file, NNSSndHeap *heap, int register_file);
SoundBank *NNSi_SndArcLoadBankFile(u32 file, NNSSndHeap *heap, int register_file);
SoundWaveArchive *NNSi_SndArcLoadWaveArcFile(u32 file, NNSSndHeap *heap, int register_file);
SoundWaveArchive *NNSi_SndArcLoadWaveArcHeader(u32 file, NNSSndHeap *heap, int register_file);
void NNSi_SndArcUncacheFile(void *memory, NNSSndArc *archive, u32 file);
void NNSi_SndArcDisposeSequence(void *memory, u32 size, u32 archive, u32 file);
void NNSi_SndArcDisposeBank(void *memory, u32 size, u32 archive, u32 file);
void NNSi_SndArcDisposeWaveArchive(void *memory, u32 size, u32 archive, u32 file);
void NNSi_SndArcDisposeWaveHeader(void *memory, u32 size, u32 archive, u32 file);
void NNSi_SndArcDisposeWave(void *memory, u32 size, u32 archive, u32 index);
int NNSi_SndArcLoadWave(SoundWaveArchive *wave, u32 index, u32 file, NNSSndHeap *heap);
int NNSi_SndArcLoadWaves(SoundWaveArchive *wave, SoundBank *bank, int slot, u32 file, NNSSndHeap *heap);

#endif
