#ifndef PIT_GAME_BATTLE_ARCHIVE_H
#define PIT_GAME_BATTLE_ARCHIVE_H

#include <game/battle_context.h>
#include <game/battle_task_queue.h>

u32 BattleArchive_GetEntrySize(void *system, const u8 *archive_cursor, u16 entry_index);
int BattleArchive_ReadAsync(void *system, void *request, int unknown_2, const u8 *archive_cursor,
                            const void *descriptor, u16 unknown_5);
u32 BattleArchive_GetDescriptorSize(void *system, const void *descriptor);
int BattleArchive_Open(void *system, const u8 *source, u32 size, const void *descriptor);

#ifdef __cplusplus
extern "C" {
#endif
void BattleMap_OpenArchiveTask(BattleQueuedTask *task);
BattleQueuedTask *BattleMap_QueueLoad(BattleSceneResource *resource, int index);
int BattleMap_GetEncounterResourceIndex(int encounter_id);
void BattleTextureCatalog_ReadTask(BattleQueuedTask *task);
void BattleTextureCatalog_OpenTask(BattleQueuedTask *task);
void BattleAIArchives_ReadSceneTask(BattleQueuedTask *task);
void BattleMap_ReadSubDisplayTask(BattleQueuedTask *task);
void BattleMap_ConfigureDisplays(void);
void BattleMap_BeginSubDisplayTask(BattleQueuedTask *task);
BattleQueuedTask *BattleMap_QueueSubDisplayLoad(void);
void BattleMap_SavePalette(void);
void BattleMap_ReadCompressedEntry(int entry, u8 **destinations, u32 *sizes);
void BattleMap_ReadEntry(int entry, u8 **destinations, u32 *sizes);
#ifdef __cplusplus
}
#endif

#endif
