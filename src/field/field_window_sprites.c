#include <game/field_area.h>
#include <game/window.h>

static inline u8 GetTileCount(const GameSpriteAnimationFile *file, int screen)
{
    switch (GameSprite_ObjBoundaryShift(screen)) {
    case 0:
        return file->tile_counts.count_32;
    case 1:
        return file->tile_counts.count_64;
    case 2:
        return file->tile_counts.count_128;
    default:
        return 0;
    }
}
void FieldArea_LoadWindowSprites(FieldAreaContext *area)
{
    GameWindowManager *windows;
    GameSpriteAnimator *animator;
    if (!area->flags.screen) {
        animator = ((GameWindowManager *)area->owner)->animator;
        GameSpriteAllocation_Allocate(&animator->allocation[0], 0, 0, GetTileCount(animator->file, 0), 0,
                                      0xb8e1, 1, 0);
        animator->state.raw |= 0x10000;
        GameSpriteAnimator_LoadMain(((GameWindowManager *)area->owner)->animator);
    } else {
        animator = ((GameWindowManager *)area->owner)->animator;
        GameSpriteAllocation_Allocate(&animator->allocation[1], 1, 1, GetTileCount(animator->file, 1), 0,
                                      0xb8e1, 1, 0);
        animator->state.raw |= 0x20000;
        GameSpriteAnimator_LoadSub(((GameWindowManager *)area->owner)->animator);
    }
    windows = (GameWindowManager *)area->owner;
    windows->display.bits.sub_palette = 15;
    windows->display.bits.main_palette = 15;
    GameWindow_SetOrigin((GameWindowManager *)area->owner, (u8)area->flags.screen, 0, 0);
}
