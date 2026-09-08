#include <game/battle_effect.h>
#include <game/battle_ai.h>
#include <game/text.h>

void BattlePosition_StoreViewRelative(BattlePosition *position, int x, int y, int z, int raw, int view)
{
    if (raw == 1) {
        position->x = x;
        position->y = y;
    } else if (view == 0) {
        position->x = x - *(s16 *)(gBattleContext + 0xcb9c);
        position->y = y - *(s16 *)(gBattleContext + 0xcb9e);
    } else {
        position->x = x - *(s16 *)(gBattleContext + 0xcba0);
        position->y = y - *(s16 *)(gBattleContext + 0xcba2);
    }
    if (z < 0)
        z = 0;
    position->z = z;
}

void *BattleText_GetEntry(int table, u16 entry)
{
    return (void *)GameTextResources_GetEntry(table, entry);
}

void func_ov002_02076ac8(void)
{
    *(u16 *)(gBattleContext + 26466) = 1;
    BattleEntity_BindResource((*(BattleSceneObject **)(gBattleContext + 26492))->actor_id, 54);
}
