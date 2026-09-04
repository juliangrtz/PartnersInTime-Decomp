#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_hit.h>
#include <game/battle_script_properties.h>

enum BattleScriptPropertyOffset {
    BATTLE_POSITION_RECORDS_OFFSET = 0xCAD8
};

typedef struct BattlePositionRecord {
    u16 object_id;
    u16 actor_id;
    s16 x;
    s16 anchor_y;
    s16 offset_y;
    s16 active;
    u8 unknown_0c[8];
} BattlePositionRecord;

typedef struct BattlePartyScriptPropertyView {
    BattlePartyActor party;
    u8 unknown_084[8];
    u16 property_08c;
} BattlePartyScriptPropertyView;

typedef struct BattleEnemyScriptResource {
    u8 unknown_00[8];
    u16 property_08;
    u16 property_0a;
} BattleEnemyScriptResource;

typedef union BattleModelScriptFlags {
    u32 raw;
    struct {
        u32 mode : 2;
        u32 unknown_02_31 : 30;
    } bits;
} BattleModelScriptFlags;

typedef struct BattleEnemyScriptPropertyView {
    BattleActor actor;
    u8 unknown_070[0x228];
    s16 property_298;
    s16 property_29a;
    u32 flags_29c;
} BattleEnemyScriptPropertyView;

typedef struct BattleSceneObjectScriptPropertyView {
    BattleSceneObject object;
    s16 property_0f8;
    s16 property_0fa;
    s8 property_0fc;
    s8 property_0fd;
    s8 property_0fe;
    s8 property_0ff;
    s8 property_100;
    s8 property_101;
} BattleSceneObjectScriptPropertyView;

typedef char BattlePositionRecord_SizeCheck[
    sizeof(BattlePositionRecord) == 0x14 ? 1 : -1];
typedef char BattlePartyScriptPropertyView_SizeCheck[
    sizeof(BattlePartyScriptPropertyView) == 0x90 ? 1 : -1];
typedef char BattleEnemyScriptPropertyView_SizeCheck[
    sizeof(BattleEnemyScriptPropertyView) == 0x2A0 ? 1 : -1];
typedef char BattleSceneObjectScriptPropertyView_SizeCheck[
    sizeof(BattleSceneObjectScriptPropertyView) == 0x104 ? 1 : -1];

void OS_Terminate(void);
int func_ov002_020b8070(BattleSceneObject *object, int channel);
int func_ov002_020b6928(BattleSceneObject *object);

#define FIELD_S8(object, offset) \
    (*(s8 *)((u8 *)(object) + (offset)))
#define FIELD_S16(object, offset) \
    (*(s16 *)((u8 *)(object) + (offset)))
#define FIELD_U16(object, offset) \
    (*(u16 *)((u8 *)(object) + (offset)))

