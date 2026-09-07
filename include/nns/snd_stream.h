#ifndef NNS_SND_STREAM_H
#define NNS_SND_STREAM_H

#include <nns/snd.h>
#include <nitro/pm.h>
#include <nitro/snd_command.h>

typedef void (*NNSSndStrmCallback)(int reason, int channels, void **buffers, u32 size, int format, void *argument);
typedef struct NNSSndStrm {
    NNSFndLink link;
    PmSleepCallback pre_sleep;
    PmSleepCallback post_sleep;
    int format;
    signed int active : 1;
    signed int started : 1;
    signed int reserved_flags : 30;
    u32 buffer_size;
    int block_count;
    NNSSndStrmCallback callback;
    void *argument;
    int block;
    int volume_db;
    int alarm;
    u32 channel_mask;
    int channel_count;
    u8 channels[16];
} NNSSndStrm;

typedef struct NNSiSndStrmChannel { u8 *buffer; int volume_db; } NNSiSndStrmChannel;
typedef char NNSSndStrmSizeCheck[(sizeof(NNSSndStrm) == 92) ? 1 : -1];

void NNS_SndStrmInit(NNSSndStrm *stream);
int NNS_SndStrmAllocChannel(NNSSndStrm *stream, int count, const u8 *channels);
void NNS_SndStrmFreeChannel(NNSSndStrm *stream);
int NNS_SndStrmSetup(NNSSndStrm *stream, int format, void *buffer, u32 size, u32 timer, int blocks, NNSSndStrmCallback callback, void *argument);
void NNS_SndStrmStart(NNSSndStrm *stream);
void NNS_SndStrmStop(NNSSndStrm *stream);
void NNS_SndStrmSetVolume(NNSSndStrm *stream, int volume);
void NNS_SndStrmSetChannelPan(NNSSndStrm *stream, int index, int pan);
void NNSi_SndStrmShutdown(NNSSndStrm *stream);
void NNSi_SndStrmRelease(NNSSndStrm *stream);
void NNSi_SndStrmAlarm(void *argument);
void NNSi_SndStrmCallback(NNSSndStrm *stream, int reason);
void NNSi_SndStrmPreSleep(void *argument);
void NNSi_SndStrmPostSleep(void *argument);

#endif
