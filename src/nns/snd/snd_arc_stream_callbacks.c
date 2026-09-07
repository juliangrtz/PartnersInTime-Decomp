#include <nns/snd_arc_stream.h>

void NNSi_SndArcStrmDisposeBuffer(void *memory, u32 size, u32 user0, u32 user1)
{
    NNSiSndArcStrmPlayer *player = (NNSiSndArcStrmPlayer *)user0;
    if (memory == player->buffer) {
        func_02039ecc(&data_020626b8);
        if (data_02061e78) func_02039ecc(&data_02061e78->mutex);
        NNSi_SndArcStrmForceStop(player);
        player->buffer = 0;
        player->buffer_size = 0;
        player->channel_count = 0;
        if (player->channel_refs > 0) {
            NNS_SndStrmFreeChannel(&player->stream);
            player->channel_refs = 0;
        }
        func_02039e5c(&data_020626b8);
        if (data_02061e78) func_02039e5c(&data_02061e78->mutex);
    }
}

void NNSi_SndArcStrmCallback(int reason, int channels, void **buffers, u32 size, int format, void *argument)
{
    NNSiSndArcStrmPlayer *player = argument;
    NNSiSndArcStrmCommand *command;
    NNSiSndArcStrmThread *thread;
    int i;
    if (player->pending >= 2) {
        command = NNS_FndGetNextListObject(&data_020626cc, 0);
        while (command) {
            if (command->player == player) break;
            command = NNS_FndGetNextListObject(&data_020626cc, command);
        }
        for (i = 0; i < command->channels; ++i) MI_CpuFill8(command->buffers[i], 0, command->size);
        NNS_FndRemoveListObject(&data_020626cc, command);
        --player->pending;
        NNSi_SndArcStrmFreeCommand(command);
    }
    command = NNSi_SndArcStrmAllocCommand();
    command->player = player;
    command->reason = reason;
    command->channels = channels;
    for (i = 0; i < channels; ++i) command->buffers[i] = buffers[i];
    command->size = size;
    thread = &data_02062220;
    if (!reason && data_02061e78) thread = data_02061e78;
    ++player->pending;
    NNS_FndAppendListObject(&thread->commands, command);
    OS_WakeupThread(&thread->queue);
}

void NNSi_SndArcStrmNextStream(NNSiSndArcStrmPlayer *player)
{
    int old_format;
    int old_rate;
    NNSSndArcStrmInfo *info;
    NNSiSndArcStrmRequest request;
    NNSiSndArcStrmResponse response;
    request.player = player->player_index;
    request.stream = player->stream_index;
    response.stream = player->stream_index;
    response.offset_ms = 0;
    if (!player->archive_callback(0, &request, &response, player->archive_argument)) return;
    info = NNS_SndArcGetStrmInfo(response.stream);
    if (!info) return;
    old_format = player->header.format;
    old_rate = player->header.sample_rate;
    player->file_offset = NNS_SndArcGetFileOffset(info->file);
    NNS_SndArcReadFile(info->file, &player->header, 64, 0);
    if (old_rate != player->header.sample_rate) return;
    if (!old_format && player->header.format) return;
    if (old_format && !player->header.format) return;
    player->stream_index = response.stream;
    player->sample_offset = player->header.sample_rate * response.offset_ms / 1000;
    if (player->sample_offset && player->header.format == 2) player->seek_adpcm = 1;
    else player->seek_adpcm = 0;
    player->finished = 0;
}
