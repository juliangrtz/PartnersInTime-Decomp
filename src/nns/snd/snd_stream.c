#include <nns/snd_stream.h>

extern int data_020617a0;
extern NNSFndList data_020617a4;
extern void *data_020617b0[16];
extern NNSiSndStrmChannel data_020617f0[16];
extern void func_0203c120(u32 channels, u32 captures, u32 alarms, u32 flags);
extern void func_0203c0b0(u32 channels, u32 captures, u32 alarms, u32 flags);
extern u32 func_0203c570(void);
extern void func_0203c5b0(u32 command);
extern void func_0203c020(u32 alarm, u32 first, u32 period, SoundCallback callback, void *argument);
extern void func_0203bf10(u32 channel, u32 format, u32 buffer, u32 loop, u32 loop_start, u32 words, u32 volume, u32 shift, u32 timer, u32 pan);
extern void func_0203bf68(u32 channels, u32 pan);
extern void func_0203bf94(u32 channels, u32 volume, u32 shift);
extern u16 func_0203ccf8(int volume_db);
extern void PM_PrependPreSleepCallback(PmSleepCallback *callback);
extern void PM_AppendPostSleepCallback(PmSleepCallback *callback);
extern void PM_DeletePreSleepCallback(PmSleepCallback *callback);
extern void PM_DeletePostSleepCallback(PmSleepCallback *callback);

void NNS_SndStrmInit(NNSSndStrm *stream)
{
    if (!data_020617a0) {
        NNS_FndInitList(&data_020617a4, 0);
        data_020617a0 = 1;
    }
    stream->pre_sleep.callback = NNSi_SndStrmPreSleep;
    stream->pre_sleep.argument = stream;
    stream->post_sleep.callback = NNSi_SndStrmPostSleep;
    stream->post_sleep.argument = stream;
    stream->channel_mask = 0;
    stream->channel_count = 0;
    stream->active = 0;
    stream->started = 0;
}

int NNS_SndStrmAllocChannel(NNSSndStrm *stream, int count, const u8 *channels)
{
    u32 mask = 0;
    int index;
    for (index = 0; index < count; index++) {
        stream->channels[index] = channels[index];
        mask |= 1 << channels[index];
    }
    if (!NNSi_SndLockChannel(mask)) return 0;
    stream->channel_count = count;
    stream->channel_mask = mask;
    return 1;
}

void NNS_SndStrmFreeChannel(NNSSndStrm *stream)
{
    if (stream->channel_mask) {
        NNSi_SndUnlockChannel(stream->channel_mask);
        stream->channel_mask = 0;
        stream->channel_count = 0;
    }
}

int NNS_SndStrmSetup(NNSSndStrm *stream, int format, void *buffer, u32 size, u32 timer, int blocks, NNSSndStrmCallback callback, void *argument)
{
    u32 interval;
    u32 interrupts;
    int index;
    u32 samples;
    if (stream->active) NNS_SndStrmStop(stream);
    samples = size / (stream->channel_count * (blocks << 5));
    samples = samples * blocks;
    stream->buffer_size = samples << 5;
    samples = stream->buffer_size;
    if (format == 1) samples >>= 1;
    interval = (timer * samples) / blocks;
    stream->alarm = NNSi_SndAllocAlarm();
    if (stream->alarm < 0) return 0;
    for (index = 0; index < stream->channel_count; index++) {
        int channel = stream->channels[index];
        data_020617f0[channel].buffer = (u8 *)buffer + stream->buffer_size * index;
        data_020617f0[channel].volume_db = 0;
        func_0203bf10(channel, format, (u32)data_020617f0[channel].buffer, 1, 0, stream->buffer_size >> 2, 127, 0, timer << 5, 64);
    }
    func_0203c020(stream->alarm, interval, interval, NNSi_SndStrmAlarm, stream);
    NNS_FndAppendListObject(&data_020617a4, stream);
    stream->format = format;
    stream->block_count = blocks;
    stream->callback = callback;
    stream->argument = argument;
    stream->block = 0;
    stream->volume_db = 0;
    stream->active = 1;
    interrupts = OS_DisableInterrupts();
    stream->block_count = 1;
    NNSi_SndStrmCallback(stream, 0);
    stream->block_count = blocks;
    OS_RestoreInterrupts(interrupts);
    return 1;
}

