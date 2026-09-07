#include <game/sprite_window.h>

/* Object shape/size byte for a block of at most eight by eight tiles; index
   zero on either axis means a full eight-tile edge. */
extern const u8 data_02049b68[64];
/* Object tile count, pixel width and pixel height per shape * 4 + size. */
extern const u16 data_02049bb0[16], data_02049bd0[16], data_02049bf0[16];

extern void MIi_CpuCopyFast(const void *source, void *destination, u32 size);
extern void MIi_CpuClearFast(u32 value, void *destination, u32 size);

void GameSpriteWindow_Upload(GameSpriteWindowManager *manager, GameWindow *window) {
    u8 index = window->sprite_slot;
    u16 columns = window->properties.shape.bits.width + 1;
    u16 rows = window->properties.shape.bits.height + 1;
    GameOamObject *object =
        (GameOamObject *)&manager->oam[index * GAME_SPRITE_WINDOW_OBJECTS];
    u8 *destination = GameSprite_ObjAddress(&manager->slots[index].allocation);
    u8 *source = window->front;
    s16 x, y;
    for (y = 0; y < rows; y += 8) {
        for (x = 0; x < columns; x += 8) {
            const u8 *block = source + x * 32;
            u8 kind;
            u16 object_width;
            int pitch;
            s16 block_rows = rows - y;
            u16 block_columns;
            u16 copy_bytes;
            int tail;
            s16 remaining;
            if (block_rows > 8) block_rows = 8;
            block_columns = columns - x;
            if (block_columns > 8) block_columns = 8;
            copy_bytes = block_columns * 32;
            kind = object->attributes.bits.shape * 4 + object->attributes.bits.size;
            object_width = data_02049bd0[kind];
            pitch = object_width * 4;
            tail = pitch - copy_bytes;
            if (tail <= 0) {
                remaining = block_rows;
                while (remaining > 0) {
                    MIi_CpuCopyFast(block, destination, copy_bytes);
                    block += columns * 32;
                    destination += pitch;
                    --remaining;
                }
            } else {
                remaining = block_rows;
                while (remaining > 0) {
                    MIi_CpuCopyFast(block, destination, copy_bytes);
                    MIi_CpuClearFast(0, (u32 *)destination + (copy_bytes >> 2), tail);
                    --remaining;
                    block += columns * 32;
                    destination += pitch;
                }
            }
            remaining = (data_02049bf0[kind] >> 3) - block_rows;
            while (remaining > 0) {
                MIi_CpuClearFast(0, destination, pitch);
                --remaining;
                destination += object_width * 4;
            }
            ++object;
        }
        source += columns * 256;
    }
}
