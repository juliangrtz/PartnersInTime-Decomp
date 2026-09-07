#include <game/sprite_window.h>

extern void DC_FlushRange(const void *address, u32 size);
extern void GameWindow_Upload(GameWindowManager *manager, GameWindow *window);

GameOamObject *GameSpriteWindow_ApplyTileOffsets(GameSpriteWindowManager *manager,
                                                 GameWindow *window) {
    GameSpriteWindowSlot *slot = GameSpriteWindow_Slot(manager, window);
    GameOamObject *object = (GameOamObject *)slot->entry.data.direct.entries;
    u16 first = GameSprite_TileIndex(window->properties.shape.bits.screen,
                                     GameSprite_ObjAddress(&slot->allocation));
    s16 remaining = slot->entry.data.direct.count;
    while (remaining > 0) {
        object->tile.bits.tile = first + object->tile.bits.tile;
        --remaining;
        ++object;
    }
    return object;
}

void GameSpriteWindow_UploadDirty(GameSpriteWindowManager *manager) {
    GameWindow *window;
    manager->base.state.bits.dirty = 0;
    for (window = manager->base.main_windows; window->next != (GameWindow *)-1;
         window = window->next) {
        if (window->allocation.bits.dirty) {
            window->allocation.bits.dirty = 0;
            switch (window->properties.shape.bits.position_mode) {
            case 0:
                DC_FlushRange(window->front, window->state.bits.size);
                GameWindow_Upload(&manager->base, window);
                break;
            case 1:
                GameSpriteWindow_Upload(manager, window);
                break;
            }
        }
    }
    for (window = manager->base.sub_windows; window->next != (GameWindow *)-1;
         window = window->next) {
        if (window->allocation.bits.dirty) {
            window->allocation.bits.dirty = 0;
            switch (window->properties.shape.bits.position_mode) {
            case 0:
                DC_FlushRange(window->front, window->state.bits.size);
                GameWindow_Upload(&manager->base, window);
                break;
            case 1:
                GameSpriteWindow_Upload(manager, window);
                break;
            }
        }
    }
}
