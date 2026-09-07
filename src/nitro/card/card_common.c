#include <nitro/card.h>

void CARDi_UnlockResource(s32 lock_id, u32 resource) {
    CardWork *work = &cardi_common;
    u32 state = OS_DisableInterrupts();
    if (work->lock_owner != lock_id || !work->lock_count) {
        OS_Terminate();
    } else {
        if (work->resource != resource) OS_Terminate();
        work->lock_count--;
        if (!work->lock_count) {
            work->lock_owner = -3;
            work->resource = 0;
            OS_WakeupThread(&work->lock_queue);
        }
    }
    work->command->result = 0;
    OS_RestoreInterrupts(state);
}

void CARDi_InitCommon(void) {
    CardWork *work = &cardi_common;
    u32 region;
    volatile u32 zero;
    work->lock_owner = -3;
    work->lock_count = 0;
    work->resource = 0;
    work->command = &data_02064d60;
    zero = 0;
    MIi_CpuClearFast(zero, &data_02064d60, 64);
    DC_FlushRange(&data_02064d60, 64);
    region = OS_GetProtectionRegion2();
    OS_SetProtectionRegion2(0);
    MI_CpuCopy8(*(vu16 *)0x027FFC40 == 2 ? (void *)0x023FE940 : (void *)0x027FFE00, data_02064da0, 0x160);
    OS_SetProtectionRegion2(region);
    work->lock_queue = 0;
    work->task_queue = 0;
    work->priority = 4;
    func_020395d0(&work->thread, CARDi_TaskThread, 0, &cardi_rom_base, 1024, work->priority);
    OS_WakeupThreadDirect(&work->thread);
    func_0203d5b8(11, CARDi_FifoCallback);
    if (*(vu16 *)0x027FFC40 != 2) CARD_Enable(1);
}

int CARD_IsEnabled(void) { return CARDi_EnableFlag; }

void CARD_CheckEnabled(void) { if (!CARD_IsEnabled()) OS_Terminate(); }

void CARD_Enable(int enabled) { CARDi_EnableFlag = enabled; }
