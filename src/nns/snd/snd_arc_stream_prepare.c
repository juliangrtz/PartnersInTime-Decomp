#include <nns/snd_arc_stream.h>

int NNSi_SndArcStrmPrepare(NNSSndStrmHandle *handle, NNSSndArcStrmInfo *info,
                         int index, int priority, int stream, u32 offset_ms,
                         NNSSndStrmCallback callback, void *argument,
                         NNSSndArcStrmCallback archive_callback, void *archive_argument)
{
    NNSiSndArcStrmPlayer *player;
    int format;
    int channels;
    FsFileId file_id;
    player = NNSi_SndArcStrmAlloc(handle, index, priority);
    if (!player) return 0;
    if (NNS_SndArcReadFile(info->file, &player->header, 64, 0) != 64) {
        NNSi_SndArcStrmRelease(player);
        return 0;
    }
    file_id = NNS_SndArcGetFileID();
    if (!FS_OpenFileFast(&player->file, file_id)) {
        NNSi_SndArcStrmRelease(player);
        return 0;
    }
    player->file_offset = NNS_SndArcGetFileOffset(info->file);
    player->sample_offset = player->header.sample_rate * offset_ms / 1000;
    if (player->sample_offset && player->header.format == 2) player->seek_adpcm = 1;
    else player->seek_adpcm = 0;
    player->blocks = 4;
    player->finished = 0;
    player->playing = 0;
    player->prepared = 0;
    player->start_pending = 0;
    player->fading = 0;
    player->pending = 0;
    player->callback = callback;
    player->argument = argument;
    player->archive_callback = archive_callback;
    player->archive_argument = archive_argument;
    player->stream_index = stream;
    player->volume_db = 0;
    player->volume = info->volume;
    NNSi_SndFaderInit(&player->fader);
    NNSi_SndFaderSet(&player->fader, 32512, 1);
    /* STRM files use PCM8, PCM16 or ADPCM. */
    switch (player->header.format) {
    case 0: format = 0; break;
    case 1:
    case 2: format = 1; break;
    }
    channels = player->header.channels;
    if (info->flags & 1) channels = 2;
    if (channels > player->channel_count) channels = player->channel_count;
    player->mono = channels == 1;
    if (!NNSi_SndArcStrmAllocChannel(player, channels, player->channels)) {
        FS_CloseFile(&player->file);
        NNSi_SndArcStrmRelease(player);
        return 0;
    }
    if (!NNS_SndStrmSetup(&player->stream, format, player->buffer,
                         player->buffer_size * channels / player->channel_count,
                         player->header.timer, 4, NNSi_SndArcStrmCallback, player)) {
        NNSi_SndArcStrmReleaseChannel(player);
        FS_CloseFile(&player->file);
        NNSi_SndArcStrmRelease(player);
        return 0;
    }
    if (channels == 2) {
        NNS_SndStrmSetChannelPan(&player->stream, 0, 0);
        NNS_SndStrmSetChannelPan(&player->stream, 1, 127);
    }
    return 1;
}
