#ifndef PIT_GAME_BATTLE_ENTRY_H
#define PIT_GAME_BATTLE_ENTRY_H
#include <game/battle_task_queue.h>
#include <nitro/rtc.h>

/* Ten-byte encounter request at save-context offset 0x558. The signed approach
 * nibble selects the entry animation; bit 8 selects the Luigi-side initiator. */
typedef struct BattleEntrySaveView {
    s16 formation;
    u8 unknown_02[4];
    s16 exit_mode;
    union {
        s16 raw;
        struct {
            u16 unknown_00_02 : 3;
            u16 sub_screen : 1;
            u16 unknown_04_15 : 12;
        };
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
void BattleEntry_SeedRandomFromClock(u32 result, void *argument);
void BattleEntry_ShowBattleDisplay(void);
void BattleEntry_InitializeGraphics(void);
void BattleEntry_ConfigureVideoBanks(void);
void BattleEntry_WaitCapture(void);
void BattleEntry_CaptureScene(void);
void BattleEntry_StartTransitionSound(void);
void BattleEntry_RestoreFieldDisplay(void);
void BattleEntry_CopySubDisplayToMain(void);
void BattleEntry_MapCaptureBanks(void);
void BattleEntry_FadeField(void);
void BattleEntry_ApplyFieldBrightness(void);
void BattleMain_Initialize(void);
int BattlePalette_Animate(const s16 *commands, int colors, const u16 *buffer, u16 **destination);
void BattleEntry_InitializeScene(void);
void BattleEntry_LoadResources(void);
void BattleEntry_WaitArchives(void);
void BattleEntry_LoadArchives(BattleQueuedTask *task);
void BattleEntry_InitializeHeaps(BattleQueuedTask *task);
#ifdef __cplusplus
}
#endif
#endif
