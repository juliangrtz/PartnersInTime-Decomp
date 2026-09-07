#include <game/texture_allocation.h>

int GameTextureAllocation_Allocate(GameTextureAllocation *allocation, int mode, u32 size, int shared,
u32 resource, int unused, u8 dirty, u32 offset)
{
    GameTextureAllocation *entry;
    allocation->offset = offset;
    allocation->size = size;
    allocation->state = 0;
    allocation->flags.raw = (allocation->flags.raw & ~1) | (shared & 1);
    allocation->flags.raw = (allocation->flags.raw & ~2) | ((dirty & 1) << 1);
    allocation->flags.raw |= 8;
    allocation->resource = resource;
    if (mode == 2) {
        GameTextureAllocation *current = data_0205a8c0;
        if (!current) {
            allocation->previous = 0;
            allocation->next = 0;
            data_0205a8c0 = allocation;
            data_0205a8b4 = allocation;
            allocation->end = mode;
            return mode;
        }
        while (current) {
            if (offset == current->offset && shared) {
                allocation->offset = current->offset;
                allocation->previous = current;
                allocation->next = current->next;
                if (current->next) current->next->previous = allocation;
                current->next = allocation;
                if (current == data_0205a8b4) data_0205a8b4 = allocation;
                allocation->end = current->end;
                return 3;
            }
            if (offset < current->offset) {
                allocation->previous = current->previous;
                allocation->next = current;
                if (current->previous) current->previous->next = allocation;
                if (current == data_0205a8c0) data_0205a8c0 = allocation;
                allocation->end = mode;
                return mode;
            }
            current = current->next;
        }
        if (!current) {
            GameTextureAllocation *tail = data_0205a8b4;
            allocation->previous = tail;
            allocation->next = 0;
            tail->next = allocation;
            allocation->end = mode;
            return mode;
        }
    }
    if (mode == 3 || shared) {
        GameTextureAllocation *current = data_0205a8c0;
        while (current) {
            if (current->size == size && current->resource == resource && (current->state & 1)) {
                allocation->offset = current->offset;
                allocation->previous = current;
                allocation->next = current->next;
                if (current->next) current->next->previous = allocation;
                current->next = allocation;
                if (current == data_0205a8b4) data_0205a8b4 = allocation;
                allocation->end = current->end;
                return 3;
            }
            current = current->next;
        }
    }
    entry = mode == 0 ? data_0205a8c0 : data_0205a8b4;
    if (!entry) {
        if (!mode) allocation->offset = offset;
        else if (offset) allocation->offset = offset - size;
        else allocation->offset = func_020354f4() - size;
        allocation->previous = 0;
        allocation->next = 0;
        data_0205a8c0 = allocation;
        data_0205a8b4 = allocation;
        allocation->end = mode;
        return mode;
    }
    if (!mode) {
        u32 end;
        GameTextureAllocation *previous;
        u32 start;
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
            if (previous == data_0205a8b4) {
                start = previous->offset + previous->size;
                available = func_020354f4() - start;
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
                if (!previous) data_0205a8c0 = allocation;
                if (!entry) data_0205a8b4 = allocation;
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
                if (available >= size) {
                    allocation->offset = end;
                    allocation->previous = previous;
                    allocation->next = entry;
                    if (previous) previous->next = allocation;
                    if (entry) entry->previous = allocation;
                    if (!previous) data_0205a8c0 = allocation;
                    if (!entry) data_0205a8b4 = allocation;
                    allocation->end = mode;
                    return mode;
                }
                previous = entry;
                end = next_start + entry->size;
            }
            entry = entry->next;
        }
        if (previous == data_0205a8b4) {
            start = previous->offset + previous->size;
            available = func_020354f4() - start;
        }
        if (available >= size) {
            allocation->offset = start;
            allocation->previous = previous;
            allocation->next = entry;
            if (previous) previous->next = allocation;
            if (entry) entry->previous = allocation;
            if (!previous) data_0205a8c0 = allocation;
            if (!entry) data_0205a8b4 = allocation;
            allocation->end = mode;
            return mode;
        }
    } else {
        GameTextureAllocation *next = 0;
        u32 start = func_020354f4();
        u32 end = start;
        u32 available;
        if (offset) {
            while (entry) {
                end = entry->offset + entry->size;
                if (start < end) {
                    start = entry->offset;
                    next = entry;
                } else {
                    available = start - end;
                    if (end < offset - size) break;
                    start = entry->offset;
                    next = entry;
                }
                entry = entry->previous;
            }
            if (next == data_0205a8c0) {
                start = next->offset;
                end = 0;
                available = next->offset;
            }
            if (end + available > offset) available -= end + available - offset;
            if (available >= size) {
                allocation->offset = end + available - size;
                allocation->previous = entry;
                allocation->next = next;
                if (entry) entry->next = allocation;
                if (next) next->previous = allocation;
                if (!entry) data_0205a8c0 = allocation;
                if (!next) data_0205a8b4 = allocation;
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
                if (available >= size) {
                    allocation->offset = end + available - size;
                    allocation->previous = entry;
                    allocation->next = next;
                    if (entry) entry->next = allocation;
                    if (next) next->previous = allocation;
                    if (!entry) data_0205a8c0 = allocation;
                    if (!next) data_0205a8b4 = allocation;
                    allocation->end = mode;
                    return mode;
                }
                start = entry->offset;
                next = entry;
            }
            entry = entry->previous;
        }
        if (next == data_0205a8c0) {
            available = next->offset;
            end = 0;
        }
        if (available >= size) {
            allocation->offset = end + available - size;
            allocation->previous = entry;
            allocation->next = next;
            if (entry) entry->next = allocation;
            if (next) next->previous = allocation;
            if (!entry) data_0205a8c0 = allocation;
            if (!next) data_0205a8b4 = allocation;
            allocation->end = mode;
            return mode;
        }
    }
    return 0;
}
