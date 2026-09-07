#include <game/sprite_window.h>
#include <game/heap.h>
extern s16 func_0201d320(GameWindowManager *, const GameWindowProperties *, int);
extern void func_0201a714(GameWindowManager *, GameWindow *);

int GameSpriteWindow_OpenWithProperties(GameSpriteWindowManager *manager, const GameWindowProperties *properties, int requested_index)
{
    int index;
    /* Callers select either background (0) or sprite (1) presentation. */
    switch (properties->shape.bits.position_mode) {
    case 0:
        index = func_0201d320(&manager->base, properties, requested_index);
        if (index == -1) return -1;
        GameWindow_WriteTilemap(&manager->base, &manager->base.windows[(s16)index]);
        if ((u8)manager->base.windows[(s16)index].properties.shape.bits.screen)
            manager->base.state.raw |= 4;
        else
            manager->base.state.raw |= 2;
        break;
    case 1:
        index = GameSpriteWindow_Open(manager, properties, requested_index);
        if (index == -1) return -1;
        break;
    }
    func_0201a714(&manager->base, &manager->base.windows[(s16)index]);
    if (requested_index != -1) index = requested_index;
    return index;
}

extern u8 data_020565f0[];
extern void *data_020565d4[];
extern void MI_CpuFill8(void *destination, u8 value, u32 size);
extern void func_0202cbd4(void *destination, int value, u32 size);
extern GameWindowManager *func_0201f9b4(GameWindowManager *manager, int heap, int priority,
    u8 main_bg, u8 sub_bg, u8 main_priority, u8 sub_priority, const u32 *buffers,
    const u16 *tilemaps, int configure, GameSpriteAnimator *animator);
extern GameWindowManager *GameWindow_DestroyBase(GameWindowManager *);

void GameSpriteWindow_Release(GameSpriteWindowManager *manager, s16 index)
{
    if (!GameWindow_Release(&manager->base, index)) return;
    switch (manager->base.windows[index].properties.shape.bits.position_mode) {
    case 0:
        GameWindow_RebuildTilemap(&manager->base,
            (u8)manager->base.windows[index].properties.shape.bits.screen);
        return;
    case 1:
        GameSpriteWindow_Close(manager, index);
        return;
    }
}
void GameSpriteWindow_AllocateSlots(GameSpriteWindowManager *manager, int heap, int count) {
    /* One allocation holds the slot array followed by every slot's objects. */
    u32 slot_bytes = count * sizeof(GameSpriteWindowSlot);
    s16 index;
    GameSpriteWindowSlot *slot;
    manager->slots = GameHeap_NewArray(
        count * (GAME_SPRITE_WINDOW_OBJECTS * sizeof(GameOamEntry)) + slot_bytes,
        heap, data_020565f0, 0);
    func_0202cbd4(manager->slots, 0,
        count * (GAME_SPRITE_WINDOW_OBJECTS * sizeof(GameOamEntry)) + slot_bytes);
    manager->oam = (GameOamEntry *)((u8 *)manager->slots + slot_bytes);
    index = 0;
    if (count > 0) {
        do {
            slot = &manager->slots[index];
            MI_CpuFill8(slot, 0, sizeof(GameSpriteAnimation));
            slot->entry.affine = 0xffff;
            manager->slots[index].entry.data.direct.entries =
                &manager->oam[index * GAME_SPRITE_WINDOW_OBJECTS];
            ++index;
        } while (index < count);
    }
    manager->state.bytes.count = count;
}

void GameSpriteWindow_ReleaseSlots(GameSpriteWindowManager *manager) {
    GameSpriteWindowSlot *slot = manager->slots;
    s16 remaining;
    if (!slot) return;
    remaining = manager->state.bytes.count;
    while (remaining > 0) {
        if (slot->active) GameSpriteWindow_Close(manager, slot->window);
        --remaining;
        ++slot;
    }
    GameHeap_DeleteArray(manager->slots);
    manager->slots = 0;
    manager->state.bytes.count = 0;
}

GameSpriteWindowManager *GameSpriteWindow_Construct(GameSpriteWindowManager *manager, int heap,
        int priority, u8 main_bg, u8 sub_bg, u8 main_priority, u8 sub_priority,
        const u32 *buffers, const u16 *tilemaps, int configure, GameSpriteAnimator *animator) {
    func_0201f9b4(&manager->base, heap, priority, main_bg, sub_bg, main_priority, sub_priority,
                  buffers, tilemaps, configure, animator);
    manager->base.vtable = data_020565d4;
    manager->slots = 0;
    manager->state.bytes.count = 0;
    manager->state.bits.main_palette = 14;
    manager->state.bits.sub_palette = 14;
    return manager;
}

GameSpriteWindowManager *GameSpriteWindow_Destroy(GameSpriteWindowManager *manager) {
    manager->base.vtable = data_020565d4;
    GameIrqTask_Disable(manager->base.irq_task);
    GameSpriteWindow_ReleaseSlots(manager);
    GameWindow_DestroyBase(&manager->base);
    return manager;
}

GameSpriteWindowManager *GameSpriteWindow_Delete(GameSpriteWindowManager *manager) {
    manager->base.vtable = data_020565d4;
    GameIrqTask_Disable(manager->base.irq_task);
    GameSpriteWindow_ReleaseSlots(manager);
    GameWindow_DestroyBase(&manager->base);
    GameHeap_Delete(manager);
    return manager;
}
