/*
 * 3D texture and texture-palette memory (ARM9 resident, 0x0200E4BC-0x0200F25C).
 *
 * The same free-list scheme the sprite side uses, for the texture VRAM the 3D
 * engine reads. An allocation is a node on an address-ordered list; palettes are
 * kept on a second list because they live in their own bank and are uploaded
 * separately. Buffers can be handed in from outside (SetBuffer/ReleaseBuffer)
 * for resources that already have storage elsewhere.
 */

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

void GameTextureAllocation_Unlink(GameTextureAllocation *allocation)
{
    if (data_0205a8c0 && data_0205a8c0 == allocation) data_0205a8c0 = allocation->next;
    if (data_0205a8b4 && data_0205a8b4 == allocation) data_0205a8b4 = allocation->previous;
    if (allocation->previous) allocation->previous->next = allocation->next;
    if (allocation->next) allocation->next->previous = allocation->previous;
    allocation->previous = allocation->next = 0;
    allocation->flags.raw &= ~8;
}

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

#include <game/heap.h>

void GameTexturePalette_Unlink(GameTexturePalette *palette)
{
    if (data_0205a8b0 && data_0205a8b0 == palette) data_0205a8b0 = palette->next;
    if (data_0205a8bc && data_0205a8bc == palette) data_0205a8bc = palette->previous;
    if (palette->previous) palette->previous->next = palette->next;
    if (palette->next) palette->next->previous = palette->previous;
    palette->previous = palette->next = 0;
    palette->flags &= ~0x80;
}

void GameTexturePalette_MarkDirty(GameTexturePalette *palette, const void *source)
{
    if (source) {
        GameSprite_CopyBytes(source, data_0205a8c4 + palette->offset, palette->size);
        palette->state |= 0x60;
    } else palette->state |= 0x20;
}

void *GameTexturePalette_GetBuffer(const GameTexturePalette *palette)
{
    return data_0205a8c4 + palette->offset;
}

void GameTexturePalette_Upload(const u32 *banks)
{
    GameTexturePalette *palette;
    for (palette = data_0205a8b0; palette; palette = palette->next) {
        if (palette->state & 0x20) {
            u32 destination = banks[palette->offset >> 14] + (palette->offset & 0x3fff);
            u32 size = palette->size;
            const void *source = palette->state & 0x40 ? data_0205a8c4 + palette->offset : palette->data;
            GameSprite_CopyBytes(source, (void *)destination, size);
            palette->state &= ~0x60;
        }
    }
}

void GameTexturePalette_SetBuffer(void *buffer, u32 size)
{
    data_0205a8c4 = buffer;
    data_0205a8b8 = size;
}

void GameTexturePalette_ReleaseBuffer(int release)
{
    if (release) {
        if (data_0205a8c4) {
            GameHeap_Free(data_0205a8c4);
            data_0205a8c4 = 0;
        }
        data_0205a8b8 = 0;
    } else {
        data_0205a8c4 = 0;
        data_0205a8b8 = 0;
    }
}
