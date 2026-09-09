#ifndef PIT_GAME_PAUSE_MENU_LABEL_H
#define PIT_GAME_PAUSE_MENU_LABEL_H
#include <nitro.h>
/* Label rows store integer-pixel offsets relative to the shared menu origin. */
typedef struct PauseMenuLabelTask {
    u8 unknown_00[40];
    int x, y;
    u8 unknown_30[24];
} PauseMenuLabelTask;
typedef char PauseMenuLabelTask_SizeCheck[sizeof(PauseMenuLabelTask) == 72 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void PauseMenuLabel_DrawPage(int page);
void PauseMenuLabel_ReleaseRows(void);
void PauseMenuLabel_DrawSubmenu(void);
void PauseMenuLabel_UpdateTextRow(PauseMenuLabelTask *task);
void PauseMenuLabel_UpdateIconRow(PauseMenuLabelTask *task);
#ifdef __cplusplus
}
#endif
#endif
