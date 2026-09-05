#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_party.h>
#include <game/battle_scene.h>

enum BattlePartyLuigiReloadConstant {
    BATTLE_PARTY_CONTEXT_PHASE_OFFSET = 0x18,
    BATTLE_PARTY_RELOAD_PHASE = 0x200A,
    BATTLE_PARTY_RELOAD_ACTIVE_FLAG = 1 << 2,
    BATTLE_PARTY_LUIGI_OBJECT_DATA_ID = 1,
    BATTLE_PARTY_LUIGI_REBOUND_OBJECT_DATA_ID = 6,
    BATTLE_PARTY_BABY_LUIGI_RESOURCE_ID = 0xC0000003,
    BATTLE_PARTY_RELOAD_MOTION_CHANNEL = 3
};

enum BattlePartyLuigiReloadAnimation {
    BATTLE_PARTY_LUIGI_REBOUND_ANIMATION = 0,
    BATTLE_PARTY_LUIGI_RETREAT_START_ANIMATION = 1,
    BATTLE_PARTY_LUIGI_RETREAT_ANIMATION = 2
};

void BattleParty_UpdateLuigiReloadWaitResource(BattleAITask *task) {
    int result;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    result = BattleObjectData_IsLoadPending(
        BATTLE_PARTY_LUIGI_REBOUND_OBJECT_DATA_ID);
    if (!result) {
        BattleEntity_BindResource(
            BATTLE_ACTOR_LUIGI,
            BATTLE_PARTY_LUIGI_REBOUND_OBJECT_DATA_ID);
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI),
            BATTLE_PARTY_LUIGI_REBOUND_ANIMATION, -1);
        BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI);
        BattleSound_Play(218, 0, 0, 0);
        task->callback = BattleParty_UpdateLuigiReloadWaitAnimation;
    }
}

void BattleParty_UpdateLuigiReloadWaitAnimation(BattleAITask *task) {
    BattleSceneObject *object;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    object = BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI);
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI),
            BATTLE_PARTY_LUIGI_RETREAT_START_ANIMATION, -1);
        task->callback = BattleParty_UpdateLuigiReloadStartRetreat;
    }
}

void BattleParty_UpdateLuigiReloadStartRetreat(BattleAITask *task) {
    BattleSceneObject *object;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    object = BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI);
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        int duration;

        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI),
            BATTLE_PARTY_LUIGI_RETREAT_ANIMATION, -1);
        duration = (object->x + 36) / 3;
        BattleSceneObject_AdjustPosition(object, -7, 0, 0);
        BattleSceneObject_MoveTo(
            object, BATTLE_PARTY_RELOAD_MOTION_CHANNEL,
            -36, object->y, object->z, duration);
        BattleSound_Play(70, 0, 0, 0);
        task->callback = BattleParty_UpdateLuigiReloadWaitRetreat;
    }
}

void BattleParty_UpdateLuigiReloadWaitRetreat(BattleAITask *task) {
    int result;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    result = BattleSceneObject_IsAnimationActiveById(
        BATTLE_ACTOR_LUIGI, BATTLE_PARTY_RELOAD_MOTION_CHANNEL);
    if (!result) {
        if (BattleContext_GetRuntimeState()->flags.bits.alternate_formation) {
            BattleSceneObject_GetById(BATTLE_ACTOR_MARIO)->effect_anchor_z = 0;
        }
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI), -1, -1);
        BattleActor_GetPartySlot(BATTLE_ACTOR_LUIGI)->flags &=
            ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        BattleActor_GetPartySlot(BATTLE_ACTOR_BABY_LUIGI)->flags |=
            BATTLE_ACTOR_FLAG_HIT_LOCK;
        task->callback = BattleParty_UpdateLuigiReloadRequestBaby;
    }
}

void BattleParty_UpdateLuigiReloadRequestBaby(BattleAITask *task) {
    u32 phase = *(u32 *)(gBattleContext + BATTLE_PARTY_CONTEXT_PHASE_OFFSET);

    if (phase == BATTLE_PARTY_RELOAD_PHASE) {
        *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
            BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
        BattleObjectData_EnsureLoaded(
            BATTLE_PARTY_LUIGI_OBJECT_DATA_ID,
            BATTLE_PARTY_BABY_LUIGI_RESOURCE_ID);
        task->callback = BattleParty_UpdateLuigiReloadFinish;
    }
}
