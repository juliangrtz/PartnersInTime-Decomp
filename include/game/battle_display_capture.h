#ifndef PIT_GAME_BATTLE_DISPLAY_CAPTURE_H
#define PIT_GAME_BATTLE_DISPLAY_CAPTURE_H

struct BattleSceneResource;

int BattleObjectData_QueueCaptureSurfaceUpload(
    struct BattleSceneResource *load_state);
int BattleDisplayCapture_QueueReset(void);
int BattleDisplayCapture_QueueConfigure(int capture_source);

#endif
