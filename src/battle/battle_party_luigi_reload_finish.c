#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_party.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>
#include <game/item.h>
#include <game/save_data.h>

enum BattlePartyLuigiReloadFinishConstant {
    BATTLE_PARTY_RELOAD_ACTIVE_FLAG = 1 << 2,
    BATTLE_PARTY_LUIGI_OBJECT_DATA_ID = 1,
    SAVE_EQUIPMENT_RECORD_SIZE = 0x24,
    SAVE_EQUIPMENT_EFFECT_OFFSET = 0x418,
    EQUIPMENT_EFFECT_POWER_SPECIAL = 0x3024,
    BATTLE_PARTY_POWER_SPECIAL_MAGNITUDE = 40,
    BATTLE_PARTY_POWER_SPECIAL_CHANCE = 101
};

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
