#include <nitro/card.h>

static inline void CARDi_BeginTask(CardWork *work) {
    u32 state = OS_DisableInterrupts();
    while (work->flags & 4) OS_SleepThread(&work->task_queue);
    work->flags |= 4;
    OS_RestoreInterrupts(state);
}

void CARDi_EndBackupTask(CardWork *work) {
    CardCallback callback = work->callback;
    void *argument = work->argument;
    u32 state = OS_DisableInterrupts();
    work->flags &= ~12;
    OS_WakeupThread(&work->task_queue);
    if (work->flags & 16) OS_WakeupThreadDirect(&work->thread);
    OS_RestoreInterrupts(state);
    if (callback) callback(argument);
}

void CARDi_ReadBackup(CardWork *work) {
    u32 size;
    u8 *buffer = work->buffer;
    do {
        size = work->length;
        if (size > 256) size = 256;
        DC_InvalidateRange(buffer, 256);
        work->command->source = (u32)work->source;
        work->command->destination = (u32)buffer;
        work->command->length = size;
        if (!CARDi_Request(work, 6, 1)) break;
        MI_CpuCopy8(buffer, (void *)work->destination, size);
        work->source = (const u8 *)((u32)work->source + size);
        work->destination = work->destination + size;
        work->length = work->length - size;
    } while (work->length);
    CARDi_EndBackupTask(work);
}

void CARDi_ProgramAndVerifyBackup(CardWork *work) {
    u32 size;
    u8 *buffer = work->buffer;
    do {
        size = work->length;
        if (size > 256) size = 256;
        MI_CpuCopy8((void *)work->source, buffer, size);
        DC_FlushRange(buffer, size);
        DC_WaitWriteBufferEmpty();
        work->command->source = (u32)buffer;
        work->command->destination = (u32)work->destination;
        work->command->length = size;
        if (!CARDi_Request(work, 8, 10)) break;
        if (!CARDi_Request(work, 9, 1)) break;
        work->source = (const u8 *)((u32)work->source + size);
        work->destination = work->destination + size;
        work->length = work->length - size;
    } while (work->length);
    CARDi_EndBackupTask(work);
}

void CARDi_WriteAndVerifyBackup(CardWork *work) {
    u32 size;
    u8 *buffer = work->buffer;
    do {
        size = work->length;
        if (size > 256) size = 256;
        MI_CpuCopy8((void *)work->source, buffer, size);
        DC_FlushRange(buffer, size);
        DC_WaitWriteBufferEmpty();
        work->command->source = (u32)buffer;
        work->command->destination = (u32)work->destination;
        work->command->length = size;
        if (!CARDi_Request(work, 7, 10)) break;
        if (!CARDi_Request(work, 9, 1)) break;
        work->source = (const u8 *)((u32)work->source + size);
        work->destination = work->destination + size;
        work->length = work->length - size;
    } while (work->length);
    CARDi_EndBackupTask(work);
}

int CARD_ReadBackup(u32 source, void *destination, u32 length,
    CardCallback callback, void *argument, int asynchronous) {
    CardWork *work = CARDi_GetCommon();
    CARDi_BeginTask(work);
    work->source = (const u8 *)source;
    work->destination = (u8 *)destination;
    work->length = length;
    work->callback = callback;
    work->argument = argument;
    if (asynchronous) { CARDi_SetTask(CARDi_ReadBackup); return 1; }
    cardi_common.current_thread = OSi_ThreadInfo.current;
    CARDi_ReadBackup(work);
    return work->command->result == 0;
}

int CARD_ProgramAndVerifyBackup(u32 destination, const void *source, u32 length,
    CardCallback callback, void *argument, int asynchronous) {
    CardWork *work = CARDi_GetCommon();
    CARDi_BeginTask(work);
    work->source = (const u8 *)source;
    work->destination = (u8 *)destination;
    work->length = length;
    work->callback = callback;
    work->argument = argument;
    if (asynchronous) { CARDi_SetTask(CARDi_ProgramAndVerifyBackup); return 1; }
    cardi_common.current_thread = OSi_ThreadInfo.current;
    CARDi_ProgramAndVerifyBackup(work);
    return work->command->result == 0;
}

int CARD_WriteAndVerifyBackup(u32 destination, const void *source, u32 length,
    CardCallback callback, void *argument, int asynchronous) {
    CardWork *work = CARDi_GetCommon();
    CARDi_BeginTask(work);
    work->source = (const u8 *)source;
    work->destination = (u8 *)destination;
    work->length = length;
    work->callback = callback;
    work->argument = argument;
    if (asynchronous) { CARDi_SetTask(CARDi_WriteAndVerifyBackup); return 1; }
    cardi_common.current_thread = OSi_ThreadInfo.current;
    CARDi_WriteAndVerifyBackup(work);
    return work->command->result == 0;
}

int CARD_IdentifyBackup(int type) {
    CardWork *work = CARDi_GetCommon();
    CARD_CheckEnabled();
    CARDi_BeginTask(work);
    CARDi_SetBackupSpec(type);
    work->callback = 0;
    work->argument = 0;
    cardi_common.current_thread = OSi_ThreadInfo.current;
    CARDi_Request(work, 2, 1);
    work->command->source = 0;
    work->command->destination = (u32)work->buffer;
    work->command->length = 1;
    CARDi_Request(work, 6, 1);
    CARDi_EndBackupTask(work);
    return work->command->result == 0;
}

int CARD_WaitBackupAsync(void) { return CARDi_WaitAsync(); }
