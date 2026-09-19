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

#include <game/battle_status.h>
#include <game/item.h>
#include <game/save_data.h>

enum BattlePartyMarioReloadFinishConstant {
    SAVE_EQUIPMENT_RECORD_SIZE = 0x24,
    SAVE_EQUIPMENT_EFFECT_OFFSET = 0x418,
    EQUIPMENT_EFFECT_POWER_SPECIAL = 0x3024,
    BATTLE_PARTY_POWER_SPECIAL_MAGNITUDE = 40,
    BATTLE_PARTY_POWER_SPECIAL_CHANCE = 101
};
void BattleParty_UpdateMarioReloadFinish(BattleAITask *task) {
    int result;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    result = BattleObjectData_IsLoadPending(
        BATTLE_PARTY_MARIO_OBJECT_DATA_ID);
    if (!result) {
        BattleSceneObject *adult =
            BattleSceneObject_GetById(BATTLE_ACTOR_MARIO);
        BattleSceneObject *baby =
            BattleSceneObject_GetById(BATTLE_ACTOR_BABY_MARIO);
        BattlePartyActor *actor;

        baby->x = baby->motion_target_x = baby->motion_origin_x = adult->x;
        baby->y = baby->motion_target_y = baby->motion_origin_y = adult->y;
        baby->z = baby->motion_target_z = baby->motion_origin_z = 0;

        BattleSceneObject_SwapSlots(
            BATTLE_ACTOR_BABY_MARIO, BATTLE_ACTOR_MARIO);
        BattleActor_GetPartySlot(BATTLE_ACTOR_MARIO)->flags &=
            ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        BattleActor_GetPartySlot(BATTLE_ACTOR_BABY_MARIO)->flags &=
            ~BATTLE_ACTOR_FLAG_HIT_LOCK;

        BattleEntity_BindResource(
            BATTLE_ACTOR_MARIO, BATTLE_PARTY_MARIO_OBJECT_DATA_ID);
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_MARIO), 2, -1);
        BattleEntity_BindResource(
            BATTLE_ACTOR_BABY_MARIO, BATTLE_PARTY_MARIO_OBJECT_DATA_ID);
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_BABY_MARIO), -1, -1);

        actor = (BattlePartyActor *)BattleActor_GetPartySlot(
            BATTLE_ACTOR_MARIO);
        if (!actor->state_flags.status_bits.power_special_disabled) {
            u8 *equipment_record =
                gSaveData + SAVE_EQUIPMENT_RECORD_SIZE *
                                (u8)*(u16 *)actor->actor.resource_slot;

            if ((equipment_record[SAVE_EQUIPMENT_EFFECT_OFFSET] |
                 ITEM_ID_BADGE_TAG) == EQUIPMENT_EFFECT_POWER_SPECIAL) {
                BattleStatus_TryApply(
                    &actor->actor, BATTLE_STATUS_POWER_CHANGE, 0x7FFF,
                    BATTLE_PARTY_POWER_SPECIAL_MAGNITUDE,
                    BATTLE_PARTY_POWER_SPECIAL_CHANCE);
                actor->actor.power_change.parameter_1 = 0;
            }
        }
        task->callback = 0;
    }
}

enum BattlePartyLuigiReloadConstant {
    BATTLE_PARTY_LUIGI_OBJECT_DATA_ID = 1,
    BATTLE_PARTY_LUIGI_REBOUND_OBJECT_DATA_ID = 6,
    BATTLE_PARTY_BABY_LUIGI_RESOURCE_ID = 0xC0000003
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

void BattleParty_UpdateLuigiReloadFinish(BattleAITask *task) {
    int result;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_PARTY_RELOAD_ACTIVE_FLAG;
    result = BattleObjectData_IsLoadPending(BATTLE_PARTY_LUIGI_OBJECT_DATA_ID);
    if (!result) {
        BattleSceneObject *adult =
            BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI);
        BattleSceneObject *baby =
            BattleSceneObject_GetById(BATTLE_ACTOR_BABY_LUIGI);
        BattlePartyActor *actor;

        baby->x = baby->motion_target_x = baby->motion_origin_x = adult->x;
        baby->y = baby->motion_target_y = baby->motion_origin_y = adult->y;
        baby->z = baby->motion_target_z = baby->motion_origin_z = 0;

        BattleSceneObject_SwapSlots(BATTLE_ACTOR_BABY_LUIGI,
                                    BATTLE_ACTOR_LUIGI);
        BattleActor_GetPartySlot(BATTLE_ACTOR_LUIGI)->flags &=
            ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        BattleActor_GetPartySlot(BATTLE_ACTOR_BABY_LUIGI)->flags &=
            ~BATTLE_ACTOR_FLAG_HIT_LOCK;

        BattleEntity_BindResource(BATTLE_ACTOR_LUIGI,
                                  BATTLE_PARTY_LUIGI_OBJECT_DATA_ID);
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_LUIGI), 2, -1);
        BattleEntity_BindResource(BATTLE_ACTOR_BABY_LUIGI,
                                  BATTLE_PARTY_LUIGI_OBJECT_DATA_ID);
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_ACTOR_BABY_LUIGI), -1, -1);

        actor =
            (BattlePartyActor *)BattleActor_GetPartySlot(BATTLE_ACTOR_LUIGI);
        if (!actor->state_flags.status_bits.power_special_disabled) {
            u8 *equipment_record =
                gSaveData + SAVE_EQUIPMENT_RECORD_SIZE * (u8) *
                                (u16 *)actor->actor.resource_slot;

            if ((equipment_record[SAVE_EQUIPMENT_EFFECT_OFFSET] |
                 ITEM_ID_BADGE_TAG) == EQUIPMENT_EFFECT_POWER_SPECIAL) {
                BattleStatus_TryApply(&actor->actor, BATTLE_STATUS_POWER_CHANGE,
                                      0x7FFF,
                                      BATTLE_PARTY_POWER_SPECIAL_MAGNITUDE,
                                      BATTLE_PARTY_POWER_SPECIAL_CHANCE);
                actor->actor.power_change.parameter_1 = 0;
            }
        }
        task->callback = 0;
    }
}
