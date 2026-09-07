#include <nns/snd_arc_stream.h>

extern int data_02061e74;
extern void *data_02061e7c;
extern OsMutex data_02061e8c;
extern NNSiSndArcStrmCommand data_02061ea0[8];
extern u8 data_02062020[];

void NNS_SndArcStrmInit(u32 priority, NNSSndHeap *heap)
{
    int i;
    NNSiSndArcStrmCommand *command;
    NNSiSndArcStrmPlayer *player;
    if (data_02061e74) {
        NNS_SndArcStrmSetupPlayer(heap);
        return;
    }
    data_02061e74 = 1;
    NNS_FndInitList(&data_02061e80, 0);
    command = data_02061ea0;
    for (i = 0; i < 8; ++i, ++command) NNS_FndAppendListObject(&data_02061e80, command);
    func_02039f58(&data_02061e8c);
    player = data_020626d8;
    data_02061e7c = data_02062020;
    for (i = 0; i < 4; ++i, ++player) {
        player->active = 0;
        FS_InitFile(&player->file);
        NNS_SndStrmInit(&player->stream);
        player->player_index = i;
        player->channel_count = 0;
        player->buffer = 0;
        player->buffer_size = 0;
        player->channel_refs = 0;
    }
    NNS_SndArcStrmSetupPlayer(heap);
    NNSi_SndArcStrmCreateThread(&data_02062220, priority);
}
