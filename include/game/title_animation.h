#ifndef PIT_GAME_TITLE_ANIMATION_H
#define PIT_GAME_TITLE_ANIMATION_H
#include <game/task.h>

typedef struct TitleAnimationController TitleAnimationController;
#ifdef __cplusplus
extern "C" {
#endif
void TitleAnimation_ReleaseArchiveTables(TitleAnimationController *work);
void TitleAnimation_ReleaseLocalizedResources(TitleAnimationController *work);
void TitleAnimation_LoadLocalizedResources(TitleAnimationController *work);
void TitleAnimation_LoadArchiveTables(TitleAnimationController *work);
TitleAnimationController *TitleAnimation_Delete(TitleAnimationController *work);
TitleAnimationController *TitleAnimation_Destroy(TitleAnimationController *work);
TitleAnimationController *TitleAnimation_Init(TitleAnimationController *work, u32 priority,
                                              u32 unused, void *argument);
#ifdef __cplusplus
}
#endif
#endif
