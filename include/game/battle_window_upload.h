#ifndef PIT_GAME_BATTLE_WINDOW_UPLOAD_H
#define PIT_GAME_BATTLE_WINDOW_UPLOAD_H
#include <game/battle_window.h>

typedef union BattleWindowUploadFlags {
    u32 raw;
    struct {
        u32 pending : 1;
        u32 unknown_01_31 : 31;
    } bits;
} BattleWindowUploadFlags;
typedef union BattleWindowLinkFlags {
    u32 raw;
    struct {
        u32 unknown_00_29 : 30;
        u32 flag_30 : 1;
        u32 unknown_31 : 1;
    } bits;
} BattleWindowLinkFlags;
typedef char BattleWindowUploadFlagsSizeCheck[sizeof(BattleWindowUploadFlags) == 4 ? 1 : -1];
typedef char BattleWindowLinkFlagsSizeCheck[sizeof(BattleWindowLinkFlags) == 4 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
int BattleWindow_IsUploadReady(BattleWindowManager *manager, s16 index);
void BattleWindow_SetLinkFlag30(GameWindow *window, u8 enabled);
void BattleWindow_UploadSubWindow(BattleWindowManager *manager, const GameWindow *window);
void BattleWindow_UploadSubTilemap(BattleWindowManager *manager);
void BattleWindow_ScheduleSubUploads(BattleWindowManager *manager);
#ifdef __cplusplus
}
#endif
#endif
