#include <game/window.h>
extern void DC_FlushRange(const void *, u32);
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
