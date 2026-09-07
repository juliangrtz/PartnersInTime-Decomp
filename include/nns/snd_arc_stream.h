#ifndef NNS_SND_ARC_STREAM_H
#define NNS_SND_ARC_STREAM_H

#include <nns/snd_stream.h>
#include <nns/snd_archive.h>

typedef struct NNSiSndArcStrmPlayer NNSiSndArcStrmPlayer;
typedef struct NNSSndStrmHandle { NNSiSndArcStrmPlayer *player; } NNSSndStrmHandle;
typedef struct NNSSndArcStrmInfo {
    u32 file;
    u8 volume;
    u8 priority;
    u8 player;
    u8 flags;
} NNSSndArcStrmInfo;
typedef struct NNSSndArcStrmPlayerInfo { u8 count; u8 channels[6]; } NNSSndArcStrmPlayerInfo;

typedef struct NNSiSndStrmFileHeader {
    u8 header[24];
    u8 format, loop, channels, reserved;
    u16 sample_rate, timer;
    u32 loop_start, samples, data_offset, blocks;
    u32 block_size, block_samples, last_block_size, last_block_samples;
} NNSiSndStrmFileHeader;

typedef struct NNSiSndArcStrmRequest { int player; int stream; } NNSiSndArcStrmRequest;
typedef struct NNSiSndArcStrmResponse { int stream; u32 offset_ms; } NNSiSndArcStrmResponse;
typedef int (*NNSSndArcStrmCallback)(int event, NNSiSndArcStrmRequest *request, NNSiSndArcStrmResponse *response, void *argument);

typedef struct NNSiSndAdpcmState {
    s16 sample;
    u8 index;
    u8 reserved;
} NNSiSndAdpcmState;

struct NNSiSndArcStrmPlayer {
    NNSSndStrm stream;
    FsFile file;
    u32 file_offset;
    NNSiSndStrmFileHeader header;
    NNSiSndFader fader;
    NNSiSndAdpcmState decoder[6];
    signed int active : 1;
    signed int playing : 1;
    signed int start_pending : 1;
    signed int fading : 1;
    signed int seek_adpcm : 1;
    signed int finished : 1;
    signed int mono : 1;
    signed int reserved_flags : 25;
    int blocks;
    int prepared;
    int pending;
    int channel_refs;
    u8 channel_count;
    u8 reserved289;
    u8 channels[6];
    void *buffer;
    u32 buffer_size;
    NNSSndStrmCallback callback;
    void *argument;
    NNSSndArcStrmCallback archive_callback;
    void *archive_argument;
    int stream_index;
    int player_index;
    NNSSndStrmHandle *handle;
    int priority;
    int volume;
    int volume_db;
    u32 sample_offset;
};

typedef struct NNSiSndArcStrmCommand {
    NNSFndLink link;
    NNSiSndArcStrmPlayer *player;
    int reason;
    int channels;
    void *buffers[6];
    u32 size;
} NNSiSndArcStrmCommand;

typedef struct NNSiSndArcStrmThread {
    OsThread thread;
    u8 stack[1024];
    OsThreadQueue queue;
    u16 padding;
    OsMutex mutex;
    NNSFndList commands;
} NNSiSndArcStrmThread;

typedef char NNSiSndArcStrmPlayerSizeCheck[(sizeof(NNSiSndArcStrmPlayer) == 348) ? 1 : -1];
typedef char NNSiSndArcStrmCommandSizeCheck[(sizeof(NNSiSndArcStrmCommand) == 48) ? 1 : -1];
typedef char NNSiSndArcStrmThreadSizeCheck[(sizeof(NNSiSndArcStrmThread) == 1208) ? 1 : -1];

extern NNSiSndArcStrmThread *data_02061e78;
extern NNSFndList data_02061e80;
extern NNSiSndArcStrmThread data_02062220;
extern OsMutex data_020626b8;
extern NNSFndList data_020626cc;
extern NNSiSndArcStrmPlayer data_020626d8[4];

void NNSi_SndArcStrmThreadMain(void *argument);
void NNSi_SndArcStrmFreeCommand(NNSiSndArcStrmCommand *command);
NNSiSndArcStrmCommand *NNSi_SndArcStrmAllocCommand(void);
NNSiSndArcStrmCommand *NNSi_SndArcStrmPopCommand(NNSFndList *list);
void NNSi_SndArcStrmRemoveCommands(NNSFndList *list, NNSiSndArcStrmPlayer *player);
void NNSi_SndArcStrmCreateThread(NNSiSndArcStrmThread *thread, u32 priority);
void NNSi_SndArcStrmReleaseChannel(NNSiSndArcStrmPlayer *player);
int NNSi_SndArcStrmAllocChannel(NNSiSndArcStrmPlayer *player, int count, const u8 *channels);
void NNSi_SndArcStrmClose(NNSiSndArcStrmPlayer *player);
void NNSi_SndArcStrmForceStop(NNSiSndArcStrmPlayer *player);
void NNSi_SndArcStrmStop(NNSiSndArcStrmPlayer *player, int frames);
void NNSi_SndArcStrmRelease(NNSiSndArcStrmPlayer *player);
NNSiSndArcStrmPlayer *NNSi_SndArcStrmAlloc(NNSSndStrmHandle *handle, int index, int priority);
void NNSi_SndArcStrmMain(void);
void NNS_SndStrmHandleRelease(NNSSndStrmHandle *handle);
void NNS_SndStrmHandleInit(NNSSndStrmHandle *handle);
void NNS_SndArcStrmStop(NNSSndStrmHandle *handle, int frames);
void NNS_SndArcStrmStartPrepared(NNSSndStrmHandle *handle);
int NNS_SndArcStrmStart(NNSSndStrmHandle *handle, int stream, u32 offset_ms);
int NNS_SndArcStrmPrepare(NNSSndStrmHandle *handle, int stream, u32 offset_ms);
int NNSi_SndArcStrmPrepare(NNSSndStrmHandle *handle, NNSSndArcStrmInfo *info, int player, int priority, int stream, u32 offset_ms, NNSSndStrmCallback callback, void *argument, NNSSndArcStrmCallback archive_callback, void *archive_argument);
int NNS_SndArcStrmSetupPlayer(NNSSndHeap *heap);
void NNSi_SndArcStrmDisposeBuffer(void *memory, u32 size, u32 player, u32 argument);
void NNSi_SndArcStrmCallback(int reason, int channels, void **buffers, u32 size, int format, void *argument);
void NNSi_SndArcStrmNextStream(NNSiSndArcStrmPlayer *player);
void NNS_SndArcStrmInit(u32 priority, NNSSndHeap *heap);

extern void func_02039ecc(OsMutex *mutex);
extern void func_02039e5c(OsMutex *mutex);
extern void func_02039f58(OsMutex *mutex);
extern void func_020395d0(OsThread *thread, void (*entry)(void *), void *argument, void *stack, u32 size, u32 priority);
extern void OS_WakeupThreadDirect(OsThread *thread);
extern void FS_CancelFile(FsFile *file);

#endif
