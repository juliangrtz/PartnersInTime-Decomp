#include <game/texture_allocation.h>

int GameTexturePalette_Allocate(GameTexturePalette *allocation, int mode, u8 kind, u16 colors,
u8 shared, u8 dirty, u8 slot, const void *data, u32 resource, u32 offset)
{
    u32 size = (u16)(2 * colors);
    GameTexturePalette *entry;
    allocation->offset = offset;
    allocation->size = size;
    allocation->resource = resource;
    allocation->data = data;
    allocation->state = 0;
    allocation->flags = (allocation->flags & ~15) | (slot & 15);
    allocation->flags = (allocation->flags & ~16) | ((shared & 1) << 4);
    allocation->flags = (allocation->flags & ~32) | ((dirty & 1) << 5);
    allocation->flags |= 0x80;
    allocation->kind = kind;
    if (mode == 2) {
        GameTexturePalette *current = data_0205a8b0;
        if (!current) {
            allocation->previous = 0;
            allocation->next = 0;
            data_0205a8b0 = allocation;
            data_0205a8bc = allocation;
            return mode;
        }
        while (current) {
            if (offset < current->offset) {
                allocation->previous = current->previous;
                allocation->next = current;
                if (current->previous) current->previous->next = allocation;
                if (current == data_0205a8b0) data_0205a8b0 = allocation;
                return mode;
            }
            current = current->next;
        }
        if (!current) {
            GameTexturePalette *tail = data_0205a8bc;
            allocation->previous = tail;
            allocation->next = 0;
            tail->next = allocation;
            data_0205a8bc = allocation;
            return mode;
        }
    }
    if (mode == 1 || shared) {
        GameTexturePalette *current = data_0205a8b0;
        while (current) {
            if (current->size == size && current->resource == resource && (current->state & 16)) {
                allocation->offset = current->offset;
                allocation->previous = current;
                allocation->next = current->next;
                if (current->next) current->next->previous = allocation;
                current->next = allocation;
                if (current == data_0205a8bc) data_0205a8bc = allocation;
                return mode;
            }
            current = current->next;
        }
    }
    entry = data_0205a8b0;
    if (!entry) {
        if (offset) allocation->offset = offset;
        else {
            u32 start;
            if (!mode) start = 0;
            else start = func_020354f4() - size;
            allocation->offset = start;
        }
        allocation->previous = 0;
        allocation->next = 0;
        data_0205a8b0 = allocation;
        data_0205a8bc = allocation;
        return mode;
    }
    {
        u32 start;
        u32 end;
        GameTexturePalette *previous;
        u32 available;
        previous = 0;
        end = 0;
        start = 0;
        if (offset) {
            while (entry) {
                u32 next_start = entry->offset;
                start = end;
                if (next_start < end) {
                    previous = entry;
                    end = next_start + entry->size;
                } else {
                    available = next_start - end;
                    if (offset < end + available) break;
                    previous = entry;
                    end = next_start + entry->size;
                }
                entry = entry->next;
            }
            if (previous == data_0205a8bc) {
                start = previous->offset + previous->size;
                available = func_020354dc() - start;
            }
            if (start < offset) {
                available -= offset - start;
                start = offset;
            }
            if (available >= size) {
                allocation->offset = start;
                allocation->previous = previous;
                allocation->next = entry;
                if (previous) previous->next = allocation;
                if (entry) entry->previous = allocation;
                if (!previous) data_0205a8b0 = allocation;
                if (!entry) data_0205a8bc = allocation;
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
                if (available >= size) {
                    allocation->offset = end;
                    allocation->previous = previous;
                    allocation->next = entry;
                    if (previous) previous->next = allocation;
                    if (entry) entry->previous = allocation;
                    if (!previous) data_0205a8b0 = allocation;
                    if (!entry) data_0205a8bc = allocation;
                    return mode;
                }
                previous = entry;
                end = next_start + entry->size;
            }
            entry = entry->next;
        }
        if (previous == data_0205a8bc) {
            start = previous->offset + previous->size;
            available = func_020354dc() - start;
        }
        if (available >= size) {
            allocation->offset = start;
            allocation->previous = previous;
            allocation->next = entry;
            if (previous) previous->next = allocation;
            if (entry) entry->previous = allocation;
            if (!previous) data_0205a8b0 = allocation;
            if (!entry) data_0205a8bc = allocation;
            return mode;
        }
    }
    return 0;
}
