#include <game/window.h>
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
