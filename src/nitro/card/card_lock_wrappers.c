#include <nitro/card.h>

extern void func_02038f10(u16 lock_id);
extern void func_02038f2c(u16 lock_id);

int CARD_IsAvailable(void) { return cardi_common.flags != 0; }

u32 CARD_GetResultCode(void) { return cardi_common.command->result; }

void CARD_LockRom(u16 lock_id) { CARDi_LockResource(lock_id, 1); func_02038f2c(lock_id); }

void CARD_UnlockRom(u16 lock_id) { func_02038f10(lock_id); CARDi_UnlockResource(lock_id, 1); }

void CARD_LockBackup(u16 lock_id) { CARDi_LockResource(lock_id, 2); }

void CARD_UnlockBackup(u16 lock_id) { CARDi_UnlockResource(lock_id, 2); }
