#include <game/battle_object.h>
#include <game/battle_actor.h>
#include <game/overlay026_attack.h>

#include "attack_state.h"

enum Overlay26ActionState {
    OVERLAY26_ACTION_STATE_FIRST = 1,
    OVERLAY26_ACTION_STATE_SECOND = 2,
    OVERLAY26_ACTION_STATE_THIRD = 3,
};

extern void func_02029bb0(void *allocation);
extern void func_ov002_0207179c(BattlePartyActor *actor,
                                BattleSceneObject *object);

/* Metrowerks emits C functions in reverse source order. */
int Overlay26Attack_IsResourceLoadPending(void) {
    if (gOverlay26AttackState->resource_id != 0) {
        return BattleObjectData_IsLoadPending(
            gOverlay26AttackState->resource_id);
    }
    return 0;
}

int Overlay26Attack_GetActionState(const void *action) {
    const Overlay26ActionStateView *view =
        (const Overlay26ActionStateView *)action;
    int state = view->state;

    switch (state) {
    case OVERLAY26_ACTION_STATE_FIRST:
    case OVERLAY26_ACTION_STATE_SECOND:
    case OVERLAY26_ACTION_STATE_THIRD:
        return state;
    default:
        return 0;
    }
}

int Overlay26Attack_GetTrackedObjectState(u16 object_id) {
    int state = gOverlay26AttackState
                    ->tracked_objects[
                        object_id - OVERLAY26_FIRST_TRACKED_OBJECT_ID]
                    .state;

    switch (state) {
    case OVERLAY26_ACTION_STATE_FIRST:
    case OVERLAY26_ACTION_STATE_SECOND:
    case OVERLAY26_ACTION_STATE_THIRD:
        return state;
    default:
        return 0;
    }
}

int Overlay26Attack_TrySetObjectAnimation(BattleSceneObject *object,
                                          int animation_id,
                                          int resource_id) {
    if (Overlay26Attack_GetTrackedObjectState(object->actor_id)) {
        return 1;
    }
    if (resource_id != -1) {
        BattleEntity_BindResource(object->actor_id, (u16)resource_id);
    }
    BattleSceneObject_SetAnimation(object, animation_id, -1);
    return 0;
}

int Overlay26Attack_IsStateIdle(void) {
    int task_index;

    if (gOverlay26AttackState->primary_async_task != 0) {
        return 0;
    }
    for (task_index = 0;
         task_index < OVERLAY26_ASYNC_TASK_COUNT;
         ++task_index) {
        if (gOverlay26AttackState->async_tasks[task_index] != 0) {
            return 0;
        }
    }
    if (gOverlay26AttackState->completion_link != 0 &&
        *gOverlay26AttackState->completion_link != 0) {
        return 0;
    }
    return 1;
}

void Overlay26Attack_DestroyState(void *owner, int restore_party_objects) {
    Overlay26AttackOwnerView *owner_view =
        (Overlay26AttackOwnerView *)owner;
    int object_index = 0;

    owner_view->attack_state = 0;
    if (restore_party_objects) {
        do {
            Overlay26TrackedObjectState *tracked =
                &gOverlay26AttackState->tracked_objects[object_index];

            if (tracked->flag_bits.restore_actor) {
                u16 object_id = tracked->object_id;
                BattleSceneObject *object =
                    BattleSceneObject_GetById(object_id);

                object->effect_anchor_z = 0;
                object = BattleSceneObject_GetById(object_id);
                if ((((u32)(BattleSceneObject_GetActiveModel(object)->flags
                              << 23) >> 31) & 0xFF) != 0 &&
                    BattleActor_CanReceiveStatus(
                        BattleActor_GetById(object_id)) &&
                    !Overlay26Attack_GetTrackedObjectState(object_id)) {
                    BattlePartyActor *actor =
                        (BattlePartyActor *)BattleActor_GetPartySlot(
                            object_id);

                    object = BattleSceneObject_GetById(object_id);
                    func_ov002_0207179c(actor, object);
                }
            }
            ++object_index;
        } while (object_index < OVERLAY26_TRACKED_OBJECT_COUNT);
    }

    if (gOverlay26AttackState != 0) {
        func_02029bb0(gOverlay26AttackState);
        gOverlay26AttackState = 0;
    }
}
