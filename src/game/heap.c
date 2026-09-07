#include <game/heap.h>

extern GameHeapRegion data_02060b6c[32];

void *GameHeap_New(u32 size, int heap, void *unused, int mode)
{
    return GameHeap_Allocate(heap, size, unused, mode);
}

void *GameHeap_NewArray(u32 size, int heap, void *unused, int mode)
{
    return GameHeap_Allocate(heap, size, unused, mode);
}

void GameHeap_Delete(void *allocation)
{
    GameHeap_Free(allocation);
}

void GameHeap_DeleteArray(void *allocation)
{
    GameHeap_Free(allocation);
}

void GameHeap_Merge(GameHeapBlock *first, GameHeapBlock *second)
{
    first->size_flags += (second->size_flags & ~1) + sizeof(GameHeapBlock);
    first->next = second->next;
    if (first->next) first->next->previous = first;
}

void GameHeap_Free(void *allocation)
{
    GameHeapBlock *block;
    GameHeapBlock *previous;
    GameHeapBlock *next;
    u32 heap;
    if (!allocation) return;
    block = (GameHeapBlock *)allocation - 1;
    heap = block->heap;
    next = block->next;
    previous = block->previous;
    block->size_flags |= 1;
    if (next && (next->size_flags & 1)) {
        GameHeap_Merge(block, next);
        if (heap != 1 && data_02060b6c[heap].cursor == next) data_02060b6c[heap].cursor = block;
    }
    if (previous && (previous->size_flags & 1)) {
        GameHeap_Merge(previous, block);
        if (heap != 1) {
            if (data_02060b6c[heap].cursor == block) data_02060b6c[heap].cursor = previous;
        } else {
            if (data_02060b6c[heap].cursor == block) data_02060b6c[heap].cursor = block->next;
        }
    }
}

void *GameHeap_Allocate(int heap, u32 size, void *unused, int mode)
{
    GameHeapBlock *block;
    if (!size) goto failed;
    block = mode == 1 ? data_02060b6c[heap].cursor : data_02060b6c[heap].first;
    size = (size + 3) & ~3;
    if (heap == 1) block = block->previous;
    do {
        u32 size_flags = block->size_flags;
        if ((size_flags & 1) && size_flags >= size) {
            u32 available = size_flags & ~1;
            if (available > size + sizeof(GameHeapBlock)) {
                if (heap != 1) {
                    GameHeapBlock *remainder = (GameHeapBlock *)((u8 *)(block + 1) + size);
                    available -= size + sizeof(GameHeapBlock);
                    remainder->size_flags = available | 1;
                    remainder->previous = block;
                    remainder->next = block->next;
                    if (remainder->next) remainder->next->previous = remainder;
                    block->size_flags = size;
                    block->next = remainder;
                    if ((u32)data_02060b6c[heap].cursor < (u32)remainder) data_02060b6c[heap].cursor = remainder;
                } else {
                    GameHeapBlock *remainder = block;
                    block = (GameHeapBlock *)((u8 *)block + (available - size));
                    block->size_flags = size;
                    block->previous = remainder;
                    block->next = remainder->next;
                    if (block->next) block->next->previous = block;
                    size += sizeof(GameHeapBlock);
                    size = available - size;
                    remainder->size_flags = size | 1;
                    remainder->next = block;
                    if ((u32)data_02060b6c[heap].cursor > (u32)block) data_02060b6c[heap].cursor = block;
                }
            } else {
                block->size_flags = available;
            }
            block->heap = heap;
            return block + 1;
        }
        if (heap != 1) block = block->next;
        else block = block->previous;
    } while (block);
failed:
    return 0;
}

int GameHeap_Create(int parent, u32 size, u32 unused, int mode)
{
    void *allocation = GameHeap_Allocate(parent, size + sizeof(GameHeapBlock), 0, mode);
    int heap;
    if (!allocation) return -1;
    for (heap = 5; heap < 32; ++heap) {
        if (!data_02060b6c[heap].first) {
            GameHeapBlock *block;
            data_02060b6c[heap].first = allocation;
            data_02060b6c[heap].cursor = allocation;
            data_02060b6c[heap].size = size;
            data_02060b6c[heap].heap = heap;
            block = data_02060b6c[heap].first;
            block->next = 0;
            block->previous = block->next;
            block->size_flags = data_02060b6c[heap].size | 1;
            block->heap = heap;
            return heap;
        }
    }
    GameHeap_Free(allocation);
    return -1;
}

void GameHeap_Destroy(int heap)
{
    GameHeap_Free(data_02060b6c[heap].first);
    data_02060b6c[heap].first = 0;
    data_02060b6c[heap].cursor = 0;
    data_02060b6c[heap].size = 0;
}
