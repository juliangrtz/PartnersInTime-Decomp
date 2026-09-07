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
    const u8 *volatile source;
    u8 *volatile destination;
    volatile u32 length;
    int dma;
    CardCallback callback;
    void *argument;
    CardTask task;
    volatile u32 flags;
    volatile u32 priority;
    OsThread thread;
    OsThread *volatile current_thread;
    OsThreadQueue task_queue;
    u16 paddingd6;
    u32 reservedd8[2];
    u8 buffer[256];
};

extern CardWork cardi_common;
/* Preserve a register-based view of the shared work block. */
static inline CardWork *CARDi_GetCommon(void) { return (CardWork *)(u32)&cardi_common; }
extern CardCommand data_02064d60;
extern u8 data_02064da0[];
extern u32 cardi_rom_base;
extern int CARDi_EnableFlag;

typedef struct CardRomStat CardRomStat;
struct CardRomStat {
    void (*read)(CardRomStat *stat);
    u32 control;
    u32 cache_address;
    u32 reserved0c[5];
    u8 buffer[512];
};
extern CardRomStat rom_stat;
typedef char CardRomStatSizeCheck[(sizeof(CardRomStat) == 544) ? 1 : -1];
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
u32 CARD_GetResultCode(void);
void CARD_LockBackup(u16 lock_id);
void CARD_UnlockBackup(u16 lock_id);
int CARD_WaitBackupAsync(void);
int CARD_ReadBackup(u32 source, void *destination, u32 length, CardCallback callback, void *argument, int asynchronous);
int CARD_ProgramAndVerifyBackup(u32 destination, const void *source, u32 length, CardCallback callback, void *argument, int asynchronous);
int CARD_WriteAndVerifyBackup(u32 destination, const void *source, u32 length, CardCallback callback, void *argument, int asynchronous);
void CARDi_EndBackupTask(CardWork *work);
void CARDi_WriteAndVerifyBackup(CardWork *work);
void CARDi_ProgramAndVerifyBackup(CardWork *work);
void CARDi_ReadBackup(CardWork *work);
int CARDi_Request(CardWork *work, u32 command, int retries);
void CARD_Init(void);
void CARDi_ReadRom(u32 dma, u32 source, void *destination, u32 size,
    CardCallback callback, void *argument, int asynchronous);
int CARD_WaitRomAsync(void);
void CARDi_ReadRomSyncCore(CardWork *work);
void CARDi_ReadCard(CardRomStat *stat);
int CARDi_TryReadCardDma(CardRomStat *stat);
void CARDi_OnReadCard(void);
void CARDi_SetCardDma(void);
void CARDi_SetRomOp(u32 high, u32 low);
void CARDi_EndTask(void);
int CARDi_ReadFromCache(CardRomStat *stat);
void CARDi_InitPulledOutCallback(void);

extern int PXI_IsCallbackReady(u32 tag, int processor);
extern int PXI_SendWordByFifo(u32 tag, u32 data, int error);
extern void func_0203ae6c(u32 cycles);
extern void MI_StopDma(u32 dma);
extern void MIi_CardDmaCopy32(u32 dma, const void *source, void *destination, u32 size);
extern u32 OS_GetDTCMAddress(void);
extern void DC_StoreRange(const void *address, u32 size);
extern void IC_InvalidateRange(const void *address, u32 size);
extern void OS_SetIrqFunction(u32 mask, void (*callback)(void));
extern u32 OS_ResetRequestIrqMask(u32 mask);
extern u32 OS_EnableIrqMask(u32 mask);
extern u32 OS_DisableIrqMask(u32 mask);

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
extern void PXI_SetFifoRecvCallback(u32 tag, void (*callback)(u32, u32, int));

#endif
