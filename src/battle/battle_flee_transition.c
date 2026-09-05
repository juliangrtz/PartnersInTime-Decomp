#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

enum BattleFleeConstant {
    BATTLE_FLEE_RUN_CHANNEL = 1,
    BATTLE_FLEE_OFFSCREEN_X = -32,
    BATTLE_FLEE_RUN_ANIMATION = 2,
    BATTLE_FLEE_RUN_SOUND_ID = 61
};

typedef union BattleFleeFlags {
    u16 raw;
    struct {
        u16 unknown_00_13 : 14;
        u16 ready_for_partner : 1;
        u16 unknown_15 : 1;
    } bits;
} BattleFleeFlags;

typedef struct BattleFleeTask BattleFleeTask;

typedef struct BattleFleeState {
    BattleFleeTask *partner_task;
    u16 object_id;
    u8 unknown_06[8];
    BattleFleeFlags flags;
} BattleFleeState;

struct BattleFleeTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleFleeState data;
};

typedef char BattleFleeState_SizeCheck[
    sizeof(BattleFleeState) == 0x10 ? 1 : -1];

void BattleFlee_BeginRun(BattleAITask *task);
void BattleFlee_WaitForExitAnimation(BattleAITask *task);
void BattleFlee_MoveActorOffscreen(BattleAITask *task);
void BattleFlee_WaitForPartnerExit(BattleAITask *task);

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
void BattleFlee_PreparePartnerExit(BattleAITask *base_task) {
    BattleFleeTask *task = (BattleFleeTask *)base_task;
    BattleFleeState *state = &task->data;
    BattleFleeState *partner_state =
        state->partner_task != 0 ? &state->partner_task->data : 0;
    BattleSceneObject *object = BattleSceneObject_GetById(state->object_id);

    if (partner_state != 0 && partner_state->flags.bits.ready_for_partner) {
        BattleModel *model = object->primary_model;

        if (!model->flag_bits.panel_animation_trigger) {
            return;
        }
        model->flags &= ~BATTLE_MODEL_ANIMATION_MODE_MASK;
        object->primary_model->flags |= BATTLE_MODEL_FLAG_09;
    }
    task->callback = BattleFlee_WaitForPartnerExit;
}

void BattleFlee_WaitForPartnerExit(BattleAITask *base_task) {
    BattleFleeTask *task = (BattleFleeTask *)base_task;
    BattleFleeState *state = &task->data;
    BattleFleeState *partner_state =
        state->partner_task != 0 ? &state->partner_task->data : 0;
    BattleSceneObject *object = BattleSceneObject_GetById(state->object_id);

    if (partner_state == 0 || !partner_state->flags.bits.ready_for_partner) {
        BattleSceneObject_SetAnimation(
            object, BATTLE_FLEE_RUN_ANIMATION, -1);
        state->flags.bits.ready_for_partner = 1;
        task->callback = BattleFlee_BeginRun;
    }
}

void BattleFlee_MoveActorOffscreen(BattleAITask *base_task) {
    BattleFleeTask *task = (BattleFleeTask *)base_task;
    BattleFleeState *state = &task->data;
    BattleSceneObject *object = BattleSceneObject_GetById(state->object_id);
    int delta_x = BATTLE_FLEE_OFFSCREEN_X - object->x;

    BattleSceneObject_MoveBy(
        object, BATTLE_FLEE_RUN_CHANNEL, delta_x, 0, 0, -delta_x);
    state->flags.bits.ready_for_partner = 0;
    if (state->partner_task != 0) {
        state->partner_task->data.partner_task = 0;
    }

    switch (((BattlePartyActor *)BattleActor_GetPartySlot(object->actor_id))
                ->formation_index) {
        case BATTLE_FORMATION_MARIO:
        case BATTLE_FORMATION_MARIO_CARRYING:
            BattleSound_Play(BATTLE_FLEE_RUN_SOUND_ID, 0, 0, 0);
            break;
        case BATTLE_FORMATION_LUIGI:
        case BATTLE_FORMATION_LUIGI_CARRYING:
            BattleSound_Play(BATTLE_FLEE_RUN_SOUND_ID, 0, 0, 0);
            break;
        case BATTLE_FORMATION_BABY_MARIO:
            BattleSound_Play(BATTLE_FLEE_RUN_SOUND_ID, 0, 0, 0);
            break;
        case BATTLE_FORMATION_BABY_LUIGI:
            BattleSound_Play(BATTLE_FLEE_RUN_SOUND_ID, 0, 0, 0);
            break;
        default:
            break;
    }
    task->callback = BattleFlee_WaitForExitAnimation;
}

void BattleFlee_WaitForExitAnimation(BattleAITask *base_task) {
    BattleFleeTask *task = (BattleFleeTask *)base_task;

    if (!BattleSceneObject_IsAnimationActiveById(task->data.object_id, 1)) {
        task->callback = 0;
    }
}
