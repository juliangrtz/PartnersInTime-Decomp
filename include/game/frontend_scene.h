#ifndef PIT_GAME_FRONTEND_SCENE_H
#define PIT_GAME_FRONTEND_SCENE_H
#include <game/task.h>
#include <game/text.h>

typedef struct TitleSceneTask {
    GameTask base;
    u8 unknown_18[16];
    GameIrqTask *irq;
    int phase;
} TitleSceneTask;
typedef struct CreditsSceneTask {
    GameTask base;
    u8 unknown_18[16];
    GameIrqTask *irq;
    int phase;
    const u32 *font;
    GameText text;
    u8 pixels[4096];
    u32 unknown_1064;
} CreditsSceneTask;
typedef struct CreditsMotion CreditsMotion;
struct CreditsMotion {
    void (*update)(CreditsMotion *motion);
    u32 phase, counter;
    u8 variant, column, row, unknown_0f;
    int texture_x, texture_y, scale_x, scale_y;
    int x, y, target_x, target_y, vx, vy, ax, ay;
    s32 unknown_40, unknown_44, unknown_48, unknown_4c;
    u16 frames, unknown_52;
};

typedef char TitleSceneTaskSizeCheck[sizeof(TitleSceneTask) == 48 ? 1 : -1];
typedef char CreditsSceneTaskSizeCheck[sizeof(CreditsSceneTask) == 4200 ? 1 : -1];
typedef char CreditsMotionSizeCheck[sizeof(CreditsMotion) == 84 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
GameIrqTask *TitleSceneIrq_Destroy(GameIrqTask *task);
GameIrqTask *TitleSceneIrq_Delete(GameIrqTask *task);
void TitleScene_LoadResources(TitleSceneTask *unused);
void TitleScene_SelectGreeting(void);
TitleSceneTask *TitleScene_Delete(TitleSceneTask *task);
TitleSceneTask *TitleScene_Destroy(TitleSceneTask *task);
void TitleScene_UploadFrame(void);
GameIrqTask *TitleSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent);
GameIrqTask *CreditsSceneIrq_Destroy(GameIrqTask *task);
GameIrqTask *CreditsSceneIrq_Delete(GameIrqTask *task);
void CreditsMotion_SetAcceleration(CreditsMotion *motion, int frames);
void CreditsMotion_SetEaseOut(CreditsMotion *motion, int frames);
void CreditsMotion_TargetLayoutCenter(CreditsMotion *motion, int layout, int screen);
void CreditsMotion_TargetLayoutCell(CreditsMotion *motion, int layout, int screen);
void CreditsMotion_TargetLayoutOrigin(CreditsMotion *motion, int layout, int screen);
void CreditsGridFade_Update(CreditsMotion *motion);
void CreditsRadialDelay_Update(CreditsMotion *motion);
void CreditsFade_InitAll(void);
void CreditsFade_Update(CreditsMotion *motion);
void CreditsSpiralCollapse_Update(CreditsMotion *motion);
void CreditsColumnCollapse_InitAll(void);
void CreditsColumnCollapse_Update(CreditsMotion *motion);
void CreditsPatternCollapse_InitAll(void);
void CreditsPatternCollapse_Update(CreditsMotion *motion);
void CreditsScatter_InitAll(void);
void CreditsScatter_Update(CreditsMotion *motion);
void CreditsCollapse_InitAll(void);
void CreditsCollapse_Update(CreditsMotion *motion);
void CreditsScene_InitGeometry(CreditsSceneTask *unused);
void CreditsScene_LoadResources(CreditsSceneTask *task);
CreditsSceneTask *CreditsScene_Delete(CreditsSceneTask *task);
CreditsSceneTask *CreditsScene_Destroy(CreditsSceneTask *task);
CreditsSceneTask *CreditsScene_Init(CreditsSceneTask *task, u32 priority, u32 unused);
GameIrqTask *CreditsSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent);
#ifdef __cplusplus
}
#endif
#endif
