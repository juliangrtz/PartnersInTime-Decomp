#ifndef PIT_GAME_PAUSE_HP_WARNING_H
#define PIT_GAME_PAUSE_HP_WARNING_H
#include <game/pause_scene.h>

enum PauseHpWarningMode {
    PAUSE_HP_WARNING_DISABLED,
    PAUSE_HP_WARNING_ACTIVE,
    PAUSE_HP_WARNING_SUSPENDED
};

typedef struct PauseHpWarningTask {
    u8 unknown_00[40];
    int member, x, y;
    u8 unknown_34[20];
} PauseHpWarningTask;
typedef char PauseHpWarningTaskSize[sizeof(PauseHpWarningTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void PauseHpWarning_Stop(void);
void PauseHpWarning_Update(PauseHpWarningTask *task);
#ifdef __cplusplus
}
#endif
#endif
