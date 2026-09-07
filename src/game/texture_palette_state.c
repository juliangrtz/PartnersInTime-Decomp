#include <game/texture_allocation.h>
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
