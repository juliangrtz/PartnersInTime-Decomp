#include <game/sprite_window.h>

void GameSpriteWindow_Close(GameSpriteWindowManager *manager, s16 index) {
    GameWindow *window = &manager->base.windows[index];
    GameSpriteWindow_Slot(manager, window)->active = 0;
    window->properties.shape.bits.position_mode = 0;
    if (GameSpriteWindow_Slot(manager, window)->entry.flags.bits.linked) {
        GameSpriteWindowSlot *slot = GameSpriteWindow_Slot(manager, window);
        slot->entry.flags.bits.linked = 0;
        slot->entry.affine = 0xffff;
        slot->entry.previous->next = slot->entry.next;
        slot->entry.next->previous = slot->entry.previous;
    }
    GameSpriteAllocation_Unlink(&GameSpriteWindow_Slot(manager, window)->allocation);
}
