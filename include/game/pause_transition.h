#ifndef PIT_GAME_PAUSE_TRANSITION_H
#define PIT_GAME_PAUSE_TRANSITION_H

/*
 * The transition that opens and closes the pause menu over the field.
 */
#include <nitro.h>

typedef struct PauseTransitionProgressTask PauseTransitionProgressTask;
typedef struct PauseShutterPanelTask PauseShutterPanelTask;
typedef struct PauseTransitionControllerTask PauseTransitionControllerTask;
typedef struct PauseFadeInTask PauseFadeInTask;

#ifdef __cplusplus
extern "C" {
#endif
void PauseTransition_ExitPanelTask(PauseShutterPanelTask *task);
void PauseTransition_ExitProgressTask(PauseTransitionProgressTask *task);
void PauseTransition_WindowTask(PauseTransitionProgressTask *task);
void PauseTransition_EntryPanelTask(PauseShutterPanelTask *task);
void PauseTransition_EntryProgressTask(PauseTransitionControllerTask *task);
void PauseScene_FadeInTask(PauseFadeInTask *task);
void PauseTransition_Project(int x, int y, int angle, int *out_x, int *out_y,
                             int *out_width, int *out_height);
#ifdef __cplusplus
}
#endif
#endif
