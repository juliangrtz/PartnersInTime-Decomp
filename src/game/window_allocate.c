#include <game/window.h>
void GameWindow_MeasureText(GameWindowManager *manager, const u8 *string, const u32 *const *fonts, GameTextBounds *bounds, u8 compact) {
    GameText text;
    GameText_Init(&text, fonts, 0, string, 0, 0, compact == 0 ? 2 : 1, compact == 0 ? 3 : 1, 1, 6, 64, compact, 255, 0);
    GameText_MeasureBounds(&text, bounds);
}

#include <game/audio.h>
int GameWindow_Release(GameWindowManager *manager, s16 index) {
    GameWindow *window = &manager->windows[index];
    if (manager->state.bits.main_count + manager->state.bits.sub_count == 0) return 0;
    if (!window->front) return 0;
    window->front = 0;
    window->allocation.bits.closing = 1;
    if (window->state.bits.animation) {
        window->state.bits.animation = 0;
        if (window->properties.shape.bits.sound) {
            GameAudio_StopEffect((s16)window->properties.shape.bits.sound);
            GameAudio_StopEffect((s16)(window->properties.shape.bits.sound + 1));
        }
    }
    if (window->links[0].state.bits.linked) {
        window->links[0].state.bits.linked = 0;
        window->links[0].id = 0xffff;
        window->links[0].previous->next = window->links[0].next;
        window->links[0].next->previous = window->links[0].previous;
    }
    if (window->links[1].state.bits.linked) {
        window->links[1].state.bits.linked = 0;
        window->links[1].id = 0xffff;
        window->links[1].previous->next = window->links[1].next;
        window->links[1].next->previous = window->links[1].previous;
    }
    window->previous->next = window->next;
    window->next->previous = window->previous;
    if (!window->properties.shape.bits.screen) --manager->state.bits.main_count;
    else --manager->state.bits.sub_count;
    return 1;
}

extern void func_0202cbd4(void *, u8, u32);
extern void GameWindow_WriteTilemap(GameWindowManager *, GameWindow *);
void GameWindow_RebuildTilemap(GameWindowManager *manager, int screen) {
    GameWindow *window;
    if (!screen) {
        func_0202cbd4(manager->main_tilemap, 0, 1920);
        window = manager->main_windows;
    } else {
        func_0202cbd4(manager->sub_tilemap, 0, 1920);
        window = manager->sub_windows;
    }
    for (; window->next != (GameWindow *)-1; window = window->next) {
        if (!window->properties.shape.bits.position_mode) GameWindow_WriteTilemap(manager, window);
    }
    if (screen) manager->state.bits.sub_map_dirty = 1;
    else manager->state.bits.main_map_dirty = 1;
}

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

#include <game/window_operations.h>

extern void func_0201b4cc(GameWindowManager *, GameWindow *, const GameWindowSkin *, u8 *, int);
extern void func_0201c65c(GameWindowManager *, GameWindow *, const GameWindowSkin *, u8 *);

void GameWindow_DrawSkin(GameWindowManager *manager, GameWindow *window) {
    unsigned skin_index = window->properties.shape.bits.skin;
    const GameWindowSkin *skin = data_0205671c[skin_index];
    u8 *buffer = !manager->state.bits.buffer_mode ? window->back : window->front;
    if (!skin_index) {
        func_0202cbd4(buffer, 0, window->state.bits.size);
        if (!manager->state.bits.buffer_mode)
            func_0202cbd4(window->front, 0, window->state.bits.size);
    } else if (!window->properties.shape.bits.position_mode &&
               ((window->properties.position.bits.x & 7) ||
                (window->properties.position.bits.y & 7))) {
        func_0201b4cc(manager, window, skin, buffer, 0);
        if (!manager->state.bits.buffer_mode)
            func_0201b4cc(manager, window, skin, window->front, 1);
    } else {
        func_0201c65c(manager, window, skin, buffer);
        if (!manager->state.bits.buffer_mode)
            func_0201c65c(manager, window, skin, window->front);
    }
}
