/*
 * Window backgrounds (ARM9 resident, 0x0201AB54-0x0201B4CC).
 *
 * Configuring the backgrounds windows are drawn on, writing a window into the
 * tilemap, uploading the dirty parts and applying the scroll.
 */

#include <game/window.h>
void GameWindow_WriteTilemap(GameWindowManager *manager, GameWindow *window) {
    GameWindowProperties *properties = &window->properties;
    s16 x, y;
    u16 start_x = properties->position.bits.x >> 3;
    u16 start_y = properties->position.bits.y >> 3;
    u16 tile = window->allocation.bits.tile_offset;
    u16 *map;
    if (!window->properties.shape.bits.screen) {
        map = manager->main_tilemap;
        tile += manager->state.bits.main_bg_palette << 12;
    } else {
        map = manager->sub_tilemap;
        tile += manager->state.bits.sub_bg_palette << 12;
    }
    y = start_y;
    if (y < start_y + properties->position.bits.tile_height + 1) {
        int row = y * 32;
        do {
            for (x = start_x; x < start_x + properties->position.bits.tile_pitch + 1; ++x) {
                map[row + x] = tile++;
            }
            ++y;
            row += 32;
        } while (y < start_y + properties->position.bits.tile_height + 1);
    }
}

extern void DC_FlushRange(const void *, u32);
extern void GameWindow_Upload(GameWindowManager *, GameWindow *);
extern void func_02038448(const void *, u32, u32);
extern void func_02038378(const void *, u32, u32);
extern void func_020382a8(const void *, u32, u32);
extern void func_020381d8(const void *, u32, u32);
extern void func_020384b0(const void *, u32, u32);
extern void func_020383e0(const void *, u32, u32);
extern void func_02038310(const void *, u32, u32);
extern void func_02038240(const void *, u32, u32);

void GameWindow_ConfigureMainBackground(GameWindowManager *manager, int screen_base, int char_base) {
    switch (manager->state.bits.main_bg) {
    case 0:
        (*(vu16 *)0x04000008) = ((*(vu16 *)0x04000008) & 0x43) | (screen_base << 8) | (char_base << 2);
        (*(vu16 *)0x04000008) = ((*(vu16 *)0x04000008) & ~3) | manager->state.bits.main_priority;
        break;
    case 1:
        (*(vu16 *)0x0400000a) = ((*(vu16 *)0x0400000a) & 0x43) | (screen_base << 8) | (char_base << 2);
        (*(vu16 *)0x0400000a) = ((*(vu16 *)0x0400000a) & ~3) | manager->state.bits.main_priority;
        break;
    case 2:
        (*(vu16 *)0x0400000c) = ((*(vu16 *)0x0400000c) & 0x43) | (screen_base << 8) | (char_base << 2);
        (*(vu16 *)0x0400000c) = ((*(vu16 *)0x0400000c) & ~3) | manager->state.bits.main_priority;
        break;
    case 3:
        (*(vu16 *)0x0400000e) = ((*(vu16 *)0x0400000e) & 0x43) | (screen_base << 8) | (char_base << 2);
        (*(vu16 *)0x0400000e) = ((*(vu16 *)0x0400000e) & ~3) | manager->state.bits.main_priority;
        break;
    }
}

void GameWindow_ConfigureSubBackground(GameWindowManager *manager, int screen_base, int char_base) {
    switch (manager->state.bits.sub_bg) {
    case 0:
        (*(vu16 *)0x04001008) = ((*(vu16 *)0x04001008) & 0x43) | (screen_base << 8) | (char_base << 2);
        (*(vu16 *)0x04001008) = ((*(vu16 *)0x04001008) & ~3) | manager->state.bits.sub_priority;
        break;
    case 1:
        (*(vu16 *)0x0400100a) = ((*(vu16 *)0x0400100a) & 0x43) | (screen_base << 8) | (char_base << 2);
        (*(vu16 *)0x0400100a) = ((*(vu16 *)0x0400100a) & ~3) | manager->state.bits.sub_priority;
        break;
    case 2:
        (*(vu16 *)0x0400100c) = ((*(vu16 *)0x0400100c) & 0x43) | (screen_base << 8) | (char_base << 2);
        (*(vu16 *)0x0400100c) = ((*(vu16 *)0x0400100c) & ~3) | manager->state.bits.sub_priority;
        break;
    case 3:
        (*(vu16 *)0x0400100e) = ((*(vu16 *)0x0400100e) & 0x43) | (screen_base << 8) | (char_base << 2);
        (*(vu16 *)0x0400100e) = ((*(vu16 *)0x0400100e) & ~3) | manager->state.bits.sub_priority;
        break;
    }
}

