#include <game/window.h>
extern void *data_02056534[];
extern u32 data_02056afc[];
extern u8 data_0205672c[], data_02056550[], data_02056560[], data_02056574[], data_02056584[],
    data_02056594[];
extern void func_0202cbd4(void *, int, u32);

static inline GameWindowManager *Construct(GameWindowManager *manager, int priority, int unused, u8 main_bg,
                                           u8 sub_bg, u8 main_priority, u8 sub_priority,
                                           const GameWindowBuffers *buffers,
                                           const GameWindowTilemaps *tilemaps, int configure,
                                           GameSpriteAnimator *animator)
{
    u32 buffer_bytes, tilemap_bytes, size;
    GameSpriteEffectPool *effects;
    GameIrqTask *irq;
    GameTask_Init((GameTask *)manager, priority, unused, 0);
    manager->vtable = data_02056534;
    manager->tiles = data_02056afc;
    manager->palette_data = data_0205672c;
    manager->state.bits.main_bg = main_bg;
    manager->state.bits.sub_bg = sub_bg;
    manager->state.bits.main_priority = main_priority;
    manager->state.bits.sub_priority = sub_priority;
    if (configure == 1) {
        GameWindow_ConfigureMainBackground(manager, 9, 0);
        GameWindow_ConfigureSubBackground(manager, 9, 0);
    }
    buffer_bytes = 0;
    tilemap_bytes = 0;
    if (buffers) {
        manager->main_buffer = buffers->main;
        manager->sub_buffer = buffers->sub;
        manager->main_capacity = !buffers->main ? 0 : buffers->main_capacity;
        manager->sub_capacity = !buffers->sub ? 0 : buffers->sub_capacity;
    } else {
        manager->main_capacity = 18432;
        manager->sub_capacity = 18432;
        buffer_bytes = 36864;
    }
    if (tilemaps) {
        manager->main_tilemap = tilemaps->main;
        manager->sub_tilemap = tilemaps->sub;
        manager->main_map_size = !tilemaps->main ? 0 : tilemaps->main_size;
        manager->sub_map_size = !tilemaps->sub ? 0 : tilemaps->sub_size;
    } else {
        manager->main_map_size = 1920;
        manager->sub_map_size = 1920;
        tilemap_bytes = 3840;
    }
    manager->owned_buffer = 0;
    size = buffer_bytes + tilemap_bytes;
    if (size)
        manager->owned_buffer = GameHeap_NewArray(size, 0, data_02056550, 0);
    if (!buffers) {
        manager->main_buffer = manager->owned_buffer + tilemap_bytes;
        manager->sub_buffer = manager->main_buffer + 18432;
    }
    if (!tilemaps) {
        manager->main_tilemap = (u16 *)manager->owned_buffer;
        manager->sub_tilemap = (u16 *)(manager->owned_buffer + 1920);
    }
    if (!tilemaps) {
        func_0202cbd4(manager->main_tilemap, 0, 1920);
        func_0202cbd4(manager->sub_tilemap, 0, 1920);
    } else {
        if (tilemaps->main)
            func_0202cbd4(manager->main_tilemap, 0, 1920);
        if (tilemaps->sub)
            func_0202cbd4(manager->sub_tilemap, 0, 1920);
    }
    effects = GameHeap_New(40, 0, data_02056560, 0);
    if (effects)
        effects = GameSpriteEffectPool_Init(effects, 30);
    manager->effects = effects;
    manager->animator = animator;
    if (!animator) {
        animator = GameHeap_New(108, 0, data_02056574, 0);
        if (animator)
            animator = GameSpriteAnimator_Construct(animator);
        manager->animator = animator;
    }
    irq = GameHeap_New(40, 0, data_02056584, 0);
    if (irq)
        irq = GameWindowIrq_Init(irq, 8, (u32)data_02056594, manager);
    manager->irq_task = irq;
    manager->state.bits.enabled = 1;
    manager->display.bits.fixed_main_scroll = 0;
    manager->display.bits.fixed_sub_scroll = 0;
    manager->state.bits.main_map_dirty = 0;
    manager->state.bits.sub_map_dirty = 0;
    manager->state.bits.dirty = 0;
    manager->state.bits.main_count = 0;
    manager->state.bits.sub_count = 0;
    manager->main_origin_x = 0;
    manager->main_origin_y = 0;
    manager->sub_origin_x = 0;
    manager->sub_origin_y = 0;
    manager->main_scroll_x = 0;
    manager->main_scroll_y = 0;
    manager->sub_scroll_x = 0;
    manager->sub_scroll_y = 0;
    manager->windows = 0;
    manager->window_capacity = 0;
    manager->reservedf3c = 3843;
    manager->reservedf3e = 2;
    manager->display.bits.main_palette = 0;
    manager->display.bits.sub_palette = 0;
    manager->state.bits.main_bg_palette = 15;
    manager->state.bits.sub_bg_palette = 15;
    manager->main_head_marker = (GameWindow *)0x11111111;
    manager->main_windows = (GameWindow *)&manager->main_last;
    manager->main_last = (GameWindow *)&manager->main_head_marker;
    manager->main_tail_marker = (GameWindow *)-1;
    manager->sub_head_marker = (GameWindow *)0x11111111;
    manager->sub_windows = (GameWindow *)&manager->sub_last;
    manager->sub_last = (GameWindow *)&manager->sub_head_marker;
    manager->sub_tail_marker = (GameWindow *)-1;
    GameText_SetControl(11, 1, 0, 0);
    GameText_SetControl(12, 1, 0, 0);
    GameText_SetControl(15, 1, GameWindow_MeasureNumber, manager);
    GameText_SetControl(16, 1, GameWindow_MeasurePaddedNumber, manager);
    GameText_SetControl(17, 1, 0, 0);
    GameText_SetControl(18, 1, 0, 0);
    GameIrqTask_Enable(manager->irq_task);
    return manager;
}
GameWindowManager *GameWindow_ConstructBase(GameWindowManager *manager, int priority, int unused, u8 main_bg,
                                            u8 sub_bg, u8 main_priority, u8 sub_priority,
                                            const GameWindowBuffers *buffers,
                                            const GameWindowTilemaps *tilemaps, int configure,
                                            GameSpriteAnimator *animator)
{
    return Construct(manager, priority, unused, main_bg, sub_bg, main_priority, sub_priority, buffers,
                     tilemaps, configure, animator);
}
GameWindowManager *GameWindow_ConstructComplete(GameWindowManager *manager, int priority, int unused,
                                                u8 main_bg, u8 sub_bg, u8 main_priority, u8 sub_priority,
                                                const GameWindowBuffers *buffers,
                                                const GameWindowTilemaps *tilemaps, int configure,
                                                GameSpriteAnimator *animator)
{
    return Construct(manager, priority, unused, main_bg, sub_bg, main_priority, sub_priority, buffers,
                     tilemaps, configure, animator);
}
