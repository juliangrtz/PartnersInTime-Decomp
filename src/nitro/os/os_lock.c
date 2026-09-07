#include <nitro/os_lock.h>

extern int data_02062d00;
extern void func_0203ae6c(u32 cycles);
extern u32 func_0203ae28(void);
extern u32 func_0203ae3c(u32 state);
extern u32 MI_SwapWord(u32 value, volatile u32 *address);
extern void func_0203b7a0(u32 value, void *destination, u32 size);

#define CARD_LOCK ((OsLockWord *)0x027fffe0)
#define CARTRIDGE_LOCK ((OsLockWord *)0x027fffe8)
#define INITIALIZATION_LOCK ((OsLockWord *)0x027ffff0)
#define EXMEMCNT (*(vu16 *)0x04000204)

static inline u16 ReadLockExtension(const OsLockWord *lock) { return lock->extension; }

void OS_InitLock(void)
{
    if (data_02062d00) return;
    data_02062d00 = 1;
    INITIALIZATION_LOCK->flag = 0;
    OS_LockWord(126, INITIALIZATION_LOCK, 0);
    while (ReadLockExtension(INITIALIZATION_LOCK)) OSi_SpinWaitLock();
    *(vu32 *)0x027fffb0 = 0xffffffff;
    *(vu32 *)0x027fffb4 = -0x10000;
    {
        volatile u32 zero = 0;
        func_0203b7a0(zero, (void *)0x027fffc0, 40);
    }
    EXMEMCNT |= 0x800;
    EXMEMCNT |= 0x80;
    OS_UnlockWord(126, INITIALIZATION_LOCK, 0);
    OS_LockWord(127, INITIALIZATION_LOCK, 0);
}

int OSi_LockWord(u16 id, OsLockWord *lock, OsLockCallback callback, int disable_fiq)
{
    int result;
    for (result = OSi_TryLockWord(id, lock, callback, disable_fiq);
         result > 0; result = OSi_TryLockWord(id, lock, callback, disable_fiq))
        OSi_SpinWaitLock();
    return result;
}

int OS_LockWord(u16 id, OsLockWord *lock, OsLockCallback callback)
{
    return OSi_LockWord(id, lock, callback, 0);
}

int OSi_UnlockWord(u16 id, OsLockWord *lock, OsLockCallback callback, int disable_fiq)
{
    u32 state;
    if (id != lock->owner) return -2;
    if (disable_fiq) state = func_0203ae28();
    else state = OS_DisableInterrupts();
    lock->owner = 0;
    if (callback) callback();
    lock->flag = 0;
    if (disable_fiq) func_0203ae3c(state);
    else OS_RestoreInterrupts(state);
    return 0;
}

int OS_UnlockWord(u16 id, OsLockWord *lock, OsLockCallback callback)
{
    return OSi_UnlockWord(id, lock, callback, 0);
}

int OSi_TryLockWord(u16 id, OsLockWord *lock, OsLockCallback callback, int disable_fiq)
{
    u32 state;
    int result;
    if (disable_fiq) state = func_0203ae28();
    else state = OS_DisableInterrupts();
    result = MI_SwapWord(id, &lock->flag);
    if (!result) {
        if (callback) callback();
        lock->owner = id;
    }
    if (disable_fiq) func_0203ae3c(state);
    else OS_RestoreInterrupts(state);
    return result;
}

int OS_LockCartridge(u16 id)
{
    return OSi_LockWord(id, CARTRIDGE_LOCK, OSi_AllocateCartridgeBus, 1);
}

int OS_UnlockCartridge(u16 id)
{
    return OSi_UnlockWord(id, CARTRIDGE_LOCK, OSi_ReleaseCartridgeBus, 1);
}

int OS_TryLockCartridge(u16 id)
{
    return OSi_TryLockWord(id, CARTRIDGE_LOCK, OSi_AllocateCartridgeBus, 1);
}

void OSi_AllocateCartridgeBus(void) { EXMEMCNT &= ~0x80; }
void OSi_ReleaseCartridgeBus(void) { EXMEMCNT |= 0x80; }

int OS_LockCard(u16 id)
{
    return OS_LockWord(id, CARD_LOCK, OSi_AllocateCardBus);
}

int OS_UnlockCard(u16 id)
{
    return OS_UnlockWord(id, CARD_LOCK, OSi_ReleaseCardBus);
}

void OSi_AllocateCardBus(void) { EXMEMCNT &= ~0x800; }
void OSi_ReleaseCardBus(void) { EXMEMCNT |= 0x800; }

u16 OS_ReadOwnerOfLockWord(const OsLockWord *lock) { return lock->owner; }

void OSi_SpinWaitLock(void) { func_0203ae6c(4096); }