int BattleScript_GetProperty(u16 actor_id, int property) {
    switch (property) {
    case BATTLE_PROPERTY_MODEL_FLAG_08: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        return model != 0 ? (model->flags << 23) >> 31 : 0;
    }
    case BATTLE_PROPERTY_SCENE_FLAG_16_INVERTED:
        return ((BattleSceneObject_GetById(actor_id)->flags.raw << 15) >> 31)
               ^ 1;
    case BATTLE_PROPERTY_SCENE_EA:
        return BattleSceneObject_GetById(actor_id)->effect_anchor_z;
    case BATTLE_PROPERTY_MODEL_MODE: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return ((BattleModelScriptFlags *)&model->flags)->bits.mode;
    }
    case BATTLE_PROPERTY_SCENE_DISTANCE:
        return BattleSceneObject_GetById(actor_id)->smoothed_travel_distance;
    case BATTLE_PROPERTY_MODEL_SCALE: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return (FIELD_S16(model, 0x5A) << 12) / 256;
    }
    case BATTLE_PROPERTY_CONSTANT_ZERO_06:
        return 0;
    case BATTLE_PROPERTY_ACTOR_FLAG_07:
        return (u32)(BattleActor_GetById(actor_id)->flags << 24) >> 31;
    case BATTLE_PROPERTY_PRIMARY_MODEL_STATE: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        if (object->flags.bits.use_alternate_model) {
            OS_Terminate();
        }
        if (object->primary_model == 0) {
            OS_Terminate();
        }
        return (u32)(FIELD_U16(object->primary_model, 0x162) << 27) >> 27;
    }
    case BATTLE_PROPERTY_MODEL_FLAG_03: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return (model->flags << 28) >> 31;
    }
    case BATTLE_PROPERTY_POSITION_X:
        return BattleSceneObject_GetById(actor_id)->x;
    case BATTLE_PROPERTY_POSITION_Y:
        return BattleSceneObject_GetById(actor_id)->y;
    case BATTLE_PROPERTY_POSITION_Z:
        return BattleSceneObject_GetById(actor_id)->z;
    case BATTLE_PROPERTY_CURRENT_HP:
        return BattleActor_GetById(actor_id)->current_hp;
    case BATTLE_PROPERTY_MAX_HP:
        return BattleActor_GetById(actor_id)->max_hp;
    case BATTLE_PROPERTY_POWER:
        return BattleActor_GetById(actor_id)->power;
    case BATTLE_PROPERTY_DEFENSE:
        return BattleActor_GetById(actor_id)->defense;
    case BATTLE_PROPERTY_SPEED:
        return BattleActor_GetById(actor_id)->speed;
    case BATTLE_PROPERTY_CONSTANT_ZERO_21:
        return 0;
    case BATTLE_PROPERTY_CONSTANT_ZERO_22:
        return 0;
    case BATTLE_PROPERTY_ANIMATION_ID: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return model->animation_id;
    }
    case BATTLE_PROPERTY_MODEL_56: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return FIELD_S16(model, 0x56);
    }
    case BATTLE_PROPERTY_MODEL_FLAG_10: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return (model->flags << 21) >> 31;
    }
    case BATTLE_PROPERTY_MODEL_FLAG_11: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return (model->flags << 20) >> 31;
    }
    case BATTLE_PROPERTY_MODEL_FLAG_09: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return (model->flags << 22) >> 31;
    }
    case BATTLE_PROPERTY_OBJECT_DATA_ID: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        if (object->resource == 0) {
            OS_Terminate();
        }
        return object->resource->object_data_id;
    }
    case BATTLE_PROPERTY_SCENE_STATE:
        return FIELD_S8(BattleSceneObject_GetById(actor_id), 0xF4);
    case BATTLE_PROPERTY_SCENE_E6:
        return FIELD_S16(BattleSceneObject_GetById(actor_id), 0xE6);
    case BATTLE_PROPERTY_POSITION_RECORD_00: {
        BattlePositionRecord *record = (BattlePositionRecord *)(
            gBattleContext + BATTLE_POSITION_RECORDS_OFFSET);
        while (record->active != 0) {
            if (record->object_id == actor_id) {
                return record->active;
            }
            record++;
        }
        return 0;
    }
    case BATTLE_PROPERTY_POSITION_RECORD_ACTOR: {
        BattlePositionRecord *record = (BattlePositionRecord *)(
            gBattleContext + BATTLE_POSITION_RECORDS_OFFSET);
        while (record->active != 0) {
            if (record->actor_id == actor_id) {
                return record->active;
            }
            record++;
        }
        return 0;
    }
    case BATTLE_PROPERTY_MODEL_78: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return FIELD_U16(model, 0x78);
    }
    case BATTLE_PROPERTY_MODEL_74_Q4: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return 16 * FIELD_S16(model, 0x74);
    }
    case BATTLE_PROPERTY_MODEL_76_Q4: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return 16 * FIELD_S16(model, 0x76);
    }
    case BATTLE_PROPERTY_PENDING_DAMAGE:
        return BattleActor_GetById(actor_id)->pending_damage;
    case BATTLE_PROPERTY_ENEMY_RESOURCE_08: {
        BattleEnemyScriptResource *resource =
            (BattleEnemyScriptResource *)BattleActor_GetEnemySlot(actor_id)
                ->resource_slot;
        if (resource == 0) {
            return 0;
        }
        return resource->property_08;
    }
    case BATTLE_PROPERTY_SCENE_E8:
        return FIELD_S16(BattleSceneObject_GetById(actor_id), 0xE8);
    case BATTLE_PROPERTY_ACTOR_22_PERCENT:
        return 100 * FIELD_S16(BattleActor_GetById(actor_id), 0x22) / 256;
    case BATTLE_PROPERTY_MODEL_FLAG_02: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) {
            OS_Terminate();
        }
        return (model->flags << 29) >> 31;
    }
    case BATTLE_PROPERTY_PARTY_FORMATION_INDEX:
        return ((BattlePartyActor *)
                BattleActor_GetPartySlot(actor_id))->formation_index;
    case BATTLE_PROPERTY_PARTY_8C:
        return ((BattlePartyScriptPropertyView *)
                BattleActor_GetPartySlot(actor_id))->property_08c;
    case BATTLE_PROPERTY_ENEMY_298:
        return ((BattleEnemyScriptPropertyView *)
                BattleActor_GetEnemySlot(actor_id))->property_298;
    case BATTLE_PROPERTY_ENEMY_FLAG_01:
        return (((BattleEnemyScriptPropertyView *)
                 BattleActor_GetEnemySlot(actor_id))->flags_29c << 30) >> 31;
    case BATTLE_PROPERTY_SCENE_1A:
        return BattleSceneObject_GetById(actor_id)->unk_01a;
    case BATTLE_PROPERTY_SCENE_FLAGS_20_23:
        return (BattleSceneObject_GetById(actor_id)->flags.raw << 8) >> 28;
    case BATTLE_PROPERTY_ENEMY_RESOURCE_0A: {
        BattleEnemyScriptResource *resource =
            (BattleEnemyScriptResource *)BattleActor_GetEnemySlot(actor_id)
                ->resource_slot;
        if (resource == 0) {
            return -1;
        }
        return resource->property_0a;
    }
    case BATTLE_PROPERTY_ACTOR_18:
        return FIELD_S16(BattleActor_GetById(actor_id), 0x18);
    case BATTLE_PROPERTY_ACTOR_1A:
        return FIELD_S16(BattleActor_GetById(actor_id), 0x1A);
    case BATTLE_PROPERTY_ACTOR_1C:
        return FIELD_S16(BattleActor_GetById(actor_id), 0x1C);
    case BATTLE_PROPERTY_HIT_FLAG_07_INVERTED:
        return (((u32)(BattleHitDescriptor_GetByActorId(actor_id)->flags << 24)
                 >> 31) == 0);
    case BATTLE_PROPERTY_HIT_FLAG_08_INVERTED:
        return (((u32)(BattleHitDescriptor_GetByActorId(actor_id)->flags << 23)
                 >> 31) == 0);
    case BATTLE_PROPERTY_ENEMY_FLAG_03:
        return (((BattleEnemyScriptPropertyView *)
                 BattleActor_GetEnemySlot(actor_id))->flags_29c << 28) >> 31;
    case BATTLE_PROPERTY_VIEW_POSITION_X: {
        BattlePosition position;
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        BattlePosition_StoreViewRelative(
            &position, object->x, (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        return position.x;
    }
    case BATTLE_PROPERTY_VIEW_POSITION_Y: {
        BattlePosition position;
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        BattlePosition_StoreViewRelative(
            &position, object->x, (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        return position.y;
    }
    case BATTLE_PROPERTY_VIEW_POSITION_Z: {
        BattlePosition position;
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        BattlePosition_StoreViewRelative(
            &position, object->x, (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        return position.z;
    }
    case BATTLE_PROPERTY_SCENE_FC:
        return ((BattleSceneObjectScriptPropertyView *)
                BattleSceneObject_GetById(actor_id))->property_0fc;
    case BATTLE_PROPERTY_SCENE_FD:
        return ((BattleSceneObjectScriptPropertyView *)
                BattleSceneObject_GetById(actor_id))->property_0fd;
    case BATTLE_PROPERTY_SCENE_F8:
        return ((BattleSceneObjectScriptPropertyView *)
                BattleSceneObject_GetById(actor_id))->property_0f8;
    case BATTLE_PROPERTY_SCENE_FE:
        return ((BattleSceneObjectScriptPropertyView *)
                BattleSceneObject_GetById(actor_id))->property_0fe;
    case BATTLE_PROPERTY_SCENE_FF:
        return ((BattleSceneObjectScriptPropertyView *)
                BattleSceneObject_GetById(actor_id))->property_0ff;
    case BATTLE_PROPERTY_SCENE_FA:
        return ((BattleSceneObjectScriptPropertyView *)
                BattleSceneObject_GetById(actor_id))->property_0fa;
    case BATTLE_PROPERTY_SCENE_100:
        return ((BattleSceneObjectScriptPropertyView *)
                BattleSceneObject_GetById(actor_id))->property_100;
    case BATTLE_PROPERTY_SCENE_101:
        return ((BattleSceneObjectScriptPropertyView *)
                BattleSceneObject_GetById(actor_id))->property_101;
    case BATTLE_PROPERTY_PARTY_FLAGS_06_07:
        return (u32)(FIELD_U16(BattleActor_GetPartySlot(actor_id), 0x74)
                     << 24) >> 30;
    case BATTLE_PROPERTY_SCENE_FLAGS_24_26:
        return (32 * BattleSceneObject_GetById(actor_id)->flags.raw) >> 29;
    case BATTLE_PROPERTY_USE_RAW_POSITION:
        return (BattleSceneObject_GetById(actor_id)->flags.raw << 14) >> 31;
    case BATTLE_PROPERTY_SCENE_E4_MINUS_E8: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        return FIELD_S16(object, 0xE4) - FIELD_S16(object, 0xE8);
    }
    case BATTLE_PROPERTY_ENEMY_29A:
        return ((BattleEnemyScriptPropertyView *)
                BattleActor_GetEnemySlot(actor_id))->property_29a;
    case BATTLE_PROPERTY_ACTOR_KNOCKED_OUT:
        return (u32)(BattleActor_GetById(actor_id)->flags << 23) >> 31;
    case BATTLE_PROPERTY_PARTY_FLAG_12:
        return (u32)(FIELD_U16(BattleActor_GetPartySlot(actor_id), 0x74)
                     << 19) >> 31;
    case BATTLE_PROPERTY_EXPECTED_PARTY_RESOURCE: {
        BattleSceneResource *resource =
            BattleSceneObject_GetById(actor_id)->resource;
        if (resource == 0) {
            return 0;
        }
        if (actor_id == 56) {
            return resource->object_data_id == 5;
        }
        return resource->object_data_id == 6;
    }
    case BATTLE_PROPERTY_PARTY_82:
        return FIELD_U16(BattleActor_GetPartySlot(actor_id), 0x82);
    case BATTLE_PROPERTY_SCENE_F0:
        return FIELD_U16(BattleSceneObject_GetById(actor_id), 0xF0);
    case BATTLE_PROPERTY_ENEMY_FLAG_04:
        return (((BattleEnemyScriptPropertyView *)
                 BattleActor_GetEnemySlot(actor_id))->flags_29c << 27) >> 31;
    case BATTLE_PROPERTY_ACTOR_FLAG_14:
        return (u32)(BattleActor_GetById(actor_id)->flags << 17) >> 31;
    case BATTLE_PROPERTY_ACTOR_0E:
        return FIELD_S16(BattleActor_GetById(actor_id), 0x0E);
    case BATTLE_PROPERTY_TRANSITION_STATE_1:
        return BattleActor_GetById(actor_id)->transition_state == 1;
    case BATTLE_PROPERTY_TRANSITION_STATE_2:
        return BattleActor_GetById(actor_id)->transition_state == 2;
    case BATTLE_PROPERTY_TRANSITION_STATE_3:
        return BattleActor_GetById(actor_id)->transition_state == 3;
    case BATTLE_PROPERTY_ACTOR_44_STATE_1:
        return FIELD_S8(BattleActor_GetById(actor_id), 0x44) == 1;
    case BATTLE_PROPERTY_ACTOR_44_STATE_2:
        return FIELD_S8(BattleActor_GetById(actor_id), 0x44) == 2;
    case BATTLE_PROPERTY_ACTOR_50:
        return FIELD_S8(BattleActor_GetById(actor_id), 0x50);
    case BATTLE_PROPERTY_ACTOR_5C:
        return FIELD_S8(BattleActor_GetById(actor_id), 0x5C);
    case BATTLE_PROPERTY_ACTOR_68:
        return FIELD_S8(BattleActor_GetById(actor_id), 0x68);
    case BATTLE_PROPERTY_SCENE_1A_ALIAS:
        return BattleSceneObject_GetById(actor_id)->unk_01a;
    case BATTLE_PROPERTY_SCENE_16:
        return BattleSceneObject_GetById(actor_id)->unk_016;
    case BATTLE_PROPERTY_MOTION_ORIGIN_X:
        return BattleSceneObject_GetById(actor_id)->motion_origin_x;
    case BATTLE_PROPERTY_MOTION_ORIGIN_Y:
        return BattleSceneObject_GetById(actor_id)->motion_origin_y;
    case BATTLE_PROPERTY_MOTION_ORIGIN_Z:
        return BattleSceneObject_GetById(actor_id)->motion_origin_z;
    case BATTLE_PROPERTY_ENEMY_FLAG_05:
        return (((BattleEnemyScriptPropertyView *)
                 BattleActor_GetEnemySlot(actor_id))->flags_29c << 26) >> 31;
    case BATTLE_PROPERTY_CHANNEL_STATE_0:
        return func_ov002_020b8070(BattleSceneObject_GetById(actor_id), 0);
    case BATTLE_PROPERTY_CHANNEL_STATE_1:
        return func_ov002_020b8070(BattleSceneObject_GetById(actor_id), 1);
    case BATTLE_PROPERTY_CHANNEL_STATE_2:
        return func_ov002_020b8070(BattleSceneObject_GetById(actor_id), 2);
    case BATTLE_PROPERTY_SCENE_DERIVED_128:
        return func_ov002_020b6928(BattleSceneObject_GetById(actor_id));
    case BATTLE_PROPERTY_ACTOR_FLAG_15:
        return (u32)(BattleActor_GetById(actor_id)->flags << 16) >> 31;
    default:
        return 0;
    }
}
