#include <nitro/fs.h>

int FS_SuspendArchive(FsArchive *archive) {
    u32 state = OS_DisableInterrupts();
    int running = FSi_TestArchiveFlag(archive, 8) == 0;
    if (running) {
        if (FSi_TestArchiveFlag(archive, 0x10)) {
            archive->flags |= 0x40;
            do { OS_SleepThread(&archive->suspend_queue); } while (FSi_TestArchiveFlag(archive, 0x40));
        } else archive->flags |= 8;
    }
    OS_RestoreInterrupts(state);
    return running;
}

int FS_ResumeArchive(FsArchive *archive) {
    FsFile *file = 0;
    u32 state = OS_DisableInterrupts();
    int running = FSi_TestArchiveFlag(archive, 8) == 0;
    if (!running) {
        archive->flags &= ~8;
        file = FSi_NextCommand(archive);
    }
    OS_RestoreInterrupts(state);
    if (file) FSi_ExecuteAsyncCommand(file);
    return running;
}

void FS_SetArchiveProc(FsArchive *archive, FsArchiveProc procedure, u32 mask) {
    if (!mask) procedure = 0;
    else if (!procedure) mask = 0;
    archive->procedure = procedure;
    archive->procedure_mask = mask;
}

void FSi_ReleaseCommand(FsFile *file, int result) {
    u32 state = OS_DisableInterrupts();
    FSi_CutFromList(&file->link);
    file->flags &= ~0x4F;
    file->result = result;
    OS_WakeupThread(&file->queue);
    OS_RestoreInterrupts(state);
}
