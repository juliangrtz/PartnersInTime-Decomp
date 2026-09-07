#include <game/window.h>
void GameWindow_ResetProperties(GameWindowManager *manager, GameWindowProperties *properties) {
    properties->shape.bits.screen = 0;
    properties->shape.bits.skin = 1;
    properties->shape.bits.width = 0;
    properties->shape.bits.height = 0;
    properties->position.bits.tile_pitch = 0;
    properties->position.bits.tile_height = 0;
    properties->shape.bits.position_mode = 0;
    properties->shape.bits.sound = 0;
    properties->layout.bits.mode = 0;
    properties->layout.bits.flag4 = 0;
    properties->layout.bits.style = 0;
    properties->layout.bits.flag9 = 0;
    properties->layout.bits.width = 0;
    properties->layout.bits.extent = 0;
    properties->layout.bits.reserved28 = 0;
    properties->position.bits.x = 0;
    properties->position.bits.y = 0;
    properties->position.bits.reserved18 = 0;
    properties->value = 0;
    properties->string = 0;
    properties->fonts = 0;
    properties->reserved18 = 0;
}
