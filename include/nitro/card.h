#ifndef NITRO_CARD_H
#define NITRO_CARD_H

#include <nitro/os_sync.h>

typedef void (*CardCallback)(void *argument);
typedef struct CardCommand {
    u32 result;
    u32 backup_type;
    u32 rom_id;
    u32 source;
    u32 destination;
    u32 length;
    u32 total_size;
    u32 sector_size;
    u32 page_size;
    u32 address_width;
    u32 write_delay;
    u32 erase_delay;
    u32 erase_timeout;
    u32 total_erase_delay;
    u32 total_erase_timeout;
    u32 program_timeout;
} CardCommand;

typedef struct CardWork CardWork;
typedef void (*CardTask)(CardWork *work);
struct CardWork {
    CardCommand *command;
    u32 reserved04;
    volatile s32 lock_owner;
    volatile s32 lock_count;
    OsThreadQueue lock_queue;
    u16 padding12;
    u32 resource;
    const u8 *source;
    u8 *destination;
    u32 length;
    int dma;
    CardCallback callback;
    void *argument;
    CardTask task;
    u32 flags;
    volatile u32 priority;
    OsThread thread;
    OsThread *volatile current_thread;
    OsThreadQueue task_queue;
    u16 paddingd6;
    u32 reservedd8[2];
    u8 buffer[256];
};

extern CardWork cardi_common;
extern CardCommand data_02064d60;
extern u8 data_02064da0[];
extern u32 cardi_rom_base;
extern int CARDi_EnableFlag;

extern void *rom_stat;
typedef char CardCommandSizeCheck[(sizeof(CardCommand) == 64) ? 1 : -1];
typedef char CardWorkSizeCheck[(sizeof(CardWork) == 0x1E0) ? 1 : -1];

void CARDi_LockResource(s32 lock_id, u32 resource);
void CARDi_UnlockResource(s32 lock_id, u32 resource);
void CARDi_SetTask(CardTask task);
void CARDi_InitCommon(void);
int CARDi_WaitAsync(void);
int CARD_IsAvailable(void);
void CARD_Enable(int enabled);
int CARD_IsEnabled(void);
void CARD_CheckEnabled(void);
void CARDi_TaskThread(void *argument);
void CARDi_FifoCallback(u32 tag, u32 data, int error);
void CARDi_SetBackupSpec(int type);
int CARD_IdentifyBackup(int type);
void CARDi_EndBackupTask(CardWork *work);
void CARDi_WriteAndVerifyBackup(CardWork *work);
void CARDi_ProgramAndVerifyBackup(CardWork *work);
void CARDi_ReadBackup(CardWork *work);
int CARDi_Request(CardWork *work, u32 command, int retries);
void CARD_Init(void);

extern void MIi_CpuClearFast(u32 value, void *destination, u32 size);
extern void MI_CpuCopy8(const void *source, void *destination, u32 size);
extern void MI_CpuFill8(void *destination, u8 value, u32 size);
extern void DC_FlushRange(const void *address, u32 size);
extern void DC_InvalidateRange(const void *address, u32 size);
extern void DC_WaitWriteBufferEmpty(void);
extern void OS_Terminate(void);
extern u32 OS_GetProtectionRegion2(void);
extern void OS_SetProtectionRegion2(u32 region);
extern void OS_WakeupThreadDirect(OsThread *thread);
extern void OS_SetThreadPriority(OsThread *thread, u32 priority);
extern u32 func_0203929c(OsThread *thread);
extern void func_020395d0(OsThread *thread, void (*entry)(void *), void *argument,
    void *stack, u32 stack_size, u32 priority);
extern void func_0203d5b8(u32 tag, void (*callback)(u32, u32, int));

#endif
