#ifndef PIT_GAME_ARCHIVE_IO_H
#define PIT_GAME_ARCHIVE_IO_H

#include <nitro/fs_overlay.h>

typedef struct ArchiveReadRequest ArchiveReadRequest;
struct ArchiveReadRequest {
    ArchiveReadRequest *next;
    const u32 *offsets;
    u8 *cursor;
    u8 *destination;
    const char *path;
    u32 limit;
    u32 remaining;
    u32 chunk_size;
    u32 offset;
    u16 entry;
    u16 state;
};

typedef struct ArchiveOpenRequest {
    const char *path;
    void *destination;
    u32 size;
    u16 state;
    u16 reserved;
} ArchiveOpenRequest;

typedef struct ArchiveIO {
    void *vtable;
    u8 task_state[36];
    ArchiveReadRequest *first;
    ArchiveReadRequest *last;
    FsFile file;
    FsOverlayInfo overlay;
    u8 *overlay_cursor;
    u32 overlay_remaining;
    u32 overlay_chunk;
    u16 overlay_state;
    u16 reserved_ae;
    u32 scanline;
    u16 budget_frames;
    u16 interval_frames;
    u16 current_frame;
    u16 reserved_ba;
    ArchiveOpenRequest opens[8];
    u16 open_head;
    u16 open_tail;
} ArchiveIO;

typedef char ArchiveReadRequestSizeCheck[(sizeof(ArchiveReadRequest) == 40) ? 1 : -1];
typedef char ArchiveOpenRequestSizeCheck[(sizeof(ArchiveOpenRequest) == 16) ? 1 : -1];
typedef char ArchiveIOSizeCheck[(sizeof(ArchiveIO) == 320) ? 1 : -1];

typedef void (*ArchiveBlockDecoder)(u8 **source, u8 **destination);
typedef struct ArchiveCompressedRequest {
    ArchiveReadRequest read;
    u16 compressed_state;
    u16 reserved;
    u8 *destination;
    u8 *write_cursor;
    ArchiveBlockDecoder decode;
    u32 output_size;
    u16 blocks;
    u16 reserved_3e;
} ArchiveCompressedRequest;
typedef struct ArchiveCompressedIO { ArchiveIO base; u8 buffer[512]; } ArchiveCompressedIO;

typedef struct ArchiveMemoryDecoder {
    void *vtable;
    u8 task_state[36];
    ArchiveReadRequest *first;
    ArchiveReadRequest *last;
    u32 scanline;
    u16 budget_frames;
    u16 interval_frames;
    u16 current_frame;
    u16 reserved;
} ArchiveMemoryDecoder;

typedef char ArchiveMemoryDecoderSizeCheck[(sizeof(ArchiveMemoryDecoder) == 60) ? 1 : -1];
typedef char ArchiveCompressedRequestSizeCheck[(sizeof(ArchiveCompressedRequest) == 64) ? 1 : -1];

void ArchiveCodec_ReadHeader(u8 **source, u32 *blocks, u32 *size);
void ArchiveIO_SelectDecoder(ArchiveIO *archive, ArchiveCompressedRequest *request);
int ArchiveIO_ProcessCompressedRead(ArchiveIO *archive);
int ArchiveIO_ReadImmediate(ArchiveIO *archive, ArchiveReadRequest *request);
void ArchiveIO_WaitCompressedRead(ArchiveIO *archive, ArchiveReadRequest *request);
int ArchiveIO_QueueCompressedRead(ArchiveIO *archive, ArchiveCompressedRequest *request, void *destination);
u32 ArchiveIO_PrepareCompressedRead(ArchiveIO *archive, ArchiveCompressedRequest *request, const u32 *offsets, const char *path);
void ArchiveIO_UpdateCompressed(ArchiveIO *archive);
ArchiveCompressedIO *ArchiveCompressedIO_DestroyBase(ArchiveCompressedIO *archive);
ArchiveCompressedIO *ArchiveCompressedIO_Delete(ArchiveCompressedIO *archive);
ArchiveCompressedIO *ArchiveCompressedIO_DestroyComplete(ArchiveCompressedIO *archive);
ArchiveCompressedIO *ArchiveCompressedIO_InitBase(ArchiveCompressedIO *archive, u32 priority, u32 unused, void *argument, int dma);
ArchiveCompressedIO *ArchiveCompressedIO_InitComplete(ArchiveCompressedIO *archive, u32 priority, u32 unused, void *argument, int dma);
void ArchiveMemoryDecoder_Process(ArchiveMemoryDecoder *decoder);
void ArchiveMemoryDecoder_Wait(ArchiveMemoryDecoder *decoder, ArchiveCompressedRequest *request);
void ArchiveMemoryDecoder_Queue(ArchiveMemoryDecoder *decoder, ArchiveCompressedRequest *request, void *destination);
u32 ArchiveMemoryDecoder_Prepare(ArchiveMemoryDecoder *decoder, ArchiveCompressedRequest *request);
void ArchiveMemoryDecoder_Update(ArchiveMemoryDecoder *decoder);
ArchiveMemoryDecoder *ArchiveMemoryDecoder_Delete(ArchiveMemoryDecoder *decoder);
ArchiveMemoryDecoder *ArchiveMemoryDecoder_Destroy(ArchiveMemoryDecoder *decoder);
ArchiveMemoryDecoder *ArchiveMemoryDecoder_Init(ArchiveMemoryDecoder *decoder, u32 priority, u32 unused, void *argument);

int ArchiveIO_ProcessOverlay(ArchiveIO *archive);
int ArchiveIO_ProcessRead(ArchiveIO *archive);
int ArchiveIO_QueueRead(ArchiveIO *archive, ArchiveReadRequest *request, u32 offset, const u32 *offsets, const char *path, u16 flags);
int ArchiveIO_ProcessOpen(ArchiveIO *archive);
int ArchiveIO_OpenFile(ArchiveIO *archive, const char *path);
int ArchiveIO_LoadFileTables(void *buffer, u32 size);
u32 ArchiveIO_GetFileTableSize(void);
u32 ArchiveIO_InitFilesystem(u32 dma, void *buffer, u32 size);
int ArchiveIO_UnloadOverlay(u32 id, int processor);
int ArchiveIO_FinishOverlay(ArchiveIO *archive);
int ArchiveIO_BeginOverlay(ArchiveIO *archive, u32 id, int processor);
int ArchiveIO_FinishGlobalOverlay(void);
int ArchiveIO_BeginGlobalOverlay(u32 id, u32 unused, int processor);
int ArchiveIO_LoadOverlay(u32 id, int processor);
void ArchiveIO_WaitRead(ArchiveIO *archive, ArchiveReadRequest *request);
int ArchiveIO_GetOpenState(ArchiveIO *archive, const char *path);
void ArchiveIO_WaitOpen(ArchiveIO *archive, const char *path);
void ArchiveIO_Update(ArchiveIO *archive);
ArchiveIO *ArchiveIO_DestroyBase(ArchiveIO *archive);
ArchiveIO *ArchiveIO_Delete(ArchiveIO *archive);
ArchiveIO *ArchiveIO_DestroyComplete(ArchiveIO *archive);
ArchiveIO *ArchiveIO_InitBase(ArchiveIO *archive, u32 priority, u32 unused, void *argument, int dma);
ArchiveIO *ArchiveIO_InitComplete(ArchiveIO *archive, u32 priority, u32 unused, void *argument, int dma);

static inline int ArchiveIO_FileSucceeded(FsFile *file)
{
    if (file->result == 0) return 1;
    return 0;
}

#endif
