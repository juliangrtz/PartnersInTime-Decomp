#include <nitro/fs_overlay.h>

extern int data_02064c20;
extern int data_02064c1c;
extern const char data_02059d98[];
extern int OS_GetLockID(void);
extern void func_0204326c(int lock_id);
extern void CARD_LockRom(u16 lock_id);
extern void CARD_UnlockRom(u16 lock_id);
extern void CARDi_ReadRom(int dma, u32 source, void *destination, u32 size,
    void (*callback)(FsArchive *), FsArchive *argument, int asynchronous);

int FSi_EmptyArchiveProc(FsFile *file, int command);
int FSi_ReadDummyCallback(FsArchive *archive, void *buffer, u32 offset, u32 size);
int FSi_RomArchiveProc(FsFile *file, int command);
int FSi_WriteDummyCallback(FsArchive *archive, void *buffer, u32 offset, u32 size);
int FSi_ReadRomCallback(FsArchive *archive, void *buffer, u32 offset, u32 size);


void FSi_OnRomReadDone(FsArchive *archive) { FS_NotifyArchiveAsyncEnd(archive, 0); }

int FSi_ReadRomCallback(FsArchive *archive, void *buffer, u32 offset, u32 size) {
    CARDi_ReadRom(data_02064c20, offset, buffer, size, FSi_OnRomReadDone, archive, 1);
    return 6;
}
int FSi_WriteDummyCallback(FsArchive *archive, void *buffer, u32 offset, u32 size) { return 1; }

int FSi_RomArchiveProc(FsFile *file, int command) {
    switch (command) {
    case 9: CARD_LockRom((u16)data_02064c1c); return 0;
    case 10: CARD_UnlockRom((u16)data_02064c1c); return 0;
    case 1: return 4;
    default: return 8;
    }
}
int FSi_ReadDummyCallback(FsArchive *archive, void *buffer, u32 offset, u32 size) { return 1; }
int FSi_EmptyArchiveProc(FsFile *file, int command) { return 4; }

void FSi_InitRom(int dma) {
    data_02064c20 = dma;
    data_02064c1c = OS_GetLockID();
    fsi_ovt9.offset = 0;
    fsi_ovt9.size = 0;
    fsi_ovt7.offset = 0;
    fsi_ovt7.size = 0;
    func_0204326c(data_02064c1c);
    FS_InitArchive(&fsi_arc_rom);
    FS_RegisterArchiveName(&fsi_arc_rom, data_02059d98, 3);
    if (*(vu16 *)0x027FFC40 == 2) {
        fsi_ovt9.offset = -1;
        fsi_ovt9.size = 0;
        fsi_ovt7.offset = -1;
        fsi_ovt7.size = 0;
        FS_SetArchiveProc(&fsi_arc_rom, FSi_EmptyArchiveProc, -1);
        FS_LoadArchive(&fsi_arc_rom, 0, 0, 0, 0, 0, FSi_ReadDummyCallback, FSi_WriteDummyCallback);
    } else {
        const FsRomTable *fnt;
        const FsRomTable *fat;
        u32 fnt_offset;
        u32 fat_offset;
        /* The ROM header bases remain live across archive registration. */
        asm {
            ldr fnt, =0x027FFE40
            ldr r0, =fsi_arc_rom
            ldr r1, =FSi_RomArchiveProc
            ldr r2, =0x602
            ldr fat, =0x027FFE48
            bl FS_SetArchiveProc
        }
        fnt_offset = fnt->offset;
        if (fnt_offset == -1 || !fnt_offset) return;
        fat_offset = fat->offset;
        if (fat_offset == -1 || !fat_offset) return;
        FS_LoadArchive(&fsi_arc_rom, 0, fat_offset, fat->size, fnt_offset, fnt->size,
            FSi_ReadRomCallback, FSi_WriteDummyCallback);
    }
}

int FS_SetDefaultDMA(int dma) {
    u32 state = OS_DisableInterrupts();
    int previous = data_02064c20;
    int running = FS_SuspendArchive(&fsi_arc_rom);
    data_02064c20 = dma;
    if (running) FS_ResumeArchive(&fsi_arc_rom);
    OS_RestoreInterrupts(state);
    return previous;
}
u32 FS_TryLoadTable(void *buffer, u32 size) { return FS_LoadArchiveTables(&fsi_arc_rom, buffer, size); }
