#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_effect.h>
#include <game/battle_global_properties.h>
#include <game/battle_scene.h>

enum BattlePartyIndicatorAnimation {
    BATTLE_PARTY_INDICATOR_ADULT_FRONT = 12,
    BATTLE_PARTY_INDICATOR_ADULT_BACK = 13,
    BATTLE_PARTY_INDICATOR_BABY_FRONT = 14,
    BATTLE_PARTY_INDICATOR_BABY_BACK = 15,
    BATTLE_PARTY_INDICATOR_BABY_GROUP_A = 18,
    BATTLE_PARTY_INDICATOR_BABY_GROUP_B = 19
};

typedef struct BattlePartyIndicatorState {
    BattleSceneObject *object;
    s16 alpha;
    s16 enabled;
    u16 actor_id;
    u16 previous_party_group;
} BattlePartyIndicatorState;

void BattlePartyIndicator_Update(BattlePartyIndicatorState *state) {
    BattleSceneObject *object = state->object;
    BattlePartyActor *actor;

    if (object == 0) {
        return;
    }
    if (state->actor_id < BATTLE_ACTOR_PARTY_FIRST) {
        return;
    }

    actor = (BattlePartyActor *)BattleActor_GetPartySlot(state->actor_id);
    if (state->enabled == 0 ||
        *(s16 *)(gBattleContext + BATTLE_GLOBAL_12_OFFSET) == -1 ||
        actor->actor.current_hp <= 0 ||
        actor->actor.transition_state > 0 ||
        actor->state_flags.bits.flag_12 != 0) {
        if (state->alpha == 0) {
            return;
        }
        if (state->alpha > 0) {
            state->alpha -= 2;
            if (state->alpha <= 0) {
                state->alpha = 0;
                BattleSceneObject_SetAnimation(object, -1, -1);
                return;
            }
        }
    } else if (state->alpha < 31) {
        if (state->alpha == 0) {
            state->previous_party_group = (u16)-2;
        }
        state->alpha = 31;
    }

    {
        int party_group =
            *(s16 *)(gBattleContext + BATTLE_GLOBAL_12_OFFSET);

        if (state->previous_party_group != party_group &&
            party_group != -1) {
            int animation = 0;

            state->previous_party_group = party_group;
            if (state->previous_party_group == 0) {
                BattleModelEffect_Spawn(46, object, 0, 0, 0, 256);
                switch (((BattlePartyActor *)BattleActor_GetPartySlot(
                             state->actor_id))->formation_index) {
                case 0:
                case 4:
                    animation = BATTLE_PARTY_INDICATOR_ADULT_FRONT;
                    break;
                case 2:
                    animation = BATTLE_PARTY_INDICATOR_BABY_FRONT;
                    break;
                case 1:
                case 5:
                    animation = BATTLE_PARTY_INDICATOR_ADULT_BACK;
                    break;
                case 3:
                    animation = BATTLE_PARTY_INDICATOR_BABY_BACK;
                    break;
                }
            }
            if (state->previous_party_group == 1) {
                BattleModelEffect_Spawn(46, object, 0, 0, 0, 256);
                switch (((BattlePartyActor *)BattleActor_GetPartySlot(
                             state->actor_id))->formation_index) {
                case 0:
                case 2:
                case 4:
                    animation = BATTLE_PARTY_INDICATOR_BABY_GROUP_A;
                    break;
                case 1:
                case 3:
                case 5:
                    animation = BATTLE_PARTY_INDICATOR_BABY_GROUP_B;
                    break;
                }
            }
            BattleSceneObject_SetAnimation(object, animation, -1);
        }
    }

    BattleModel_SetAlpha(object->primary_model, (u8)state->alpha, 0);
}
