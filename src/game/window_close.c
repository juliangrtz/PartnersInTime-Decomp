#include <game/window.h>
void GameWindow_Close(GameWindowManager *manager, s16 index) {
    if (GameWindow_Release(manager, index)) GameWindow_RebuildTilemap(manager, (u8)manager->windows[index].properties.shape.bits.screen);
}
