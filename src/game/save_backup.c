#include <game/save_backup.h>

extern u8 data_0205e300;
extern SaveBackupWrite data_0205e304;
extern u32 data_0205e308;
extern u32 data_0205e30c;
extern u32 data_0205e310;
extern u32 data_0205e314;
extern SaveBackupRead data_0205e318;
extern int data_0205e31c;
extern int data_0205e320;
extern SaveBackupChecksum data_0205e324;
extern int OS_GetLockID(void);
extern void OS_ReleaseLockID(u16 lock_id);

int SaveBackup_WriteEeprom(u32 offset, const void *source, u32 size, CardCallback callback, void *argument)
{
    return CARD_ProgramAndVerifyBackup(offset, source, size, callback, argument, 1);
}

int SaveBackup_ReadEeprom(u32 offset, void *destination, u32 size, CardCallback callback, void *argument)
{
    return CARD_ReadBackup(offset, destination, size, callback, argument, 1);
}

int SaveBackup_WriteFlash(u32 offset, const void *source, u32 size, CardCallback callback, void *argument)
{
    return CARD_WriteAndVerifyBackup(offset, source, size, callback, argument, 1);
}

int SaveBackup_ReadFlash(u32 offset, void *destination, u32 size, CardCallback callback, void *argument)
{
    return CARD_ReadBackup(offset, destination, size, callback, argument, 1);
}

void SaveBackup_Init(int type)
{
    if (data_0205e320) return;
    if (!CARD_IsAvailable()) CARD_Init();
    data_0205e31c = -3;
    SaveBackup_Lock();
    switch (type) {
    case 0:
        CARD_IdentifyBackup(0x901);
        data_0205e304 = SaveBackup_WriteEeprom;
        data_0205e318 = SaveBackup_ReadEeprom;
        break;
    case 1:
        CARD_IdentifyBackup(0xd01);
        data_0205e304 = SaveBackup_WriteEeprom;
        data_0205e318 = SaveBackup_ReadEeprom;
        break;
    case 2:
        CARD_IdentifyBackup(0x1202);
        data_0205e304 = SaveBackup_WriteFlash;
        data_0205e318 = SaveBackup_ReadFlash;
        break;
    }
    SaveBackup_Unlock();
    data_0205e324 = SaveBackup_DefaultChecksum;
    data_0205e300 = 0;
    data_0205e308 = 0;
    data_0205e320 = 1;
}

int SaveBackup_Lock(void)
{
    if (data_0205e31c != -3) SaveBackup_Unlock();
    data_0205e31c = OS_GetLockID();
    if (data_0205e31c == -3) return 0;
    CARD_LockBackup((u16)data_0205e31c);
    return 1;
}

void SaveBackup_Unlock(void)
{
    if (data_0205e31c != -3) {
        CARD_UnlockBackup((u16)data_0205e31c);
        OS_ReleaseLockID((u16)data_0205e31c);
        data_0205e31c = -3;
    }
}

int SaveBackup_WriteAsync(u32 offset, void *source, u32 size, int checksum)
{
    if (!SaveBackup_Lock()) {
        data_0205e300 = 0;
        return 0;
    }
    data_0205e314 = offset;
    data_0205e310 = (u32)source;
    data_0205e30c = size;
    if (checksum) {
        u16 value = data_0205e324(source, size - 2);
        *(u16 *)(data_0205e310 + data_0205e30c - 2) = value;
    }
    data_0205e304(data_0205e314, (const void *)data_0205e310, data_0205e30c, SaveBackup_WriteComplete, 0);
    data_0205e300 = 1;
    return 1;
}

int SaveBackup_ReadAsync(u32 offset, void *destination, u32 size)
{
    if (!SaveBackup_Lock()) {
        data_0205e300 = 0;
        return 0;
    }
    data_0205e310 = offset;
    data_0205e314 = (u32)destination;
    data_0205e30c = size;
    data_0205e318(offset, (void *)data_0205e314, size, SaveBackup_ReadComplete, 0);
    data_0205e300 = 1;
    return 1;
}

void SaveBackup_WriteComplete(void *argument)
{
    data_0205e308 = CARD_GetResultCode();
    SaveBackup_Unlock();
    if (data_0205e308) data_0205e300 = 2;
    else data_0205e300 = 0;
}

void SaveBackup_ReadComplete(void *argument)
{
    data_0205e308 = CARD_GetResultCode();
    SaveBackup_Unlock();
    if (data_0205e308) data_0205e300 = 2;
    else data_0205e300 = 0;
}

int SaveBackup_Wait(void)
{
    if (CARD_WaitBackupAsync()) return data_0205e300 != 2;
    return 0;
}

u8 SaveBackup_GetState(void)
{
    return data_0205e300;
}

void SaveBackup_SetChecksum(SaveBackupChecksum checksum)
{
    if (!checksum) data_0205e324 = SaveBackup_DefaultChecksum;
    else data_0205e324 = checksum;
}

int SaveBackup_CheckChecksum(const void *data, u32 size)
{
    int checksum = data_0205e324(data, size - 2);
    return checksum == *(const u16 *)((const u8 *)data + size - 2);
}

int SaveBackup_DefaultChecksum(const void *data, u32 size)
{
    const u8 *cursor = data;
    u16 checksum = 0x123;
    u32 i;
    for (i = 0; i < size; ++i, ++cursor) checksum += *cursor;
    return checksum;
}
