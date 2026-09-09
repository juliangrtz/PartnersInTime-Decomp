extern "C" {
#include <game/battle_window.h>
}
extern "C" {
extern u32 data_0205670c, data_02056710;
extern u8 data_0205676c[];
void func_02007ebc(GameSpritePalette *, int, int, int, int, int, int, int, const void *, u32);
}

void BattleWindow_AllocateTexturePalettes(BattleWindowManager *manager)
{
    GameTexturePalette_Allocate(&manager->palettes[0], 0, 3, (u16)(data_0205670c >> 1), 0, 1, 0,
                                manager->base.palette_data, 0xc2fffffe, 0);
    GameTexturePalette_Allocate(&manager->palettes[1], 0, 3, (u16)(data_02056710 >> 1), 0, 1, 0,
                                data_0205676c, 0xc2fffffd, 0);
}

void BattleWindow_AllocateSpritePalette(BattleWindowManager *manager)
{
    func_02007ebc(&manager->sprite_palette, 1, 0, 15, 0, (u8)(data_02056710 >> 5), 0, 1, data_0205676c,
                  0xc2fffffd);
}

void BattleWindow_AllocatePool(BattleWindowManager *manager, int heap)
{
    GameWindow_AllocatePool(&manager->base, heap, 4, 1);
}
