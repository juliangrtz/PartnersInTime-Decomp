#include <nns/snd_arc_stream.h>

extern const s16 data_0204fb34[128];

int NNS_SndArcStrmSetupPlayer(NNSSndHeap *heap)
{
    int i;
    u32 size;
    NNSiSndArcStrmPlayer *player = data_020626d8;
    for (i = 0; i < 4; ++i, ++player) {
        NNSSndArcStrmPlayerInfo *info = NNS_SndArcGetStrmPlayerInfo(i);
        if (info) {
            int channel;
            player->channel_count = info->count;
            for (channel = 0; channel < info->count; ++channel) player->channels[channel] = info->channels[channel];
            if (heap) {
                void *buffer;
                size = player->channel_count << 11;
                buffer = NNS_SndHeapAlloc(heap, size, NNSi_SndArcStrmDisposeBuffer, (u32)player, 0);
                if (!buffer) return 0;
                NNSi_SndArcStrmForceStop(player);
                player->buffer = buffer;
                player->buffer_size = size;
            }
        }
    }
    return 1;
}

int NNS_SndArcStrmPrepare(NNSSndStrmHandle *handle, int stream, u32 offset_ms)
{
    NNSSndArcStrmInfo *info = NNS_SndArcGetStrmInfo(stream);
    if (info) return NNSi_SndArcStrmPrepare(handle, info, info->player, info->priority, stream, offset_ms, 0, 0, 0, 0);
    return 0;
}

void NNS_SndArcStrmStartPrepared(NNSSndStrmHandle *handle)
{
    if (handle->player) handle->player->start_pending = 1;
}

int NNS_SndArcStrmStart(NNSSndStrmHandle *handle, int stream, u32 offset_ms)
{
    if (!NNS_SndArcStrmPrepare(handle, stream, offset_ms)) return 0;
    NNS_SndArcStrmStartPrepared(handle);
    return 1;
}

void NNS_SndArcStrmStop(NNSSndStrmHandle *handle, int frames)
{
    if (handle->player) NNSi_SndArcStrmStop(handle->player, frames);
}

void NNS_SndStrmHandleInit(NNSSndStrmHandle *handle)
{
    handle->player = 0;
}

void NNS_SndStrmHandleRelease(NNSSndStrmHandle *handle)
{
    if (handle->player) {
        handle->player->handle = 0;
        handle->player = 0;
    }
}

void NNSi_SndArcStrmMain(void)
{
    int i;
    int initial_volume;
    NNSiSndArcStrmPlayer *player = data_020626d8;
    for (i = 0; i < 4; ++i, ++player) {
        if (player->active) {
            if (!player->blocks) {
                NNSi_SndArcStrmForceStop(player);
            } else {
                if (player->start_pending && player->prepared) {
                    NNS_SndStrmStart(&player->stream);
                    player->playing = 1;
                    player->start_pending = 0;
                }
                if (player->playing) {
                    int volume;
                    NNSi_SndFaderUpdate(&player->fader);
                    initial_volume = data_0204fb34[player->volume];
                    volume = data_0204fb34[NNSi_SndFaderGet(&player->fader) >> 8];
                    volume += initial_volume;
                    if (volume != player->volume_db) {
                        NNS_SndStrmSetVolume(&player->stream, volume);
                        player->volume_db = volume;
                    }
                    if (player->fading && NNSi_SndFaderIsFinished(&player->fader)) NNSi_SndArcStrmForceStop(player);
                }
            }
        }
    }
}

NNSiSndArcStrmPlayer *NNSi_SndArcStrmAlloc(NNSSndStrmHandle *handle, int index, int priority)
{
    NNSiSndArcStrmPlayer *player;
    if (handle->player) NNS_SndStrmHandleRelease(handle);
    player = &data_020626d8[index];
    if (!player->buffer) return 0;
    if (player->active) {
        if (priority < player->priority) return 0;
        NNSi_SndArcStrmForceStop(player);
    }
    player->priority = priority;
    player->active = 1;
    player->handle = handle;
    handle->player = player;
    return player;
}

void NNSi_SndArcStrmRelease(NNSiSndArcStrmPlayer *player)
{
    if (player->handle) {
        player->handle->player = 0;
        player->handle = 0;
    }
    player->active = 0;
    player->start_pending = 0;
    player->playing = 0;
}
