#ifndef PIT_GAME_BATTLE_DISPLAY_CAPTURE_H
#define PIT_GAME_BATTLE_DISPLAY_CAPTURE_H

#include <nitro.h>

struct BattleSceneResource;

int BattleObjectData_QueueCaptureSurfaceUpload(
    struct BattleSceneResource *load_state, u16 parameter_1,
    u16 parameter_2);
int BattleDisplayCapture_QueueReset(void);
int BattleDisplayCapture_QueueConfigure(int capture_source);

#endif
