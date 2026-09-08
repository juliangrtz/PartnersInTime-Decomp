#include <nitro/fx.h>
#include <game/battle_scene.h>
#include <game/battle_actor.h>
#include <game/battle_enemy_data.h>
#include <game/overlay010_enemy_state.h>
extern int Overlay10Enemy_IsSelectable(u16);
extern int func_ov002_02071674(int), func_ov002_020718a0(u16);
extern int Random_NextModulo(int);

int Overlay10Enemy_SelectProjectedSurvivor(Overlay10EnemyState *state)
{
    u16 ids[6];
    int count = 0, i;
    for (i = 0; i < 6; ++i)
        if (state->current_hp[i] > 0)
            ids[count++] = i + 60;
    return count ? ids[Random_NextModulo(count)] : 0;
}

int Overlay10Enemy_SelectReactionTarget(void)
{
    u16 ids[8];
    int count = 0, i;
    for (i = 0; i < 6; ++i) {
        if (Overlay10Enemy_IsSelectable((u16)(i + 60))) {
            BattleEnemy_GetStats((u16)(i + 60));
            if (!func_ov002_020718a0((u16)(i + 60)))
                ids[count++] = i + 60;
        }
    }
    if (count)
        return ids[Random_NextModulo(count)];
    for (i = 0; i < 6; ++i)
        if (Overlay10Enemy_IsSelectable((u16)(i + 60)))
            ids[count++] = i + 60;
    return count ? ids[Random_NextModulo(count)] : 0;
}
