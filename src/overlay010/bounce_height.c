#include <nitro/fx.h>
#include <game/battle_scene.h>
#include <game/battle_actor.h>
#include <game/battle_enemy_data.h>
#include <game/overlay010_enemy_state.h>
extern s32 _s32_div_f(s32, s32);

int Overlay10Motion_ScaleHeight(BattleSceneObject *object, int height, int scale)
{
    int z = object->z + object->property_0fa;
    int adjustment = _s32_div_f(z * z, scale);
    return _s32_div_f(height * height, height + adjustment);
}

void Overlay10Motion_AdjustBounce(void *unused, BattleSceneObject *object, int *velocity, int *height)
{
    int z = object->z + object->property_0fa;
    int previous_velocity = *velocity, previous_height = *height;
    int acceleration;
    *height = Overlay10Motion_ScaleHeight(object, *height, 64);
    acceleration = _s32_div_f(-previous_velocity * previous_velocity, previous_height << 9);
    *velocity = FX_Sqrt(-32 * acceleration * (z + *height)) / 16;
    *height += z;
}
