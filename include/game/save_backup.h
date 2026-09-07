#ifndef PIT_GAME_SAVE_BACKUP_H
#define PIT_GAME_SAVE_BACKUP_H

#include <nitro/card.h>

typedef int (*SaveBackupChecksum)(const void *data, u32 size);
typedef int (*SaveBackupRead)(u32 offset, void *destination, u32 size, CardCallback callback, void *argument);
typedef int (*SaveBackupWrite)(u32 offset, const void *source, u32 size, CardCallback callback, void *argument);

int SaveBackup_DefaultChecksum(const void *data, u32 size);
int SaveBackup_CheckChecksum(const void *data, u32 size);
void SaveBackup_SetChecksum(SaveBackupChecksum checksum);
u8 SaveBackup_GetState(void);
int SaveBackup_Wait(void);
void SaveBackup_ReadComplete(void *argument);
void SaveBackup_WriteComplete(void *argument);
int SaveBackup_ReadAsync(u32 offset, void *destination, u32 size);
int SaveBackup_WriteAsync(u32 offset, void *source, u32 size, int checksum);
void SaveBackup_Unlock(void);
int SaveBackup_Lock(void);
void SaveBackup_Init(int type);
int SaveBackup_ReadFlash(u32 offset, void *destination, u32 size, CardCallback callback, void *argument);
int SaveBackup_WriteFlash(u32 offset, const void *source, u32 size, CardCallback callback, void *argument);
int SaveBackup_ReadEeprom(u32 offset, void *destination, u32 size, CardCallback callback, void *argument);
int SaveBackup_WriteEeprom(u32 offset, const void *source, u32 size, CardCallback callback, void *argument);

#endif
