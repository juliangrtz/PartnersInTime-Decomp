#include <game/sprite_window.h>

extern s16 func_0201d320(GameWindowManager *manager, const GameWindowProperties *properties, int requested_index);

s16 GameSpriteWindow_Open(GameSpriteWindowManager *manager, const GameWindowProperties *properties, int requested_index) {
    s16 free_slot;
    s16 index;
    GameWindow *window;
    int found = 0;
    free_slot = manager->state.bytes.count - 1;
    while (free_slot >= 0) {
        if (!manager->slots[free_slot].active) {
            found = 1;
            break;
        }
        --free_slot;
    }
    if (!found) return -1;
    index = func_0201d320(&manager->base, properties, requested_index);
    if (index == -1) return -1;
    manager->slots[free_slot].active = 1;
    manager->slots[free_slot].window = index;
    window = &manager->base.windows[index];
    window->sprite_slot = free_slot;
    GameSpriteAllocation_Allocate(&manager->slots[free_slot].allocation,
        (u8)window->properties.shape.bits.screen, 1,
        GameSpriteWindow_BuildObjects(manager, window), 0, 0xb8e1, 1, 0);
    GameSpriteWindow_ApplyTileOffsets(manager, window);
    return index;
}
