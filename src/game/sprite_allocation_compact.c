#include <game/sprite_output.h>

int GameSpriteAllocation_Compact(int screen, int reverse, int dirty)
{
    u32 moved = 0;
    if (!reverse) {
        u32 previous_end = 0;
        GameSpriteAllocation *entry;
        for (entry = data_0205a06c[screen];
             entry && entry->end == 0 && entry->flags.bits.dirty == dirty;
             entry = entry->next) {
            u32 offset = entry->offset;
            u32 size = entry->size;
            moved += offset - previous_end;
            entry->offset -= moved;
            previous_end = offset + size;
            if (moved) entry->state |= 0x10;
        }
        if (moved) return 1;
    } else {
        u32 previous_start = GameSprite_ObjCapacity(screen);
        GameSpriteAllocation *entry;
        for (entry = data_0205a074[screen];
             entry && entry->end == 1 && entry->flags.bits.dirty == dirty;
             entry = entry->previous) {
            u32 offset = entry->offset;
            moved += previous_start - (offset + entry->size);
            entry->offset += moved;
            previous_start = offset;
            if (moved) entry->state |= 0x10;
        }
        if (moved) return 1;
    }
    return 0;
}
