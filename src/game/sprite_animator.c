#include <game/sprite_animator.h>

extern void (*data_02056614[])(GameSpriteAnimator *, GameSpriteAnimation *);
extern GameOamEntry data_02060340[128], data_02060740[128];
extern u32 data_02056718, data_02056714;
extern u8 data_02057afc[], data_0205680c[], data_02056628[], data_0205663c[];
extern void func_0202cbd4(void *destination, int value, u32 size);
extern void func_0203b7b4(const void *source, void *destination, u32 size);

void GameSpriteAnimator_LoadBoth(GameSpriteAnimator *animator)
{
    GameSpriteAnimator_LoadMain(animator);
    GameSpriteAnimator_LoadSub(animator);
}

void GameSpriteAnimator_LoadMain(GameSpriteAnimator *animator)
{
    GameSpriteAnimator_UploadTiles(animator, 0, &animator->allocation[0], animator->tiles,
        animator->tile_size, animator->file, animator->main_objects);
    GameSpriteAnimator_RemapTiles(animator, 0);
    animator->state.bits.main_loaded = 1;
}

void GameSpriteAnimator_LoadSub(GameSpriteAnimator *animator)
{
    GameSpriteAnimator_UploadTiles(animator, 1, &animator->allocation[1], animator->tiles,
        animator->tile_size, animator->file, animator->sub_objects);
    GameSpriteAnimator_RemapTiles(animator, 1);
    animator->state.bits.sub_loaded = 1;
}

static inline GameSpriteAnimator *InitializeAnimator(GameSpriteAnimator *animator)
{
    u32 size;
    GameSpriteAnimationFile *file;
    u32 object_size;
    animator->vtable = data_02056614;
    animator->state.bits.main_allocated = 0;
    animator->state.bits.sub_allocated = 0;
    animator->state.bits.main_loaded = 0;
    animator->state.bits.sub_loaded = 0;
    animator->oam[0] = data_02060340;
    animator->oam[1] = data_02060740;
    animator->reverse = 1;
    func_0202cbd4(&animator->allocation[0], 0, 24);
    func_0202cbd4(&animator->allocation[1], 0, 24);
    animator->list.head_marker = (GameSpriteAnimation *)0x11111111;
    animator->list.first = (GameSpriteAnimation *)&animator->list.last;
    animator->list.last = (GameSpriteAnimation *)&animator->list.head_marker;
    animator->list.tail_marker = (GameSpriteAnimation *)-1;
    size = data_02056718;
    animator->tiles = data_02057afc;
    animator->tile_size = data_02056714;
    animator->file = GameHeap_NewArray(size, 0, data_02056628, 0);
    func_0203b7b4(data_0205680c, animator->file, size);
    file = animator->file;
    object_size = size - (20 + 4 * file->animation_count + 8 * file->frame_count + 4 * file->group_count);
    animator->main_objects = (u8 *)file + 20 + 4 * file->animation_count + 8 * file->frame_count + 4 * file->group_count;
    animator->sub_objects = GameHeap_NewArray(object_size, 0, data_0205663c, 0);
    func_0203b7b4(animator->main_objects, animator->sub_objects, object_size);
    animator->frames = (GameSpriteAnimationFrame *)(animator->file->animations + animator->file->animation_count);
    return animator;
}

GameSpriteAnimator *GameSpriteAnimator_Init(GameSpriteAnimator *animator)
{
    return InitializeAnimator(animator);
}

GameSpriteAnimator *GameSpriteAnimator_Construct(GameSpriteAnimator *animator)
{
    return InitializeAnimator(animator);
}

GameSpriteAnimator *GameSpriteAnimator_Destroy(GameSpriteAnimator *animator)
{
    animator->vtable = data_02056614;
    GameHeap_DeleteArray(animator->file);
    GameHeap_DeleteArray(animator->sub_objects);
    return animator;
}
