#include <game/battle_context.h>
#include <game/battle_interface_text.h>
#include <game/battle_effect.h>
#define REG32(address) (*(volatile u32 *)(address))
static inline void TextTexCoord(int s, int t) {
    REG32(0x04000488) = (u16)(s16)(s >> 8) | ((u16)(s16)(t >> 8) << 16);
}
static inline void TextVertex(s16 x, s16 y, s16 z) {
    REG32(0x0400048c) = (u16)x | ((u16)y << 16);
    REG32(0x0400048c) = (u16)z;
}
/* Advances the tile buffer for text wider than its visible panel. */
void func_ov002_02092390(BattleInterfaceLayerState *state);
int BattleTexture_EncodeHeight(int), BattleTexture_EncodeWidth(int);
int _s32_div_f(int, int);
void BattleInterface_DrawText(BattleSpriteTransform *transform, BattleInterfaceLayerState *state) {
    BattleInterfaceLayer *layer = &state->layer;
    int columns, rows, height;
    u32 texture;
    int x, y, width, draw_height;
    if (!transform)
        return;
    if (layer->flags.bits.state_15)
        func_ov002_02092390(state);
    columns = layer->layout.bits.row_tiles_minus_one + 1;
    /* Keep the native division call before reading the texture allocation. */
    rows = _s32_div_f(layer->allocation_size, 32 * columns);
    texture = layer->vram_offset;
    height = BattleTexture_EncodeHeight(8 * rows);
    REG32(0x040004a8) = (texture >> 3) | 0x4c000000 |
                        (BattleTexture_EncodeWidth(8 * columns) << 20) | (height << 23) |
                        0x20030000;
    REG32(0x040004ac) = 0;
    REG32(0x04000500) = 1;
    x = layer->x;
    y = layer->y;
    width = (s16)layer->width;
    draw_height = (s16)layer->height;
    TextTexCoord((s16)state->transition_progress << 4, 0);
    TextVertex(x << 8, y << 8, -2);
    TextTexCoord((s16)state->transition_progress << 4, draw_height << 12);
    TextVertex(x << 8, (y + draw_height) << 8, -2);
    TextTexCoord(((s16)state->transition_progress << 4) + (width << 12), draw_height << 12);
    TextVertex((x + width) << 8, (y + draw_height) << 8, -2);
    TextTexCoord(((s16)state->transition_progress << 4) + (width << 12), 0);
    TextVertex((x + width) << 8, y << 8, -2);
    REG32(0x04000504) = 0;
}