void NNS_SndStrmStart(NNSSndStrm *stream)
{
    func_0203c120(stream->channel_mask, 0, 1 << stream->alarm, 0);
    if (stream->started) return;
    PM_PrependPreSleepCallback(&stream->pre_sleep);
    PM_AppendPostSleepCallback(&stream->post_sleep);
    stream->started = 1;
}

void NNS_SndStrmStop(NNSSndStrm *stream)
{
    if (!stream->active) return;
    NNSi_SndStrmShutdown(stream);
}

void NNS_SndStrmSetVolume(NNSSndStrm *stream, int volume)
{
    int channel;
    int index;
    stream->volume_db = volume;
    for (index = 0; index < stream->channel_count; index++) {
        int encoded;
        channel = stream->channels[index];
        encoded = func_0203ccf8(stream->volume_db + data_020617f0[channel].volume_db);
        func_0203bf94(1 << channel, encoded & 255, encoded >> 8);
    }
}

void NNS_SndStrmSetChannelPan(NNSSndStrm *stream, int index, int pan)
{
    if (index > stream->channel_count - 1) return;
    func_0203bf68(1 << stream->channels[index], pan);
}

void NNSi_SndStrmShutdown(NNSSndStrm *stream)
{
    u32 command;
    if (stream->started) {
        func_0203c0b0(stream->channel_mask, 0, 1 << stream->alarm, 0);
        PM_DeletePreSleepCallback(&stream->pre_sleep);
        PM_DeletePostSleepCallback(&stream->post_sleep);
        stream->started = 0;
        command = func_0203c570();
        func_0203c640(1);
        func_0203c5b0(command);
    }
    NNSi_SndStrmRelease(stream);
}

void NNSi_SndStrmRelease(NNSSndStrm *stream)
{
    NNSi_SndFreeAlarm(stream->alarm);
    NNS_FndRemoveListObject(&data_020617a4, stream);
    stream->active = 0;
}

void NNSi_SndStrmAlarm(void *argument)
{
    NNSi_SndStrmCallback(argument, 1);
}

void NNSi_SndStrmCallback(NNSSndStrm *stream, int reason)
{
    u32 block_size;
    u32 offset;
    int index;
    block_size = stream->buffer_size / stream->block_count;
    offset = block_size * stream->block;
    for (index = 0; index < stream->channel_count; index++) {
        u32 buffer = (u32)data_020617f0[stream->channels[index]].buffer;
        buffer += offset;
        data_020617b0[index] = (void *)buffer;
    }
    stream->callback(reason, stream->channel_count, data_020617b0, block_size, stream->format, stream->argument);
    stream->block++;
    if (stream->block >= stream->block_count) stream->block = 0;
}

void NNSi_SndStrmPreSleep(void *argument)
{
    NNSSndStrm *stream = argument;
    u32 command;
    if (!stream->started) return;
    func_0203c0b0(stream->channel_mask, 0, 1 << stream->alarm, 0);
    command = func_0203c570();
    func_0203c640(1);
    func_0203c5b0(command);
}

void NNSi_SndStrmPostSleep(void *argument)
{
    NNSSndStrm *stream = argument;
    if (!stream->started) return;
    while (stream->block) {
        u32 interrupts = OS_DisableInterrupts();
        NNSi_SndStrmCallback(stream, 1);
        OS_RestoreInterrupts(interrupts);
    }
    func_0203c120(stream->channel_mask, 0, 1 << stream->alarm, 0);
}
