#include <nitro/card.h>

void CARDi_ReadRomSyncCore(CardWork *work) {
    CardRomStat *stat = &rom_stat;
    if (CARDi_ReadFromCache(stat)) stat->read(stat);
    CARDi_EndTask();
}

void CARDi_ReadRom(u32 dma, u32 source, void *destination, u32 size,
    CardCallback callback, void *argument, int asynchronous) {
    CardWork *work = CARDi_GetCommon();
    CardRomStat *stat = &rom_stat;
    u32 state;
    CARD_CheckEnabled();
    state = OS_DisableInterrupts();
    while (work->flags & 4) OS_SleepThread(&work->task_queue);
    work->flags |= 4;
    OS_RestoreInterrupts(state);
    work->dma = dma;
    work->source = (u8 *)(source + cardi_rom_base);
    work->destination = destination;
    work->length = size;
    work->callback = callback;
    work->argument = argument;
    if (dma <= 3) MI_StopDma(dma);
    if (CARDi_TryReadCardDma(stat)) {
        if (!asynchronous) CARD_WaitRomAsync();
    } else if (asynchronous) CARDi_SetTask(CARDi_ReadRomSyncCore);
    else {
        work->current_thread = OSi_ThreadInfo.current;
        CARDi_ReadRomSyncCore(work);
    }
}

int CARD_WaitRomAsync(void) { return CARDi_WaitAsync(); }

void CARD_Init(void) {
    CardWork *work = CARDi_GetCommon();
    if (!work->flags) {
        work->flags = 1;
        work->source = work->destination = (u8 *)(work->length = 0);
        work->dma = -1;
        work->callback = 0;
        work->argument = 0;
        cardi_rom_base = 0;
        CARDi_InitCommon();
        rom_stat.read = CARDi_ReadCard;
        CARDi_InitPulledOutCallback();
    }
}
