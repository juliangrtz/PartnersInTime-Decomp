#ifndef NNS_SND_CAPTURE_H
#define NNS_SND_CAPTURE_H
#include <nns/snd.h>
#include <nitro/os_message.h>

typedef void (*NNSSndCaptureCallback)(void *left, void *right, u32 size, int format, void *argument);
typedef struct NNSiSndCapture {
    int active;
    int type;
    int format;
    u8 *left;
    u8 *right;
    u32 buffer_size;
    u32 block_size;
    int block;
    u32 locked_channels;
    u32 playing_channels;
    u32 capture_mask;
    int alarm;
    int block_count;
    NNSSndCaptureCallback callback;
    void *argument;
    NNSiSndFader fader;
    int stopping;
    int volume;
} NNSiSndCapture;

typedef struct NNSiSndCaptureMessage {
    NNSiSndCapture *capture;
    u32 size;
    u32 offset;
    void *left;
    void *right;
} NNSiSndCaptureMessage;

typedef char NNSiSndCaptureSizeCheck[(sizeof(NNSiSndCapture) == 84) ? 1 : -1];
typedef char NNSiSndCaptureMessageSizeCheck[(sizeof(NNSiSndCaptureMessage) == 20) ? 1 : -1];

void NNSi_SndCaptureThread(void *argument);
void NNSi_SndCaptureAlarm(void *argument);
void NNSi_SndCapturePostSleep(void);
void NNSi_SndCapturePreSleep(void);
void NNSi_SndCaptureStop(void);
int NNSi_SndCaptureStart(int type, void *left, void *right, u32 size, int format, u32 input, u32 output, u32 loop, int rate, u32 volume, u32 left_pan, u32 right_pan, int blocks, NNSSndCaptureCallback callback, void *argument);
void NNSi_SndCaptureMain(void);
void NNSi_SndCaptureInit(void);
void NNS_SndCaptureCreateThread(int priority);
void NNS_SndCaptureStopEffect(void);
int NNS_SndCaptureStartEffect(void *buffer, u32 size, int format, int rate, int blocks, NNSSndCaptureCallback callback, void *argument);
void NNS_SndCaptureStopReverb(int frames);
int NNS_SndCaptureStartReverb(void *buffer, u32 size, int format, int rate, int volume);
#endif
