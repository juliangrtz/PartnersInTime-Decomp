#ifndef PIT_GAME_TITLE_STARTUP_H
#define PIT_GAME_TITLE_STARTUP_H
#include <game/task.h>

typedef struct MenuElement MenuElement;
#ifdef __cplusplus
extern "C" {
#endif
int TitleAnimation_GetParticipantMask(void);
void *TitleAnimation_Create(void);
int TitleElement_IsInactive(const void *element);
GameTask *TitleAnimationFrameTask_Delete(GameTask *task);
GameTask *TitleAnimationFrameTask_Destroy(GameTask *task);
void TitleAnimationFrameTask_WaitVBlank(GameTask *task);
void TitleStartup_UpdateGreeting(MenuElement *work);
#ifdef __cplusplus
}
#endif
#endif
