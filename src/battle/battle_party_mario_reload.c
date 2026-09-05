#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_party.h>
#include <game/battle_scene.h>

enum BattlePartyMarioReloadConstant {
    BATTLE_PARTY_CONTEXT_PHASE_OFFSET = 0x18,
    BATTLE_PARTY_RELOAD_PHASE = 0x200A,
    BATTLE_PARTY_RELOAD_ACTIVE_FLAG = 1 << 2,
    BATTLE_PARTY_MARIO_OBJECT_DATA_ID = 0,
    BATTLE_PARTY_MARIO_REBOUND_OBJECT_DATA_ID = 5,
    BATTLE_PARTY_BABY_MARIO_RESOURCE_ID = 0xC0000002,
    BATTLE_PARTY_RELOAD_MOTION_CHANNEL = 3
};

enum BattlePartyMarioReloadAnimation {
    BATTLE_PARTY_MARIO_REBOUND_ANIMATION = 0,
    BATTLE_PARTY_MARIO_RETREAT_START_ANIMATION = 1,
    BATTLE_PARTY_MARIO_RETREAT_ANIMATION = 2
};

void BattleParty_UpdateMarioReloadWaitResource(BattleAITask *task) {
    int result;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    result = BattleObjectData_IsLoadPending(
        BATTLE_PARTY_MARIO_REBOUND_OBJECT_DATA_ID);
    if (!result) {
        BattleEntity_BindResource(
            BATTLE_ACTOR_MARIO,
            BATTLE_PARTY_MARIO_REBOUND_OBJECT_DATA_ID);
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_MARIO),
            BATTLE_PARTY_MARIO_REBOUND_ANIMATION, -1);
        BattleSceneObject_GetById(BATTLE_ACTOR_MARIO);
        BattleSound_Play(218, 0, 0, 0);
        task->callback = BattleParty_UpdateMarioReloadWaitAnimation;
    }
}

void BattleParty_UpdateMarioReloadWaitAnimation(BattleAITask *task) {
    BattleSceneObject *object;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    object = BattleSceneObject_GetById(BATTLE_ACTOR_MARIO);
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_MARIO),
            BATTLE_PARTY_MARIO_RETREAT_START_ANIMATION, -1);
        BattleSound_Play(117, 15, 0, 0);
        task->callback = BattleParty_UpdateMarioReloadStartRetreat;
    }
}

void BattleParty_UpdateMarioReloadStartRetreat(BattleAITask *task) {
    BattleSceneObject *object;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    object = BattleSceneObject_GetById(BATTLE_ACTOR_MARIO);
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_MARIO),
            BATTLE_PARTY_MARIO_RETREAT_ANIMATION, -1);
        BattleSceneObject_MoveTo(
            object, BATTLE_PARTY_RELOAD_MOTION_CHANNEL,
            -32, object->y, object->z, (object->x + 32) / 3);
        BattleSound_Play(61, 0, 0, 0);
        task->callback = BattleParty_UpdateMarioReloadWaitRetreat;
    }
}

void BattleParty_UpdateMarioReloadWaitRetreat(BattleAITask *task) {
    int result;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    result = BattleSceneObject_IsAnimationActiveById(
        BATTLE_ACTOR_MARIO, BATTLE_PARTY_RELOAD_MOTION_CHANNEL);
    if (!result) {
        if (BattleContext_GetRuntimeState()->flags.bits.alternate_formation) {
            BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI)->effect_anchor_z = 0;
        }
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_MARIO), -1, -1);
        BattleActor_GetPartySlot(BATTLE_ACTOR_MARIO)->flags &=
            ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        BattleActor_GetPartySlot(BATTLE_ACTOR_BABY_MARIO)->flags |=
            BATTLE_ACTOR_FLAG_HIT_LOCK;
        task->callback = BattleParty_UpdateMarioReloadRequestBaby;
    }
}

void BattleParty_UpdateMarioReloadRequestBaby(BattleAITask *task) {
    u32 phase = *(u32 *)(gBattleContext + BATTLE_PARTY_CONTEXT_PHASE_OFFSET);

    if (phase == BATTLE_PARTY_RELOAD_PHASE) {
        *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
            BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
        BattleObjectData_EnsureLoaded(
            BATTLE_PARTY_MARIO_OBJECT_DATA_ID,
            BATTLE_PARTY_BABY_MARIO_RESOURCE_ID);
        task->callback = BattleParty_UpdateMarioReloadFinish;
    }
}
