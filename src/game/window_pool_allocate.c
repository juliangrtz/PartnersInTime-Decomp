#include <game/window.h>
extern void func_0202cbd4(void *, int, u32);
extern char data_020565a4[];
static inline void InitLink(GameWindowLink *link) {
    MI_CpuFill8(link, 0, sizeof(*link));
    link->id = 0xffff;
}
void GameWindow_AllocatePool(GameWindowManager *manager, int heap, u32 count, int mode) {
    u32 window_bytes;
    u32 size;
    s16 i;
    if (!count) return;
    window_bytes = count * sizeof(GameWindow);
    size = window_bytes + (mode == 0 ? manager->main_capacity + manager->sub_capacity : count * sizeof(GameWindowTextCache));
    manager->windows = GameHeap_NewArray(size, heap, data_020565a4, 0);
    func_0202cbd4(manager->windows, 0, size);
    if (manager->main_capacity + manager->sub_capacity) manager->back_buffer = (u8 *)manager->windows + window_bytes;
    for (i = count - 1; i >= 0; --i) {
        InitLink(&manager->windows[i].links[0]);
        InitLink(&manager->windows[i].links[1]);
    }
    manager->state.bits.buffer_mode = mode;
    manager->window_capacity = count;
}