void GameWindow_UploadMainTilemap(GameWindowManager *manager) {
    manager->state.bits.main_map_dirty = 0;
    DC_FlushRange(manager->main_tilemap, 1920);
    switch (manager->state.bits.main_bg) {
    case 0: func_020384b0(manager->main_tilemap, 128, manager->main_map_size); break;
    case 1: func_020383e0(manager->main_tilemap, 128, manager->main_map_size); break;
    case 2: func_02038310(manager->main_tilemap, 128, manager->main_map_size); break;
    case 3: func_02038240(manager->main_tilemap, 128, manager->main_map_size); break;
    }
}

void GameWindow_UploadSubTilemap(GameWindowManager *manager) {
    manager->state.bits.sub_map_dirty = 0;
    DC_FlushRange(manager->sub_tilemap, 1920);
    switch (manager->state.bits.sub_bg) {
    case 0: func_02038448(manager->sub_tilemap, 128, manager->sub_map_size); break;
    case 1: func_02038378(manager->sub_tilemap, 128, manager->sub_map_size); break;
    case 2: func_020382a8(manager->sub_tilemap, 128, manager->sub_map_size); break;
    case 3: func_020381d8(manager->sub_tilemap, 128, manager->sub_map_size); break;
    }
}

void GameWindow_UploadDirty(GameWindowManager *manager) {
    GameWindow *window;
    manager->state.bits.dirty = 0;
    for (window = manager->main_windows; window->next != (GameWindow *)-1; window = window->next) {
        if (window->allocation.bits.dirty) {
            window->allocation.bits.dirty = 0;
            GameWindow_Upload(manager, window);
        }
    }
    for (window = manager->sub_windows; window->next != (GameWindow *)-1; window = window->next) {
        if (window->allocation.bits.dirty) {
            window->allocation.bits.dirty = 0;
            GameWindow_Upload(manager, window);
        }
    }
}

extern void func_02038170(const void *, u32, u32);
extern void func_020380a0(const void *, u32, u32);
extern void func_02037fd0(const void *, u32, u32);
extern void func_02037f00(const void *, u32, u32);
extern void func_02038108(const void *, u32, u32);
extern void func_02038038(const void *, u32, u32);
extern void func_02037f68(const void *, u32, u32);
extern void func_02037e98(const void *, u32, u32);

void GameWindow_Upload(GameWindowManager *manager, GameWindow *window) {
    DC_FlushRange(window->front, window->state.bits.size);
    if (!window->properties.shape.bits.screen) {
        switch (manager->state.bits.main_bg) {
        case 0: func_02038170(window->front, window->allocation.bits.tile_offset * 32, window->state.bits.size); break;
        case 1: func_020380a0(window->front, window->allocation.bits.tile_offset * 32, window->state.bits.size); break;
        case 2: func_02037fd0(window->front, window->allocation.bits.tile_offset * 32, window->state.bits.size); break;
        case 3: func_02037f00(window->front, window->allocation.bits.tile_offset * 32, window->state.bits.size); break;
        }
    } else {
        switch (manager->state.bits.sub_bg) {
        case 0: func_02038108(window->front, window->allocation.bits.tile_offset * 32, window->state.bits.size); break;
        case 1: func_02038038(window->front, window->allocation.bits.tile_offset * 32, window->state.bits.size); break;
        case 2: func_02037f68(window->front, window->allocation.bits.tile_offset * 32, window->state.bits.size); break;
        case 3: func_02037e98(window->front, window->allocation.bits.tile_offset * 32, window->state.bits.size); break;
        }
    }
}

void GameWindow_ApplyScroll(GameWindowManager *manager) {
    if (!manager->display.bits.fixed_main_scroll) {
        switch (manager->state.bits.main_bg) {
        case 0: *(vu32 *)0x04000010 = (manager->main_scroll_x & 0x1ff) | (((manager->main_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 1: *(vu32 *)0x04000014 = (manager->main_scroll_x & 0x1ff) | (((manager->main_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 2: *(vu32 *)0x04000018 = (manager->main_scroll_x & 0x1ff) | (((manager->main_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 3: *(vu32 *)0x0400001c = (manager->main_scroll_x & 0x1ff) | (((manager->main_scroll_y + 16) << 16) & 0x1ff0000); break;
        }
    }
    if (!manager->display.bits.fixed_sub_scroll) {
        switch (manager->state.bits.sub_bg) {
        case 0: *(vu32 *)0x04001010 = (manager->sub_scroll_x & 0x1ff) | (((manager->sub_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 1: *(vu32 *)0x04001014 = (manager->sub_scroll_x & 0x1ff) | (((manager->sub_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 2: *(vu32 *)0x04001018 = (manager->sub_scroll_x & 0x1ff) | (((manager->sub_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 3: *(vu32 *)0x0400101c = (manager->sub_scroll_x & 0x1ff) | (((manager->sub_scroll_y + 16) << 16) & 0x1ff0000); break;
        }
    }
}
