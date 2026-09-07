#include <game/sprite_output.h>

int GameSpriteAllocation_Allocate(GameSpriteAllocation *allocation, int screen, int mode, u32 tiles,
                                  u8 shared, u32 resource, u8 dirty, u32 first_tile)
{
    GameSpriteAllocation *entry;
    /* The search operates in bytes after converting the requested tile units. */
    tiles *= GameSprite_ObjBoundary(screen);
    first_tile *= GameSprite_ObjBoundary(screen);
    allocation->offset = first_tile;
    allocation->size = tiles;
    allocation->state = 0;
    allocation->flags.raw = (allocation->flags.raw & ~3) | (screen & 3);
    allocation->flags.raw = (allocation->flags.raw & ~4) | ((shared & 1) << 2);
    allocation->flags.raw = (allocation->flags.raw & ~8) | ((dirty & 1) << 3);
    allocation->flags.raw |= 0x20;
    allocation->resource = resource;
    if (mode == 2) {
        GameSpriteAllocation *current = data_0205a06c[screen];
        if (!current) {
            allocation->previous = 0;
            allocation->next = 0;
            data_0205a06c[screen] = allocation;
            data_0205a074[screen] = allocation;
            allocation->end = mode;
            return mode;
        }
        while (current) {
            if (first_tile < current->offset) {
                allocation->previous = current->previous;
                allocation->next = current;
                if (current->previous) current->previous->next = allocation;
                if (current == data_0205a06c[screen]) data_0205a06c[screen] = allocation;
                allocation->end = mode;
                return mode;
            }
            current = current->next;
        }
        if (!current) {
            GameSpriteAllocation *tail = data_0205a074[screen];
            allocation->previous = tail;
            allocation->next = 0;
            tail->next = allocation;
            allocation->end = mode;
            return mode;
        }
    }
    if (mode == 3 || shared) {
        GameSpriteAllocation *current = data_0205a06c[screen];
        while (current) {
            if (current->size == tiles && current->resource == resource && (current->state & 4)) {
                allocation->offset = current->offset;
                allocation->previous = current;
                allocation->next = current->next;
                if (current->next) current->next->previous = allocation;
                current->next = allocation;
                if (current == data_0205a074[screen]) data_0205a074[screen] = allocation;
                allocation->end = current->end;
                return 3;
            }
            current = current->next;
        }
        if (mode == 3) return 255;
    }
    entry = mode == 0 ? data_0205a06c[screen] : data_0205a074[screen];
    if (!entry) {
        if (!mode) allocation->offset = first_tile;
        else if (first_tile) allocation->offset = first_tile - tiles;
        else allocation->offset = GameSprite_ObjCapacity(screen) - tiles;
        allocation->previous = 0;
        allocation->next = 0;
        data_0205a06c[screen] = allocation;
        data_0205a074[screen] = allocation;
        allocation->end = mode;
        return mode;
    }
    if (!mode) {
        u32 end;
        GameSpriteAllocation *previous;
        u32 start;
        u32 available;
        previous = 0;
        end = 0;
        start = 0;
        if (first_tile) {
            while (entry) {
                u32 next_start = entry->offset;
                start = end;
                if (next_start < end) {
                previous = entry;
                end = next_start + entry->size;
            } else {
                    available = next_start - end;
                    if (first_tile < end + available) break;
                    previous = entry;
                    end = next_start + entry->size;
                }
                entry = entry->next;
            }
            if (previous == data_0205a074[screen]) {
                start = previous->offset + previous->size;
                available = GameSprite_ObjCapacity(screen) - start;
            }
            if (start < first_tile) {
                available -= first_tile - start;
                start = first_tile;
            }
            if (available >= tiles) {
                allocation->offset = start;
                allocation->previous = previous;
                allocation->next = entry;
                if (previous) previous->next = allocation;
                if (entry) entry->previous = allocation;
                if (!previous) data_0205a06c[screen] = allocation;
                if (!entry) data_0205a074[screen] = allocation;
                allocation->end = mode;
                return mode;
            }
            previous = entry;
            entry = entry->next;
        }
        while (entry) {
            u32 next_start = entry->offset;
            start = end;
            if (next_start < end) {
                previous = entry;
                end = next_start + entry->size;
            } else {
                available = next_start - end;
                if (available >= tiles) {
                    allocation->offset = end;
                    allocation->previous = previous;
                    allocation->next = entry;
                    if (previous) previous->next = allocation;
                    if (entry) entry->previous = allocation;
                    if (!previous) data_0205a06c[screen] = allocation;
                    if (!entry) data_0205a074[screen] = allocation;
                    allocation->end = mode;
                    return mode;
                }
                previous = entry;
                end = next_start + entry->size;
            }
            entry = entry->next;
        }
        if (previous == data_0205a074[screen]) {
            start = previous->offset + previous->size;
            available = GameSprite_ObjCapacity(screen) - start;
        }
        if (available >= tiles) {
            allocation->offset = start;
            allocation->previous = previous;
            allocation->next = entry;
            if (previous) previous->next = allocation;
            if (entry) entry->previous = allocation;
            if (!previous) data_0205a06c[screen] = allocation;
            if (!entry) data_0205a074[screen] = allocation;
            allocation->end = mode;
            return mode;
        }
    } else {
        GameSpriteAllocation *next = 0;
        u32 start = GameSprite_ObjCapacity(screen);
        u32 end = start;
        u32 available;
        if (first_tile) {
            while (entry) {
                end = entry->offset + entry->size;
                if (start < end) {
                start = entry->offset;
                next = entry;
            } else {
                    available = start - end;
                    if (end < first_tile - tiles) break;
                    start = entry->offset;
                    next = entry;
                }
                entry = entry->previous;
            }
            if (next == data_0205a06c[screen]) {
                start = next->offset;
                end = 0;
                available = next->offset;
            }
            if (end + available > first_tile) available -= end + available - first_tile;
            if (available >= tiles) {
                allocation->offset = end + available - tiles;
                allocation->previous = entry;
                allocation->next = next;
                if (entry) entry->next = allocation;
                if (next) next->previous = allocation;
                if (!entry) data_0205a06c[screen] = allocation;
                if (!next) data_0205a074[screen] = allocation;
                allocation->end = mode;
                return mode;
            }
            next = entry;
            entry = entry->previous;
        }
        while (entry) {
            end = entry->offset + entry->size;
            if (start < end) {
                start = entry->offset;
                next = entry;
            } else {
                available = start - end;
                if (available >= tiles) {
                    allocation->offset = end + available - tiles;
                    allocation->previous = entry;
                    allocation->next = next;
                    if (entry) entry->next = allocation;
                    if (next) next->previous = allocation;
                    if (!entry) data_0205a06c[screen] = allocation;
                    if (!next) data_0205a074[screen] = allocation;
                    allocation->end = mode;
                    return mode;
                }
                start = entry->offset;
                next = entry;
            }
            entry = entry->previous;
        }
        if (next == data_0205a06c[screen]) {
            available = next->offset;
            end = 0;
        }
        if (available >= tiles) {
            allocation->offset = end + available - tiles;
            allocation->previous = entry;
            allocation->next = next;
            if (entry) entry->next = allocation;
            if (next) next->previous = allocation;
            if (!entry) data_0205a06c[screen] = allocation;
            if (!next) data_0205a074[screen] = allocation;
            allocation->end = mode;
            return mode;
        }
    }
    return 0;
}
