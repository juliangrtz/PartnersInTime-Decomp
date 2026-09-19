#include <game/battle_actor.h>
#include <game/battle_enemy_data.h>
#include <game/battle_scene.h>
#include <game/overlay010_enemy_state.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

int Overlay10Enemy_IsSelectable(u16 actor_id);
int Overlay10Enemy_CollectIds(u16 *actor_ids, int trait_filter);

/* Metrowerks emits C functions in reverse source order. */
int Overlay10_SelectThresholdValue(const int *values, int input,
                                   const s16 *thresholds) {
    int result = values[0];
    int i;

    for (i = 0; i < 2; ++i) {
        if (input < thresholds[i]) {
            return result;
        }
        result = values[i];
    }
    return result;
}

int Overlay10Enemy_CalculateScaledValue(Overlay10EnemyState *state,
                                        int actor_id, int scale,
                                        int maximum) {
    int maximum_step = _s32_div_f(maximum << 8, scale) + 1;
    int step = state->scale_steps[actor_id - BATTLE_ACTOR_ENEMY_FIRST];
    int result;

    if (step > maximum_step) {
        step = maximum_step;
    }
    result = (scale * step) >> 8;
    if (result > maximum) {
        result = maximum;
    }
    return result;
}

int Overlay10Enemy_CollectIds(u16 *actor_ids, int trait_filter) {
    int count = 0;
    int i;

    for (i = 0; i < OVERLAY10_ENEMY_COUNT; ++i) {
        u16 actor_id = (u16)(i + BATTLE_ACTOR_ENEMY_FIRST);
        BattleEnemyStatRecord *stats;

        BattleActor_GetById(actor_id);
        if (!Overlay10Enemy_IsSelectable(actor_id)) {
            continue;
        }
        stats = BattleEnemy_GetStats(actor_id);
        if (trait_filter == 1) {
            if (stats->trait_bits.unknown_trait_02) {
                continue;
            }
        } else if (trait_filter == 2 && !stats->trait_bits.unknown_trait_02) {
            continue;
        }
        if (actor_ids != 0) {
            actor_ids[count] = actor_id;
        }
        ++count;
    }
    return count;
}

int Overlay10Enemy_GetFirstId(int trait_filter) {
    u16 actor_ids[8];
    int count = Overlay10Enemy_CollectIds(actor_ids, trait_filter);

    if (trait_filter != 0 && count == 0) {
        count = Overlay10Enemy_CollectIds(actor_ids, 0);
    }
    if (count == 0) {
        return 0;
    }
    return actor_ids[0];
}

int Overlay10Enemy_Snapshot(Overlay10EnemyState *state) {
    int selectable_count = 0;
    int i;

    for (i = 0; i < OVERLAY10_ENEMY_COUNT; ++i) {
        BattleActor *actor = BattleActor_GetById(
            (u16)(i + BATTLE_ACTOR_ENEMY_FIRST));

        if (Overlay10Enemy_IsSelectable(i + BATTLE_ACTOR_ENEMY_FIRST)) {
            ++selectable_count;
            state->current_hp[i] = actor->current_hp;
        } else {
            state->current_hp[i] = 0;
        }
        state->position_z[i] = actor->scene_object->z;
    }
    return selectable_count;
}

int Overlay10Enemy_IsSelectable(u16 actor_id) {
    BattleActor *actor = BattleActor_GetById(actor_id);

    return BattleActor_CanReceiveStatus(BattleActor_GetById(actor_id)) &&
           !actor->flag_bits.excluded_from_targeting;
}
