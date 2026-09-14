#include <game/window_operations.h>

extern s16 func_0201d320(GameWindowManager *, const GameWindowProperties *, int);
extern void func_0201a714(GameWindowManager *, GameWindow *);

int GameWindow_Open(GameWindowManager *manager, const GameWindowProperties *properties,
                    int requested_index) {
    int index = func_0201d320(manager, properties, requested_index);
    if (index == -1)
        return -1;
    manager->windows[(s16)index].properties.shape.bits.position_mode = 0;
    GameWindow_WriteTilemap(manager, &manager->windows[(s16)index]);
    if ((u8)manager->windows[(s16)index].properties.shape.bits.screen)
        manager->state.bits.sub_map_dirty = 1;
    else
        manager->state.bits.main_map_dirty = 1;
    func_0201a714(manager, &manager->windows[(s16)index]);
    if (requested_index != -1)
        index = requested_index;
    return index;
}
