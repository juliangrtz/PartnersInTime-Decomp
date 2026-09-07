#include <nitro/card.h>

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
