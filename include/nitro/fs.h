#ifndef NITRO_FS_H
#define NITRO_FS_H

#include <nitro/os_sync.h>

typedef struct FsFile FsFile;
typedef struct FsArchive FsArchive;
typedef struct FsLink { struct FsLink *previous; struct FsLink *next; } FsLink;
typedef struct FsFileId { FsArchive *archive; u32 id; } FsFileId;
typedef struct FsDirectoryPosition {
    FsArchive *archive;
    u16 directory;
    u16 index;
    u32 position;
} FsDirectoryPosition;
typedef struct FsReadCursor { FsArchive *archive; u32 position; } FsReadCursor;
typedef struct FsDirectoryEntry {
    union { FsDirectoryPosition directory; FsFileId file; } entry;
    int is_directory;
    u32 name_length;
    char name[128];
} FsDirectoryEntry;
typedef int (*FsArchiveIo)(FsArchive *archive, void *buffer, u32 offset, u32 size);
typedef int (*FsArchiveProc)(FsFile *file, int command);

struct FsArchive {
    u32 name;
    FsArchive *next;
    FsArchive *previous;
    OsThreadQueue sync_queue;
    OsThreadQueue suspend_queue;
    volatile u32 flags;
    FsLink commands;
    u32 base;
    u32 fat;
    u32 fat_size;
    u32 fnt;
    u32 fnt_size;
    u32 original_fat;
    u32 original_fnt;
    void *table_buffer;
    FsArchiveIo read;
    FsArchiveIo write;
    FsArchiveIo table_read;
    FsArchiveProc procedure;
    u32 procedure_mask;
};

struct FsFile {
    FsLink link;
    FsArchive *archive;
    volatile u32 flags;
    int command;
    volatile int result;
    OsThreadQueue queue;
    u16 padding1a;
    union {
        struct { u32 id; s32 top; s32 bottom; s32 position; } file;
        struct { FsDirectoryPosition position; u32 parent; } directory;
    } property;
    union {
        u32 words[6];
        FsFileId file_id;
        FsDirectoryPosition directory;
        struct { void *buffer; u32 size; u32 length; } io;
        struct { u32 top; u32 bottom; u32 id; } direct;
        struct { FsDirectoryPosition directory; const char *path; int find_directory; void *output; } path;
    } argument;
};

typedef char FsArchiveSizeCheck[(sizeof(FsArchive) == 80) ? 1 : -1];
typedef char FsFileSizeCheck[(sizeof(FsFile) == 68) ? 1 : -1];
typedef char FsDirectoryPositionSizeCheck[(sizeof(FsDirectoryPosition) == 12) ? 1 : -1];
typedef char FsDirectoryEntrySizeCheck[(sizeof(FsDirectoryEntry) == 148) ? 1 : -1];

static inline int FS_IsBusy(const FsFile *file) {
    if (file->flags & 1) return 1;
    return 0;
}
static inline int FSi_TestFileFlag(const FsFile *file, u32 mask) {
    if (file->flags & mask) return 1;
    return 0;
}
static inline int FSi_TestArchiveFlag(const FsArchive *archive, u32 mask) {
    if (archive->flags & mask) return 1;
    return 0;
}


void FS_InitFile(FsFile *file);
int FS_OpenFileDirect(FsFile *file, FsArchive *archive, u32 top, u32 bottom, u32 id);
int FS_OpenFileFast(FsFile *file, FsFileId id);
int FS_ReadFile(FsFile *file, void *buffer, int size);
int FS_SeekFile(FsFile *file, int offset, int origin);
int FS_ReadFileAsync(FsFile *file, void *buffer, int size);
int FS_CloseFile(FsFile *file);
FsArchive *FS_FindArchive(const char *name, int length);
u32 FSi_GetPackedName(const char *name, int length);
int FSi_ReadMemoryCore(FsArchive *archive, void *buffer, u32 offset, u32 size);
int FSi_ReadMemCallback(FsArchive *archive, void *buffer, u32 offset, u32 size);
int FSi_WriteMemCallback(FsArchive *archive, void *buffer, u32 offset, u32 size);
void FSi_ReleaseCommand(FsFile *file, int result);
FsFile *FSi_NextCommand(FsArchive *archive);
void FSi_ExecuteAsyncCommand(FsFile *file);
int FSi_ExecuteSyncCommand(FsFile *file);
int FSi_SendCommand(FsFile *file, int command);
void FSi_AppendToList(FsLink *item, FsLink *list);
void FSi_CutFromList(FsLink *item);
int func_0203e118(FsFile *file, int command);
int FS_WaitAsync(FsFile *file);
int FS_ConvertPathToFileID(FsFileId *id, const char *path);
int FSi_ReadFileCore(FsFile *file, void *buffer, int size, int asynchronous);
int FSi_FindPath(FsFile *file, const char *path, FsFileId *id, FsDirectoryPosition *directory);
void func_0203e068(FsReadCursor *cursor, void *buffer, u32 size);
void FS_InitArchive(FsArchive *archive);
int FS_RegisterArchiveName(FsArchive *archive, const char *name, int length);
void FS_SetArchiveProc(FsArchive *archive, FsArchiveProc procedure, u32 mask);
int FS_SuspendArchive(FsArchive *archive);
int FS_ResumeArchive(FsArchive *archive);
u32 FS_LoadArchiveTables(FsArchive *archive, void *buffer, u32 size);
int FS_LoadArchive(FsArchive *archive, u32 base, u32 fat, u32 fat_size,
                  u32 fnt, u32 fnt_size, FsArchiveIo read, FsArchiveIo write);
void FS_NotifyArchiveAsyncEnd(FsArchive *archive, int result);

extern void MI_CpuCopy8(const void *source, void *destination, u32 size);
extern void MI_CpuFill8(void *destination, u8 value, u32 size);

#endif
