#ifndef PIT_GAME_BATTLE_ARCHIVE_H
#define PIT_GAME_BATTLE_ARCHIVE_H

#include <game/battle_context.h>

u32 BattleArchive_GetEntrySize(void *system, const u8 *archive_cursor,
                               u16 entry_index);
void BattleArchive_ReadAsync(void *system, void *request,
                             int unknown_2, const u8 *archive_cursor,
                             const void *descriptor, int unknown_5);
u32 BattleArchive_GetDescriptorSize(void *system, const void *descriptor);
void BattleArchive_Open(void *system, const u8 *source, u32 size,
                        const void *descriptor);

#endif
