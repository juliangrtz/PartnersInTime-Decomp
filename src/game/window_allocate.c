#include <game/window.h>
GameWindow *GameWindow_Allocate(GameWindowManager *manager, int screen, u32 size, s16 requested_index) {
    u8 *base;
    GameWindow *head;
    u8 *position;
    s16 index;
    GameWindow *window;
    u32 capacity;
    int found;
    GameWindow *current;
    if (manager->window_capacity == manager->state.bits.main_count + manager->state.bits.sub_count) return 0;
    found = 0;
    if (!screen) {
        base = manager->main_buffer;
        head = manager->main_windows;
        capacity = manager->main_capacity;
    } else {
        base = manager->sub_buffer;
        head = manager->sub_windows;
        capacity = manager->sub_capacity;
    }
    current = head;
    position = base;
    for (;;) {
        if (current->next == (GameWindow *)-1) {
            if ((u32)(base + capacity - position) >= size) found = 1;
            break;
        } else {
            u32 gap = current->front - position;
            if (gap >= size) { found = 1; break; }
            position += gap + current->state.bits.size;
            current = current->next;
        }
    }
    if (!found) return 0;
    index = 0;
    if (requested_index != -1) {
        window = &manager->windows[(s16)requested_index];
        index = requested_index;
    } else {
        for (window = manager->windows; window->front; ++window) ++index;
    }
    MI_CpuFill8(window, 0, sizeof(*window));
    window->front = position;
    window->state.bits.size = size;
    /* Tile zero is reserved; buffer offsets start at tile one in VRAM. */
    window->allocation.bits.tile_offset = ((u32)(position - base) >> 5) + 1;
    window->allocation.bits.group = index;
    if (!manager->state.bits.buffer_mode) {
        u8 *back = screen ? manager->back_buffer + capacity : manager->back_buffer;
        window->back = back + (position - base);
    }
    while (head->next != (GameWindow *)-1) {
        if (head->front > window->front) break;
        head = head->next;
    }
    window->previous = head->previous;
    window->next = head->previous->next;
    head->previous->next = window;
    head->previous = window;
    if (!screen) ++manager->state.bits.main_count;
    else ++manager->state.bits.sub_count;
    return window;
}
