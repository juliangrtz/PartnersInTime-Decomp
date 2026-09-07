#include <game/window.h>
void GameWindow_WriteTilemap(GameWindowManager *manager, GameWindow *window) {
    GameWindowProperties *properties = &window->properties;
    s16 x, y;
    u16 start_x = properties->position.bits.x >> 3;
    u16 start_y = properties->position.bits.y >> 3;
    u16 tile = window->allocation.bits.tile_offset;
    u16 *map;
    if (!window->properties.shape.bits.screen) {
        map = manager->main_tilemap;
        tile += manager->state.bits.main_bg_palette << 12;
    } else {
        map = manager->sub_tilemap;
        tile += manager->state.bits.sub_bg_palette << 12;
    }
    y = start_y;
    if (y < start_y + properties->position.bits.tile_height + 1) {
        int row = y * 32;
        do {
            for (x = start_x; x < start_x + properties->position.bits.tile_pitch + 1; ++x) {
                map[row + x] = tile++;
            }
            ++y;
            row += 32;
        } while (y < start_y + properties->position.bits.tile_height + 1);
    }
}
