#include <game/battle_actor.h>
#include <game/battle_scene.h>
#include <game/overlay017_attack.h>

#include "attack_state.h"

enum Overlay17AttackObjectConstant {
    OVERLAY17_PARTY_OBJECT_COUNT = 2,
    OVERLAY17_ATTACK_OBJECT_COUNT = 4,
    OVERLAY17_ANIMATION_CHANNEL = 2,
    OVERLAY17_REPLACEMENT_RESOURCE_FIRST = 57,
};

extern "C" {
extern u16 data_ov017_020c6c90[];
void func_ov002_0207179c(BattlePartyActor *actor,
                         BattleSceneObject *object);
void func_ov002_02071750(BattleSceneObject *object, int value);
int func_ov017_020c5f90(void *attack_state);
}

/* Metrowerks emits C functions in reverse source order. */
int Overlay17Attack_UpdateObjectAnimations(void) {
    int all_complete = 1;
    int object_index;

    for (object_index = 0;
         object_index < OVERLAY17_ATTACK_OBJECT_COUNT;
         ++object_index) {
        BattleSceneObject *object = BattleSceneObject_GetById(
            data_ov017_020c6c90[object_index]);

        if (BattleSceneObject_IsAnimationChannelActive(
                object, OVERLAY17_ANIMATION_CHANNEL)) {
            all_complete = 0;
        } else {
            BattleModel *model = BattleSceneObject_GetActiveModel(object);

            if ((((u32)(model->flags << 22) >> 31) & 0xFF) == 0) {
                int animation_id =
                    model->get_animation_id();

                model->set_animation(animation_id & 0xFF, 0);
                model->flags |= BATTLE_MODEL_FLAG_09;
            }
        }
    }
    return all_complete;
}

int Overlay17Attack_ConfigurePartyObjects(void) {
    Overlay17BattleStateView *battle_state = data_ov002_020c0710;
    int party_index = 0;

    do {
        BattleSceneObject *support_object = BattleSceneObject_GetById(
            data_ov017_020c6c90[party_index + OVERLAY17_PARTY_OBJECT_COUNT]);
        BattleSceneObject *party_object;
        BattlePartyActor *actor;
        BattleModel *model;

        BattleSceneObject_SetAnimation(support_object, -1, 0);
        party_object =
            BattleSceneObject_GetById(data_ov017_020c6c90[party_index]);
        actor = (BattlePartyActor *)BattleActor_GetPartySlot(
            data_ov017_020c6c90[party_index]);
        BattleSceneObject_AdjustPosition(
            party_object,
            actor->actor.unk_018 - party_object->x,
            actor->actor.unk_01a - party_object->y,
            -party_object->z);
        BattleEntity_BindResource(
            party_object->actor_id,
            (u16)(party_index + OVERLAY17_REPLACEMENT_RESOURCE_FIRST));
        BattleSceneObject_SetAnimationFromComponent(
            party_object, 1, 0,
            battle_state->party_animation_components[party_index]);
        model = BattleSceneObject_GetActiveModel(party_object);
        model->flags &= ~BATTLE_MODEL_FLAG_10;
        model->flags &= ~BATTLE_MODEL_FLAG_11;
        func_ov002_02071750(party_object, 0);
        ++party_index;
    } while (party_index < OVERLAY17_PARTY_OBJECT_COUNT);

    if (battle_state->active_attack_slot != -1) {
        return func_ov017_020c5f90(
            battle_state->attack_states[battle_state->active_attack_slot]);
    }
    return -1;
}

int Overlay17Attack_UpdatePartyObjectReadiness(void) {
    int ready_count = 0;
    int party_index;

    for (party_index = 0;
         party_index < OVERLAY17_PARTY_OBJECT_COUNT;
         ++party_index) {
        BattleSceneObject *object =
            BattleSceneObject_GetById(data_ov017_020c6c90[party_index]);

        if (object->resource->object_data_id ==
            party_index + OVERLAY17_REPLACEMENT_RESOURCE_FIRST) {
            BattleModel *model = BattleSceneObject_GetActiveModel(object);

            if (((u32)(model->flags << 22) >> 31) & 0xFF) {
                BattlePartyActor *actor = (BattlePartyActor *)
                    BattleActor_GetPartySlot(
                        data_ov017_020c6c90[party_index]);

                func_ov002_0207179c(actor, object);
                ++ready_count;
            }
        } else {
            ++ready_count;
        }
    }
    return ready_count == OVERLAY17_PARTY_OBJECT_COUNT;
}
