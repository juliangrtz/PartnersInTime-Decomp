#ifndef PIT_GAME_BATTLE_ENTRY_H
#define PIT_GAME_BATTLE_ENTRY_H
#include <game/battle_task_queue.h>

/* Ten-byte encounter request at save-context offset 0x558. The signed approach
 * nibble selects the entry animation; bit 8 selects the Luigi-side initiator. */
typedef struct BattleEntrySaveView {
    s16 formation;
    u8 unknown_02[6];
    union {
        s16 raw;
        struct {
            s16 unknown_low : 4;
            s16 approach : 4;
            u16 luigi : 1;
            u16 unknown_high : 7;
        };
    } flags;
} BattleEntrySaveView;

/* Names follow the allocation labels retained in the original executable. */
typedef struct BattleEntryHeaps {
    int main_data, sub_data, scene_data, attack_data;
    int texture_animation, parameter_animation;
} BattleEntryHeaps;

typedef char BattleEntrySaveViewSizeCheck[sizeof(BattleEntrySaveView) == 10 ? 1 : -1];
typedef char BattleEntryHeapsSizeCheck[sizeof(BattleEntryHeaps) == 24 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void BattleEntry_InitializeScene(void);
void BattleEntry_LoadResources(void);
void BattleEntry_WaitArchives(void);
void BattleEntry_LoadArchives(BattleQueuedTask *task);
void BattleEntry_InitializeHeaps(BattleQueuedTask *task);
#ifdef __cplusplus
}
#endif
#endif
