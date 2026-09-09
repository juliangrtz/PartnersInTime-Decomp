extern "C" {
#include <game/battle_window.h>
}
extern "C" {
extern void *data_ov002_020be8e0[];
extern u8 *gBattleSystem;
void func_0202cbd4(void *, int, u32);
void func_ov002_0206fc7c(BattleWindowManager *);
void func_ov002_0206fb64(BattleWindowManager *);
}

static inline u8 SubTileCount(const GameSpriteAnimationFile *file)
{
    u8 count = 0;
    u16 boundary = GameSprite_ObjBoundaryShift(1);
    switch (boundary) {
    case 0:
        count = file->tile_counts.count_32;
        break;
    case 1:
        count = file->tile_counts.count_64;
        break;
    case 2:
        count = file->tile_counts.count_128;
        break;
    }
    return count;
}

BattleWindowManager *BattleWindow_Construct(BattleWindowManager *manager, int priority, int unused,
                                            GameSpriteAnimator *animator, u32 texture_offset,
                                            const GameWindowBuffers *buffers,
                                            const GameWindowTilemaps *tilemaps, int configure)
{
    GameWindow_ConstructBase(&manager->base, priority, unused, 0, 0, 0, 0, buffers, tilemaps, configure,
                             animator);
    manager->base.vtable = data_ov002_020be8e0;
    func_0202cbd4(&manager->unknown_f44, 0, (u8 *)(manager + 1) - (u8 *)&manager->unknown_f44);
    manager->base.state.bits.enabled = 0;
    GameSpriteAnimator *main_sprite = manager->base.animator;
    main_sprite->oam[0] = manager->oam[0];
    main_sprite->reverse = 0;
    GameSpriteAnimator *sub_sprite = manager->base.animator;
    sub_sprite->oam[1] = manager->oam[1];
    sub_sprite->reverse = 0;
    manager->base.display.bits.sub_palette = 15;
    manager->base.display.bits.main_palette = 15;
    GameSpriteAnimator *sprite = manager->base.animator;
    GameSpriteAllocation_Allocate(&sprite->allocation[1], 1, 1, SubTileCount(sprite->file), 0, 0xb8e1, 1, 0);
    sprite->state.bits.sub_allocated = 1;
    GameSpriteAnimator_LoadSub(manager->base.animator);
    BattleWindow_AllocateTexturePalettes(manager);
    BattleWindow_AllocateSpritePalette(manager);
    manager->texture_offset = texture_offset;
    ((BattleWindowAnimator *)manager->base.animator)->texture_offset = texture_offset + 0x6000;
    manager->texture_banks = *(const u32 **)(gBattleSystem + 3576);
    GameTask_Disable((GameTask *)manager);
    GameIrqTask_Disable(manager->base.irq_task);
    return manager;
}

static inline void DestroyResources(BattleWindowManager *manager)
{
    manager->base.vtable = data_ov002_020be8e0;
    GameIrqTask_Disable(manager->base.irq_task);
    GameTexturePalette_Unlink(&manager->palettes[0]);
    GameTexturePalette_Unlink(&manager->palettes[1]);
    GameSpritePalette_Unlink(&manager->sprite_palette);
    GameSpriteAnimator *sprite = manager->base.animator;
    GameSpriteAllocation_Unlink(&sprite->allocation[1]);
    sprite->state.bits.sub_allocated = 0;
    GameWindow_DestroyBase(&manager->base);
}

BattleWindowManager *BattleWindow_Destroy(BattleWindowManager *manager)
{
    DestroyResources(manager);
    return manager;
}

BattleWindowManager *BattleWindow_Delete(BattleWindowManager *manager)
{
    DestroyResources(manager);
    GameHeap_Delete(manager);
    return manager;
}

void BattleWindow_Update(BattleWindowManager *manager)
{
    func_0202cbd4(manager->oam, 0, sizeof(manager->oam));
    u32 palette = manager->palettes[1].offset;
    BattleWindowAnimator *animator = (BattleWindowAnimator *)manager->base.animator;
    animator->texture_cursor = 0;
    animator->palette_offset = palette;
    GameWindow_Update(&manager->base);
    func_ov002_0206fc7c(manager);
    func_ov002_0206fb64(manager);
}
