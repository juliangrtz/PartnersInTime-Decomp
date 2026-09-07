#include <nitro/card.h>

/* The ITCM lower bound aliases its first resident function. */
extern void func_01ff8000(void);

int CARDi_TryReadCardDma(CardRomStat *stat) {
    CardWork *work = CARDi_GetCommon();
    u32 size;
    int eligible;
    int aligned;
    int accessible;
    u32 destination;
    int dma_valid;
    eligible = 0;
    destination = (u32)work->destination;
    aligned = 0;
    accessible = 0;
    dma_valid = 0;
    if (!(destination & 3) && (u32)work->dma <= 3) dma_valid = 1;
    if (dma_valid) {
        u32 dtcm;
        int in_tcm;
        u32 end;
        int in_itcm;
        asm { ldr size, [work, #0x20] }
        dtcm = OS_GetDTCMAddress();
        in_tcm = 1;
        in_itcm = 0;
        end = destination + size;
        if (end > (u32)func_01ff8000 && destination < 0x02000000) in_itcm = 1;
        if (!in_itcm && (dtcm >= end || dtcm + 0x4000 <= destination)) in_tcm = 0;
        if (!in_tcm) accessible = 1;
    }
    if (accessible && !(((u32)work->source | work->length) & 511)) aligned = 1;
    if (aligned && work->length) eligible = 1;
    stat->control = (*(vu32 *)0x027FFE60 & 0xF8FFFFFF) | 0xA1000000;
    if (eligible) {
        u32 state = OS_DisableInterrupts();
        DC_StoreRange(work->destination, work->length);
        DC_InvalidateRange(work->destination, work->length);
        IC_InvalidateRange(work->destination, work->length);
        OS_SetIrqFunction(0x80000, CARDi_OnReadCard);
        OS_ResetRequestIrqMask(0x80000);
        OS_EnableIrqMask(0x80000);
        DC_WaitWriteBufferEmpty();
        OS_RestoreInterrupts(state);
        CARDi_SetCardDma();
    }
    return eligible;
}
