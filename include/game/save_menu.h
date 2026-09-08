#ifndef PIT_GAME_SAVE_MENU_H
#define PIT_GAME_SAVE_MENU_H
#include <game/task.h>
#include <game/save_data.h>
#include <game/text.h>

typedef struct SaveMenuPartyMember {
    u16 member_id;
    u16 base_max_hp, base_power, base_defense, base_speed, base_stache;
    u16 max_hp, current_hp, power, defense, speed, stache;
    SavePartyExperience experience;
    u8 unknown_1c[3];
    u8 equipment, badge;
    u8 reserved[3];
} SaveMenuPartyMember;
/* Display summaries preserve only the party and metadata fields used by menus. */
typedef struct SaveMenuSummary {
    SaveMenuPartyMember party[4];
    u8 present[4];
    u32 coins, play_time;
    u8 unknown_9c, unknown_9d, location, padding;
} SaveMenuSummary;
typedef struct SaveMenuText {
    GameText text;
    u32 pixels[6144];
    u32 *owned_fonts[2];
    const u32 *fonts[20];
    u8 *strings[3];
} SaveMenuText;
typedef struct SaveMenuTransferTask {
    u8 unknown_00[32];
    int state, counter;
    int arguments[8];
} SaveMenuTransferTask;
typedef struct SaveSceneTask {
    GameTask base;
    u8 unknown_18[16];
    GameIrqTask *irq;
    int phase;
} SaveSceneTask;
typedef struct SaveMenuMotion {
    u8 unknown_00[36];
    int remaining;
    int unknown_28;
    int x, y, velocity_x, velocity_y, acceleration_x, acceleration_y;
} SaveMenuMotion;

typedef char SaveMenuPartyMemberSizeCheck[sizeof(SaveMenuPartyMember) == 36 ? 1 : -1];
typedef char SaveMenuSummarySizeCheck[sizeof(SaveMenuSummary) == 160 ? 1 : -1];
typedef char SaveMenuTextSizeCheck[sizeof(SaveMenuText) == 24724 ? 1 : -1];
typedef char SaveSceneTaskSizeCheck[sizeof(SaveSceneTask) == 48 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void SaveMenu_BuildStoredSummary(int slot, SaveMenuSummary *summary);
void SaveMenu_BuildLiveSummary(SaveMenuSummary *summary);
int SaveMenuText_DrawTextureRows(SaveMenuText *context, int *offset, int table, int entry, int width,
                                 int row);
void SaveMenuText_DrawBackground(SaveMenuText *context, int engine, int background, int table, u16 entry,
                                 int x, int y);
void SaveMenuText_QueueTextureRow(int offset, int width, int row);
void SaveMenuText_QueueBackground(int engine, int background);
void SaveMenuText_QueueObjectStrip(int engine, int offset, int row, int column, int width);
u16 SaveMenuText_MeasureWidth(SaveMenuText *context, int table, int entry);
const u8 *SaveMenuText_GetString(SaveMenuText *context, int table, int entry);
SaveMenuText *SaveMenuText_Init(SaveMenuText *context);
int SaveMenuMotion_Update(SaveMenuMotion *motion);
SaveSceneTask *SaveScene_Init(SaveSceneTask *task, u32 priority, u32 unused);
SaveSceneTask *LoadScene_Init(SaveSceneTask *task, u32 priority, u32 unused);
SaveSceneTask *GameOverScene_Init(SaveSceneTask *task, u32 priority, u32 unused);
void SaveScene_InitGeometry(SaveSceneTask *unused);
void LoadScene_InitGeometry(SaveSceneTask *unused);
void GameOverScene_InitGeometry(SaveSceneTask *unused);
SaveSceneTask *SaveScene_Delete(SaveSceneTask *task);
SaveSceneTask *SaveScene_Destroy(SaveSceneTask *task);
SaveSceneTask *LoadScene_Delete(SaveSceneTask *task);
SaveSceneTask *LoadScene_Destroy(SaveSceneTask *task);
SaveSceneTask *GameOverScene_Delete(SaveSceneTask *task);
SaveSceneTask *GameOverScene_Destroy(SaveSceneTask *task);
GameIrqTask *SaveSceneIrq_Destroy(GameIrqTask *task);
GameIrqTask *SaveSceneIrq_Delete(GameIrqTask *task);
GameIrqTask *SaveSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent);
GameIrqTask *LoadSceneIrq_Destroy(GameIrqTask *task);
GameIrqTask *LoadSceneIrq_Delete(GameIrqTask *task);
GameIrqTask *LoadSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent);
GameIrqTask *GameOverSceneIrq_Destroy(GameIrqTask *task);
GameIrqTask *GameOverSceneIrq_Delete(GameIrqTask *task);
GameIrqTask *GameOverSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent);
void SaveScene_UploadFrame(void);
void GameOverScene_UploadFrame(void);
#ifdef __cplusplus
}
#endif
#endif
