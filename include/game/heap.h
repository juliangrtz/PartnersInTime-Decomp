#ifndef PIT_GAME_HEAP_H
#define PIT_GAME_HEAP_H

#include <nitro.h>

typedef struct GameHeapBlock GameHeapBlock;
struct GameHeapBlock {
    GameHeapBlock *previous;
    GameHeapBlock *next;
    u32 size_flags;
    u32 heap : 5;
    u32 reserved : 27;
};

typedef struct GameHeapRegion {
    GameHeapBlock *first;
    GameHeapBlock *cursor;
    u32 size;
    u32 heap : 5;
    u32 reserved : 27;
} GameHeapRegion;

typedef char GameHeapBlockSizeCheck[(sizeof(GameHeapBlock) == 16) ? 1 : -1];
typedef char GameHeapRegionSizeCheck[(sizeof(GameHeapRegion) == 16) ? 1 : -1];

void GameHeap_Destroy(int heap);
int GameHeap_Create(int parent, u32 size, u32 unused, int mode);
void *GameHeap_Allocate(int heap, u32 size, void *unused, int mode);
void GameHeap_Free(void *allocation);
void GameHeap_Merge(GameHeapBlock *first, GameHeapBlock *second);
void GameHeap_DeleteArray(void *allocation);
void GameHeap_Delete(void *allocation);
void *GameHeap_NewArray(u32 size, int heap, void *unused, int mode);
void *GameHeap_New(u32 size, int heap, void *unused, int mode);

#endif
