#include <nns/snd_capture_internal.h>

void NNSi_SndCaptureStop(void)
{
    NNSiSndCapture *capture = NNSi_SndCaptureGetWork();
    int has_alarm;
    if (!capture->active) return;
    has_alarm = capture->alarm >= 0;
    func_0203c0b0(capture->playing_channels, capture->capture_mask, has_alarm ? 1 << capture->alarm : 0, 0);
    if (has_alarm) {
        u32 command = func_0203c570();
        func_0203c640(1);
        func_0203c5b0(command);
        while (func_02039bb4(&data_02061878, 0, 0)) {}
    }
    if (capture->capture_mask) NNSi_SndFreeCapture(capture->capture_mask);
    if (capture->locked_channels) NNSi_SndUnlockChannel(capture->locked_channels);
    if (has_alarm) NNSi_SndFreeAlarm(capture->alarm);
    if (capture->type == 1) func_0203be58(0, 0, 0, 0);
    capture->active = 0;
}

void NNSi_SndCapturePreSleep(void)
{
    NNSiSndCapture *capture = NNSi_SndCaptureGetWork();
    u32 command;
    if (!capture->active) return;
    func_0203c0b0(capture->playing_channels, capture->capture_mask, capture->alarm >= 0 ? 1 << capture->alarm : 0, 0);
    command = func_0203c570();
    func_0203c640(1);
    func_0203c5b0(command);
}

void NNSi_SndCapturePostSleep(void)
{
    NNSiSndCapture *capture = NNSi_SndCaptureGetWork();
    if (!capture->active) return;
    capture->block = 0;
    NNSi_SndCaptureClear(capture->left, capture->buffer_size);
    NNSi_SndCaptureClear(capture->right, capture->buffer_size);
    DC_FlushRange(capture->left, capture->buffer_size);
    DC_FlushRange(capture->right, capture->buffer_size);
    func_0203c120(capture->playing_channels, capture->capture_mask, capture->alarm >= 0 ? 1 << capture->alarm : 0, 0);
}

void NNSi_SndCaptureAlarm(void *argument)
{
    NNSiSndCapture *capture = argument;
    void *left;
    void *right;
    u32 size;
    u32 offset;
    size = capture->block_size;
    offset = size * capture->block;
    left = capture->left + offset;
    right = capture->right + offset;
    if (data_02061874) {
        NNSiSndCaptureMessage *message = &data_02061994[data_02061870];
        message->capture = capture;
        message->size = size;
        message->offset = offset;
        message->left = left;
        message->right = right;
        func_02039c60(&data_02061878, message, 0);
        if (++data_02061870 >= 8) data_02061870 = 0;
    } else {
        DC_InvalidateRange(left, size);
        DC_InvalidateRange(right, size);
        capture->callback(left, right, size, capture->format, capture->argument);
    }
    capture->block++;
    if (capture->block >= capture->block_count) capture->block = 0;
}

void NNSi_SndCaptureThread(void *argument)
{
    NNSiSndCaptureMessage *message;
    for (;;) {
        NNSiSndCaptureMessage *current;
        func_02039bb4(&data_02061878, (void **)&message, 1);
        current = message;
        DC_InvalidateRange(current->left, current->size);
        DC_InvalidateRange(current->right, current->size);
        current->capture->callback(current->left, current->right, current->size, current->capture->format, current->capture->argument);
    }
}
