#ifndef PIT_GAME_SAVE_STORAGE_H
#define PIT_GAME_SAVE_STORAGE_H

#include <game/save_backup.h>

typedef struct SaveStorageSlot {
    u8 state[1176];
    u8 extra[848];
} SaveStorageSlot;

typedef struct SaveStorageSignature { u8 magic[6]; u8 version; u8 reserved; } SaveStorageSignature;

enum SaveStorageFooter { SAVE_FOOTER_MAIN, SAVE_FOOTER_EXTRA, SAVE_FOOTER_ALL };

int SaveStorage_Load(void);
int SaveStorage_Initialize(void);
int SaveStorage_Probe(void);
int SaveStorage_Erase(void);
int SaveStorage_CheckSignature(void);
int SaveStorage_WriteSignature(void);
int SaveStorage_WriteSettings(void);
int SaveStorage_CheckSettings(void);
void SaveStorage_ClearDirectory(void);
int SaveStorage_CheckDirectory(void);
int SaveStorage_WriteDirectory(void);
SaveStorageSlot *SaveStorage_GetSlot(u8 slot);
void *SaveStorage_GetSlotExtra(u8 slot);
int SaveStorage_WriteSlot(u8 source_slot, u8 destination_slot, int checksum);
u8 SaveStorage_GetState(void);
int SaveStorage_CheckSlot(u8 slot);
int SaveStorage_WriteFooter(enum SaveStorageFooter region);

#endif
