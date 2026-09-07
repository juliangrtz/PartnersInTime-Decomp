#include <game/window.h>
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
