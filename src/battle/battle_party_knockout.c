#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_party.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>

enum BattlePartyKnockoutConstant {
    BATTLE_PARTY_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_PARTY_RELOAD_ACTIVE_FLAG = 1 << 2,
    BATTLE_PARTY_MARIO_RELOAD_STATE_OFFSET = 0x657E,
    BATTLE_PARTY_LUIGI_RELOAD_STATE_OFFSET = 0x659A,
    BATTLE_PARTY_MARIO_REBOUND_OBJECT_DATA_ID = 5,
    BATTLE_PARTY_LUIGI_REBOUND_OBJECT_DATA_ID = 6,
    BATTLE_PARTY_MARIO_REBOUND_RESOURCE_ID = 0xC000003B,
    BATTLE_PARTY_LUIGI_REBOUND_RESOURCE_ID = 0xC000003C
};

enum BattlePartyKnockoutAnimation {
    BATTLE_PARTY_KNOCKOUT_ANIMATION = 13,
    BATTLE_PARTY_LINKED_KNOCKOUT_ANIMATION = 14
};

enum BattlePartyKnockoutSound {
    BATTLE_SOUND_PARTY_KNOCKOUT = 62,
    BATTLE_SOUND_MARIO_KNOCKOUT = 0x4023,
    BATTLE_SOUND_LUIGI_KNOCKOUT = 0x404B,
    BATTLE_SOUND_BABY_MARIO_KNOCKOUT = 0x4060,
    BATTLE_SOUND_BABY_LUIGI_KNOCKOUT = 0x4073
};

typedef struct BattlePartyKnockoutPayload {
    BattleAIState *unused_state;
    u16 actor_id;
    u16 padding;
} BattlePartyKnockoutPayload;

typedef char BattlePartyKnockoutPayload_SizeCheck
    [sizeof(BattlePartyKnockoutPayload) == 8 ? 1 : -1];

BattleAITask *BattleParty_StartKnockout(BattleSceneObject *object) {
    BattleAITask *task = BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext + BATTLE_PARTY_TASK_POOL_OFFSET), 0);
    BattlePartyActor *actor =
        (BattlePartyActor *)BattleActor_GetPartySlot(object->actor_id);

    BattleStatus_ClearAll(&actor->actor);
    BattleSceneObject_SetAnimation(object, BATTLE_PARTY_KNOCKOUT_ANIMATION, -1);
    BattleSceneObject_SetStateFlags(object, 1, 0);
    task->id = object->actor_id;
    actor->actor.flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_RUNTIME_FLAG_PARTY_KNOCKOUT;
    actor->actor.flags &= ~BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
    task->callback = BattleParty_UpdateKnockout;

    switch (actor->formation_index) {
    case BATTLE_FORMATION_MARIO:
        BattleSound_Play(BATTLE_SOUND_PARTY_KNOCKOUT, 12, 0, 0);
        BattleSound_Play(BATTLE_SOUND_MARIO_KNOCKOUT, 0, 0, 0);
        break;
    case BATTLE_FORMATION_LUIGI:
        BattleSound_Play(BATTLE_SOUND_PARTY_KNOCKOUT, 35, 0, 0);
        BattleSound_Play(BATTLE_SOUND_LUIGI_KNOCKOUT, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_MARIO:
        BattleSound_Play(BATTLE_SOUND_PARTY_KNOCKOUT, 13, 0, 0);
        BattleSound_Play(BATTLE_SOUND_BABY_MARIO_KNOCKOUT, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_LUIGI:
        BattleSound_Play(BATTLE_SOUND_PARTY_KNOCKOUT, 40, 0, 0);
        BattleSound_Play(BATTLE_SOUND_BABY_LUIGI_KNOCKOUT, 0, 0, 0);
        break;
    case BATTLE_FORMATION_MARIO_CARRYING:
        BattleSound_Play(BATTLE_SOUND_PARTY_KNOCKOUT, 10, 0, 0);
        BattleSound_Play(BATTLE_SOUND_MARIO_KNOCKOUT, 0, 0, 0);
        break;
    case BATTLE_FORMATION_LUIGI_CARRYING:
        BattleSound_Play(BATTLE_SOUND_PARTY_KNOCKOUT, 27, 0, 0);
        BattleSound_Play(BATTLE_SOUND_LUIGI_KNOCKOUT, 0, 0, 0);
        break;
    }

    BattleScreenEffect_StartPrimary(3, 10, 12);
    return task;
}

void BattleParty_UpdateKnockout(BattleAITask *task) {
    BattlePartyKnockoutPayload *payload =
        (BattlePartyKnockoutPayload *)&task->state;
    BattlePartyActor *actor;
    BattleModel *model;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    actor = (BattlePartyActor *)BattleActor_GetPartySlot(payload->actor_id);
    model = actor->actor.scene_object->primary_model;
    if (model != 0 && model->flag_bits.animation_active &&
        !model->flag_bits.panel_animation_trigger) {
        return;
    }

    if (actor->formation_index != BATTLE_FORMATION_MARIO_CARRYING &&
        actor->formation_index != BATTLE_FORMATION_LUIGI_CARRYING) {
        actor->actor.flags &= ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        task->callback = 0;
        return;
    }

    if (BattleContext_GetRuntimeState()->flags.bits.alternate_formation) {
        BattleSceneObject *source =
            BattleSceneObject_GetById(payload->actor_id);
        BattleSceneObject *linked =
            BattleSceneObject_GetById(actor->linked_object_id);

        BattleSceneObject_SetAnimation(
            linked, BATTLE_PARTY_LINKED_KNOCKOUT_ANIMATION, -1);
        BattleSceneObject_AdjustPosition(linked, source->x - linked->x,
                                         source->y - linked->y, -linked->z);
        linked->effect_anchor_z = 48;
    }

    if (payload->actor_id == BATTLE_ACTOR_MARIO) {
        BattleObjectData_EnsureLoaded(BATTLE_PARTY_MARIO_REBOUND_OBJECT_DATA_ID,
                                      BATTLE_PARTY_MARIO_REBOUND_RESOURCE_ID);
        *(u16 *)(gBattleContext + BATTLE_PARTY_MARIO_RELOAD_STATE_OFFSET) = 3;
        task->callback = BattleParty_UpdateMarioReloadWaitResource;
    } else {
        BattleObjectData_EnsureLoaded(BATTLE_PARTY_LUIGI_REBOUND_OBJECT_DATA_ID,
                                      BATTLE_PARTY_LUIGI_REBOUND_RESOURCE_ID);
        *(u16 *)(gBattleContext + BATTLE_PARTY_LUIGI_RELOAD_STATE_OFFSET) = 3;
        task->callback = BattleParty_UpdateLuigiReloadWaitResource;
    }
}
