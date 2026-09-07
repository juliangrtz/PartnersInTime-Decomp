#include <game/sprite_window.h>

/* Object shape/size byte for a block of at most eight by eight tiles; index
   zero on either axis means a full eight-tile edge. */
extern const u8 data_02049b68[64];
/* Object tile count, pixel width and pixel height per shape * 4 + size. */
extern const u16 data_02049bb0[16], data_02049bd0[16], data_02049bf0[16];

u16 GameSpriteWindow_BuildObjects(GameSpriteWindowManager *manager, GameWindow *window) {
    u32 offset = 0;
    u8 screen = window->properties.shape.bits.screen;
    s16 columns = window->properties.shape.bits.width + 1;
    s16 x;
    GameSpriteWindowSlot *slot = GameSpriteWindow_Slot(manager, window);
    GameOamObject *object = (GameOamObject *)slot->entry.data.direct.entries;
    s16 rows = window->properties.shape.bits.height + 1;
    u8 boundary = GameSprite_ObjBoundary(screen);
    u8 shift = GameSprite_ObjBoundaryShift(screen) + 5;
    s16 y;
    slot->entry.data.direct.count = 0;
    for (y = 0; y < rows; y += 8) {
        for (x = 0; x < columns; x += 8) {
            u8 wide = columns - x;
            u8 tall = rows - y;
            u32 kind;
            u8 shape;
            u8 size;
            if (wide >= 8) wide = 0;
            if (tall >= 8) tall = 0;
            kind = data_02049b68[wide + tall * 8];
            size = kind & 3;
            shape = kind >> 4;
            object->attributes.bits.shape = shape;
            object->attributes.bits.size = size;
            object->tile.bits.tile = offset >> shift;
            object->attributes.bits.x = x * 8;
            object->attributes.bytes.y = y * 8;
            offset += data_02049bb0[size + shape * 4] * 32;
            offset = (offset + boundary - 1) & ~(boundary - 1);
            ++slot->entry.data.direct.count;
            ++object;
        }
    }
    GameSpriteAnimator_Add(manager->base.animator, screen, -1,
        (u16)window->properties.position.bits.x, (u16)window->properties.position.bits.y, 0,
        screen ? manager->base.state.bits.sub_priority : manager->base.state.bits.main_priority,
        screen ? manager->state.bits.sub_palette : manager->state.bits.main_palette,
        0, &slot->entry);
    return offset >> shift;
}
