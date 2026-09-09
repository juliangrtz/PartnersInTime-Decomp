#include <nitro/fx.h>
#include <game/battle_scene.h>
#include <game/battle_actor.h>
#include <game/battle_enemy_data.h>
#include <game/overlay010_enemy_state.h>
extern int BattleActor_IsDamageImmune(int), BattleActor_HasTrait1(u16);

void Overlay10Enemy_AddScaleSteps(Overlay10EnemyState *state, int actor_id, int amount)
{
    u32 total = state->scale_steps[actor_id - 60];
    total += amount;
    if (total > 65535)
        state->scale_steps[actor_id - 60] = 65535;
    else
        state->scale_steps[actor_id - 60] += amount;
}

void Overlay10Enemy_ApplyProjectedDamage(Overlay10EnemyState *state, int actor_id, int damage)
{
    if (!BattleActor_IsDamageImmune(actor_id)) {
        int hp = BattleActor_GetById(actor_id)->max_hp;
        int remaining = state->current_hp[actor_id - 60] - damage;
        if (remaining < -10000)
            remaining = -10000;
        if (remaining > hp)
            remaining = hp;
        state->current_hp[actor_id - 60] = remaining;
    }
}

int Overlay10Motion_DistanceXYZ(BattleSceneObject *first, BattleSceneObject *second, const s16 *first_offset,
                                const s16 *second_offset)
{
    int x1 = first->x, y1 = first->y, z1 = first->z, x2 = second->x, y2 = second->y, z2 = second->z;
    if (first_offset) {
        x1 += first_offset[0];
        y1 += first_offset[1];
        z1 += first_offset[2];
    }
    if (second_offset) {
        x2 += second_offset[0];
        y2 += second_offset[1];
        z2 += second_offset[2];
    }
    return FX_Sqrt(((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2)) << 12) / 16;
}

int Overlay10Motion_DistanceXY(BattleSceneObject *first, BattleSceneObject *second, const s16 *first_offset,
                               const s16 *second_offset)
{
    int x1 = first->x, y1 = first->y, x2 = second->x, y2 = second->y;
    if (first_offset) {
        x1 += first_offset[0];
        y1 += first_offset[1];
    }
    if (second_offset) {
        x2 += second_offset[0];
        y2 += second_offset[1];
    }
    return FX_Sqrt(((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) << 12) / 16;
}
