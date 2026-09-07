#include <nitro/card.h>

extern void OS_UnlockCard(u16 lock_id);
extern void OS_LockCard(u16 lock_id);

void CARDi_FifoCallback(u32 tag, u32 data, int error) {
    if (tag == 11) {
        if (error) {
            CardWork *work = CARDi_GetCommon();
            work->flags &= ~0x20;
            OS_WakeupThreadDirect(work->current_thread);
        }
    }
}

void CARDi_TaskThread(void *argument) {
    CardWork *work = CARDi_GetCommon();
    for (;;) {
        u32 state = OS_DisableInterrupts();
        for (;;) {
            if (work->flags & 8) break;
            work->current_thread = &work->thread;
            OS_SleepThread(0);
        }
        OS_RestoreInterrupts(state);
        work->task(work);
    }
}

void CARDi_SetTask(CardTask task) {
    CardWork *work = &cardi_common;
    u32 priority = work->priority;
    switch (priority) {
    case 32:
        priority = func_0203929c(OSi_ThreadInfo.current);
        if (priority) priority--;
        break;
    case 33:
        priority = func_0203929c(OSi_ThreadInfo.current);
        if (priority < 31) priority++;
        break;
    }
    OS_SetThreadPriority(&work->thread, priority);
    work->current_thread = &work->thread;
    work->task = task;
    work->flags |= 8;
    OS_WakeupThreadDirect(&work->thread);
}

void CARDi_LockResource(s32 lock_id, u32 resource) {
    CardWork *work = CARDi_GetCommon();
    u32 state = OS_DisableInterrupts();
    if (work->lock_owner == lock_id) {
        if (work->resource != resource) OS_Terminate();
    } else {
        while (work->lock_owner != -3) OS_SleepThread(&work->lock_queue);
        work->lock_owner = lock_id;
        work->resource = resource;
    }
    work->lock_count++;
    work->command->result = 0;
    OS_RestoreInterrupts(state);
}

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
    PXI_SetFifoRecvCallback(11, CARDi_FifoCallback);
    if (*(vu16 *)0x027FFC40 != 2) CARD_Enable(1);
}

int CARD_IsEnabled(void) { return CARDi_EnableFlag; }

void CARD_CheckEnabled(void) { if (!CARD_IsEnabled()) OS_Terminate(); }

void CARD_Enable(int enabled) { CARDi_EnableFlag = enabled; }

int CARDi_WaitAsync(void) {
    CardWork *work = CARDi_GetCommon();
    u32 state = OS_DisableInterrupts();
    while (work->flags & 4) OS_SleepThread(&work->task_queue);
    OS_RestoreInterrupts(state);
    return work->command->result == 0;
}

int CARD_IsAvailable(void) { return cardi_common.flags != 0; }

u32 CARD_GetResultCode(void) { return cardi_common.command->result; }

void CARD_LockRom(u16 lock_id) { CARDi_LockResource(lock_id, 1); OS_LockCard(lock_id); }

void CARD_UnlockRom(u16 lock_id) { OS_UnlockCard(lock_id); CARDi_UnlockResource(lock_id, 1); }

void CARD_LockBackup(u16 lock_id) { CARDi_LockResource(lock_id, 2); }

void CARD_UnlockBackup(u16 lock_id) { CARDi_UnlockResource(lock_id, 2); }
