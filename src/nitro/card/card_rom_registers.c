#include <nitro/card.h>

#define CARD_CONTROL (*(vu32 *)0x040001A4)

int CARDi_ReadFromCache(CardRomStat *stat) {
    CardWork *work = CARDi_GetCommon();
    u32 address = (u32)work->source & ~511;
    if (address == stat->cache_address) {
        u32 offset = (u32)work->source - address;
        u32 size = 512 - offset;
        u32 remaining = work->length;
        if (size > remaining) size = remaining;
        MI_CpuCopy8(stat->buffer + offset, work->destination, size);
        work->source += size;
        work->destination += size;
        work->length -= size;
    }
    return work->length != 0;
}

void CARDi_EndTask(void) {
    CardWork *work = CARDi_GetCommon();
    CardCallback callback = work->callback;
    void *argument = work->argument;
    u32 state;
    work->command->result = 0;
    state = OS_DisableInterrupts();
    work->flags &= ~12;
    OS_WakeupThread(&work->task_queue);
    if (work->flags & 16) OS_WakeupThreadDirect(&work->thread);
    OS_RestoreInterrupts(state);
    if (callback) callback(argument);
}

void CARDi_SetRomOp(u32 high, u32 low) {
    while (CARD_CONTROL & 0x80000000) {}
    *(vu8 *)0x040001A1 = 0xC0;
    *(vu8 *)0x040001A8 = high >> 24;
    *(vu8 *)0x040001A9 = high >> 16;
    *(vu8 *)0x040001AA = high >> 8;
    *(vu8 *)0x040001AB = high;
    *(vu8 *)0x040001AC = low >> 24;
    *(vu8 *)0x040001AD = low >> 16;
    *(vu8 *)0x040001AE = low >> 8;
    *(vu8 *)0x040001AF = low;
}

void CARDi_SetCardDma(void) {
    CardWork *work = CARDi_GetCommon();
    u32 source;
    MIi_CardDmaCopy32(work->dma, (void *)0x04100010, work->destination, 512);
    source = (u32)work->source;
    CARDi_SetRomOp((source >> 8) | 0xB7000000, source << 24);
    CARD_CONTROL = rom_stat.control;
}
