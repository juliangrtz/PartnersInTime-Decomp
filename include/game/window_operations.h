#ifndef PIT_GAME_WINDOW_OPERATIONS_H
#define PIT_GAME_WINDOW_OPERATIONS_H
#include <game/window.h>
#ifdef __cplusplus
extern "C" {
#endif
int GameWindow_Open(GameWindowManager *manager, const GameWindowProperties *properties,
                    int requested_index);
void GameWindow_DrawSkin(GameWindowManager *manager, GameWindow *window);
#ifdef __cplusplus
}
#endif
#endif
