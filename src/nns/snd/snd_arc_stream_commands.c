#include <nns/snd_arc_stream.h>

void NNSi_SndArcStrmStop(NNSiSndArcStrmPlayer *player, int frames)
{
    if (!player->playing) {
        NNSi_SndArcStrmForceStop(player);
        return;
    }
    if (!frames) {
        NNSi_SndArcStrmForceStop(player);
        return;
    }
    NNSi_SndFaderSet(&player->fader, 0, frames);
    player->fading = 1;
    player->priority = 0;
}

void NNSi_SndArcStrmForceStop(NNSiSndArcStrmPlayer *player)
{
    func_02039ecc(&data_020626b8);
    if (data_02061e78) func_02039ecc(&data_02061e78->mutex);
    if (player->playing) NNS_SndStrmStop(&player->stream);
    if (player->active) FS_CancelFile(&player->file);
    NNSi_SndArcStrmClose(player);
    func_02039e5c(&data_020626b8);
    if (data_02061e78) func_02039e5c(&data_02061e78->mutex);
}

void NNSi_SndArcStrmClose(NNSiSndArcStrmPlayer *player)
{
    if (player->active) {
        NNSi_SndArcStrmReleaseChannel(player);
        FS_CloseFile(&player->file);
        NNSi_SndArcStrmRemoveCommands(&data_020626cc, player);
        if (data_02061e78) NNSi_SndArcStrmRemoveCommands(&data_02061e78->commands, player);
        NNSi_SndArcStrmRelease(player);
    }
}

int NNSi_SndArcStrmAllocChannel(NNSiSndArcStrmPlayer *player, int count, const u8 *channels)
{
    if (!player->channel_refs && !NNS_SndStrmAllocChannel(&player->stream, count, channels)) return 0;
    ++player->channel_refs;
    return 1;
}

void NNSi_SndArcStrmReleaseChannel(NNSiSndArcStrmPlayer *player)
{
    if (player->channel_refs) {
        --player->channel_refs;
        if (!player->channel_refs) NNS_SndStrmFreeChannel(&player->stream);
    }
}

void NNSi_SndArcStrmCreateThread(NNSiSndArcStrmThread *thread, u32 priority)
{
    func_020395d0(&thread->thread, NNSi_SndArcStrmThreadMain, thread, &thread->queue, 1024, priority);
    NNS_FndInitList(&thread->commands, 0);
    func_02039f58(&thread->mutex);
    thread->queue = 0;
    OS_WakeupThreadDirect(&thread->thread);
}

void NNSi_SndArcStrmRemoveCommands(NNSFndList *list, NNSiSndArcStrmPlayer *player)
{
    u32 interrupts = OS_DisableInterrupts();
    NNSiSndArcStrmCommand *command = NNS_FndGetNextListObject(list, 0);
    while (command) {
        NNSiSndArcStrmCommand *next = NNS_FndGetNextListObject(list, command);
        if (command->player == player) {
            NNS_FndRemoveListObject(list, command);
            NNSi_SndArcStrmFreeCommand(command);
        }
        command = next;
    }
    OS_RestoreInterrupts(interrupts);
}

NNSiSndArcStrmCommand *NNSi_SndArcStrmPopCommand(NNSFndList *list)
{
    u32 interrupts = OS_DisableInterrupts();
    NNSiSndArcStrmCommand *command = NNS_FndGetNextListObject(list, 0);
    if (command) {
        NNS_FndRemoveListObject(list, command);
        --command->player->pending;
    }
    OS_RestoreInterrupts(interrupts);
    return command;
}

NNSiSndArcStrmCommand *NNSi_SndArcStrmAllocCommand(void)
{
    u32 interrupts = OS_DisableInterrupts();
    NNSiSndArcStrmCommand *command = NNS_FndGetNextListObject(&data_02061e80, 0);
    if (command) NNS_FndRemoveListObject(&data_02061e80, command);
    OS_RestoreInterrupts(interrupts);
    return command;
}

void NNSi_SndArcStrmFreeCommand(NNSiSndArcStrmCommand *command)
{
    u32 interrupts = OS_DisableInterrupts();
    NNS_FndAppendListObject(&data_02061e80, command);
    OS_RestoreInterrupts(interrupts);
}
