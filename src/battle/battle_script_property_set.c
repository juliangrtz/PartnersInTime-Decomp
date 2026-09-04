#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_hit.h>
#include <game/battle_scene.h>
#include <game/battle_script_properties.h>

enum BattleScriptPropertySetOffset {
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

typedef struct BattleEnemyScriptPropertyView {
    BattleActor actor;
    u8 unknown_070[0x228];
    s16 property_298;
    s16 property_29a;
    union {
        u32 raw;
        struct {
            u32 bit_00 : 1;
            u32 flag_01 : 1;
            u32 bit_02 : 1;
            u32 flag_03 : 1;
            u32 flag_04 : 1;
            u32 flag_05 : 1;
            u32 bits_06_31 : 26;
        } bits;
    } flags_29c;
} BattleEnemyScriptPropertyView;

typedef union BattleScriptFlags16 {
    u16 raw;
    struct {
        u16 bit_00 : 1;
        u16 bits_01_05 : 5;
        u16 bit_06 : 1;
        u16 bit_07 : 1;
        u16 bit_08 : 1;
        u16 bits_09_11 : 3;
        u16 bit_12 : 1;
        u16 bit_13 : 1;
        u16 bit_14 : 1;
        u16 bit_15 : 1;
    } bits;
} BattleScriptFlags16;

typedef union BattleModelStateFlags {
    u16 raw;
    struct {
        u16 state : 5;
        u16 bits_05_15 : 11;
    } bits;
} BattleModelStateFlags;

typedef struct BattleModelDestroyVTable {
    u8 unknown_00[0x14];
    void (*destroy)(BattleModel *model);
} BattleModelDestroyVTable;

void OS_Terminate(void);
void func_ov002_0206f1b8(BattleModel *model);
void func_ov002_020bc73c(BattleSceneObject *object, int value);
void func_ov002_020baf90(BattleSceneObject *object, int value);
void func_ov002_020baf64(BattleSceneObject *object, int value);
void func_ov002_020baf38(BattleSceneObject *object, int value);
void func_ov002_020bae7c(BattleSceneObject *object, int value);
void func_ov002_020bae44(BattleSceneObject *object, int value);
void func_ov002_020baf18(BattleSceneObject *object, u16 value);
void func_ov002_020ba0d8(BattleSceneObject *object, int value);
void func_ov002_020ba0b0(BattleSceneObject *object, int value);
void func_ov002_020b8b14(BattleSceneObject *object,
                         BattleSceneObject *other, int a2, int a3, int a4);
void func_ov002_020b8ac8(BattleSceneObject *object, int a1, int a2);
void func_ov002_020b8aa0(BattleSceneObject *object, int value);
void func_ov002_020b8a90(BattleSceneObject *object, int value);
void func_ov002_020b8a80(BattleSceneObject *object, int value);
void func_ov002_020b8a58(BattleSceneObject *object, int value);
void func_ov002_020b8a3c(BattleSceneObject *object, int value);
void func_ov002_020b8a28(BattleSceneObject *object, int value);
void func_ov002_020b8a18(BattleSceneObject *object, int value);
void func_ov002_020b8a08(BattleSceneObject *object, int value);
void func_ov002_020b8128(u16 value);
void func_ov002_020b815c(BattleSceneObject *object);
void func_ov002_020b8114(BattleSceneObject *object, int value);
void func_ov002_020b80fc(BattleSceneObject *object, int value);
void func_ov002_020b8088(BattleSceneObject *object, u16 value, int channel);
void func_ov002_020b6a5c(BattleSceneObject *object);
void func_ov002_020b6a08(BattleSceneObject *object, int value);
void func_ov002_020b68e0(BattleSceneObject *object, int channel, u16 value);

#define FIELD_U16(object, offset) \
    (*(u16 *)((u8 *)(object) + (offset)))
#define FIELD_U8(object, offset) \
    (*(u8 *)((u8 *)(object) + (offset)))
#define FIELD_FLAGS16(object, offset) \
    (*(BattleScriptFlags16 *)((u8 *)(object) + (offset)))
#define MODEL_STATE(model) \
    (*(BattleModelStateFlags *)((u8 *)(model) + 0x162))

void BattleScript_SetProperty(u16 actor_id, int property, int value) {
    switch (property) {
    case BATTLE_PROPERTY_MODEL_FLAG_08: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        model->flags = (model->flags & ~0x100) | ((value & 1) << 8);
        break;
    }
    case BATTLE_PROPERTY_SCENE_FLAG_16_INVERTED: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        object->flags.bits.flag_16 = value == 0;
        break;
    }
    case BATTLE_PROPERTY_SCENE_EA:
        BattleSceneObject_GetById(actor_id)->effect_anchor_z = value;
        break;
    case BATTLE_PROPERTY_MODEL_MODE: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        model->flags = (model->flags & ~3) | (value & 3);
        break;
    }
    case BATTLE_PROPERTY_SCENE_DISTANCE:
        BattleSceneObject_GetById(actor_id)->smoothed_travel_distance = value;
        break;
    case BATTLE_PROPERTY_MODEL_SCALE: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        FIELD_U16(model, 0x5A) = (value << 8) / 4096;
        break;
    }
    case BATTLE_PROPERTY_CONSTANT_ZERO_06:
        return;
    case BATTLE_PROPERTY_ACTOR_FLAG_07: {
        BattleActor *actor = BattleActor_GetById(actor_id);
        actor->flag_bits.unknown_flag_07 = value;
        break;
    }
    case BATTLE_PROPERTY_PRIMARY_MODEL_STATE: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        if (object->flags.bits.use_alternate_model) OS_Terminate();
        if (object->primary_model == 0) OS_Terminate();
        if (value < 0) {
            MODEL_STATE(object->primary_model).bits.state = 0;
            return;
        }
        if (value < 30) {
            MODEL_STATE(object->primary_model).bits.state = value;
            return;
        }
        MODEL_STATE(object->primary_model).bits.state = 31;
        break;
    }
    case BATTLE_PROPERTY_MODEL_FLAG_03: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        model->flags = (model->flags & ~8) | ((value & 1) << 3);
        break;
    }
    case BATTLE_PROPERTY_POSITION_X:
        FIELD_U8(BattleSceneObject_GetById(actor_id), 0x102) = value;
        break;
    case BATTLE_PROPERTY_POSITION_Y:
        FIELD_U8(BattleSceneObject_GetById(actor_id), 0x103) = value;
        break;
    case BATTLE_PROPERTY_POSITION_Z: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        object->motion_target_z = value;
        object->z = object->motion_target_z;
        break;
    }
    case BATTLE_PROPERTY_CURRENT_HP:
        BattleActor_GetById(actor_id)->current_hp = value;
        break;
    case BATTLE_PROPERTY_MAX_HP:
        BattleActor_GetById(actor_id)->max_hp = value;
        break;
    case BATTLE_PROPERTY_POWER:
        BattleActor_GetById(actor_id)->power = value;
        break;
    case BATTLE_PROPERTY_DEFENSE:
        BattleActor_GetById(actor_id)->defense = value;
        break;
    case BATTLE_PROPERTY_SPEED:
        BattleActor_GetById(actor_id)->speed = value;
        break;
    case BATTLE_PROPERTY_CONSTANT_ZERO_21:
    case BATTLE_PROPERTY_CONSTANT_ZERO_22:
        return;
    case BATTLE_PROPERTY_ANIMATION_ID: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        model->animation_id = value;
        break;
    }
    case BATTLE_PROPERTY_MODEL_56: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        FIELD_U16(model, 0x56) = value;
        break;
    }
    case BATTLE_PROPERTY_MODEL_FLAG_10:
        BattleSceneObject_SetModelFlag10ById(actor_id, value);
        break;
    case BATTLE_PROPERTY_MODEL_FLAG_11:
        BattleSceneObject_SetModelFlag11ById(actor_id, value);
        break;
    case BATTLE_PROPERTY_MODEL_FLAG_09: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        model->flags = (model->flags & ~0x200) | ((value & 1) << 9);
        break;
    }
    case BATTLE_PROPERTY_OBJECT_DATA_ID:
        return;
    case BATTLE_PROPERTY_SCENE_STATE:
        BattleSceneObject_SetStateFlags(
            BattleSceneObject_GetById(actor_id), value, 0);
        break;
    case BATTLE_PROPERTY_SCENE_E6:
        FIELD_U16(BattleSceneObject_GetById(actor_id), 0xE6) = value;
        break;
    case BATTLE_PROPERTY_POSITION_RECORD_00: {
        BattlePositionRecord *record = (BattlePositionRecord *)(
            gBattleContext + BATTLE_POSITION_RECORDS_OFFSET);
        if (record->active != 0) {
            do {
                if (record->object_id == actor_id) record->object_id = value;
                record++;
            } while (record->active != 0);
        }
        break;
    }
    case BATTLE_PROPERTY_POSITION_RECORD_ACTOR: {
        BattlePositionRecord *record = (BattlePositionRecord *)(
            gBattleContext + BATTLE_POSITION_RECORDS_OFFSET);
        if (record->active != 0) {
            do {
                if (record->actor_id == actor_id) record->actor_id = value;
                record++;
            } while (record->active != 0);
        }
        break;
    }
    case BATTLE_PROPERTY_MODEL_78: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        FIELD_U16(model, 0x78) = value;
        break;
    }
    case BATTLE_PROPERTY_MODEL_74_Q4: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        FIELD_U16(model, 0x74) = value / 16;
        break;
    }
    case BATTLE_PROPERTY_MODEL_76_Q4: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        if (model == 0) OS_Terminate();
        FIELD_U16(model, 0x76) = value / 16;
        break;
    }
    case BATTLE_PROPERTY_PENDING_DAMAGE:
        BattleActor_GetById(actor_id)->pending_damage = value;
        break;
    case BATTLE_PROPERTY_ENEMY_RESOURCE_08:
        return;
    case BATTLE_PROPERTY_SCENE_E8:
        FIELD_U16(BattleSceneObject_GetById(actor_id), 0xE8) = value;
        break;
    case BATTLE_PROPERTY_ACTOR_22_PERCENT:
        FIELD_U16(BattleActor_GetById(actor_id), 0x22) =
            (value << 8) / 100;
        break;
    case BATTLE_PROPERTY_MODEL_FLAG_02:
    case BATTLE_PROPERTY_PARTY_FORMATION_INDEX:
        return;
    case BATTLE_PROPERTY_PARTY_8C:
        FIELD_U16(BattleActor_GetPartySlot(actor_id), 0x8C) = value;
        break;
    case BATTLE_PROPERTY_ENEMY_298:
        ((BattleEnemyScriptPropertyView *)
         BattleActor_GetEnemySlot(actor_id))->property_298 = value;
        break;
    case BATTLE_PROPERTY_ENEMY_FLAG_01: {
        BattleEnemyScriptPropertyView *enemy =
            (BattleEnemyScriptPropertyView *)BattleActor_GetEnemySlot(actor_id);
        enemy->flags_29c.bits.flag_01 = value != 0;
        break;
    }
    case BATTLE_PROPERTY_SCENE_1A: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        int enabled = value != 0;
        object->unk_01a = enabled;
        if (object->primary_model != 0) {
            object->primary_model->flags =
                (object->primary_model->flags & ~0x200) | ((enabled & 1) << 9);
        }
        if (object->alternate_model != 0) {
            object->alternate_model->flags =
                (object->alternate_model->flags & ~0x200) | ((enabled & 1) << 9);
        }
        if (actor_id >= 60 && actor_id < 68) {
            BattleActor *enemy = BattleActor_GetEnemySlot(actor_id);
            FIELD_FLAGS16(enemy, 0x292).bits.bit_00 = enabled;
            FIELD_FLAGS16(enemy, 0x1DA).bits.bit_00 =
                FIELD_FLAGS16(enemy, 0x292).bits.bit_00;
            FIELD_FLAGS16(enemy, 0x122).bits.bit_00 =
                FIELD_FLAGS16(enemy, 0x1DA).bits.bit_00;
        }
        break;
    }
    case BATTLE_PROPERTY_SCENE_FLAGS_20_23: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        object->flags.raw = (object->flags.raw & ~0xF00000) |
                            ((value & 0xF) << 20);
        break;
    }
    case BATTLE_PROPERTY_ACTOR_18:
        FIELD_U16(BattleActor_GetById(actor_id), 0x18) = value;
        break;
    case BATTLE_PROPERTY_ACTOR_1A:
        FIELD_U16(BattleActor_GetById(actor_id), 0x1A) = value;
        break;
    case BATTLE_PROPERTY_ACTOR_1C:
        FIELD_U16(BattleActor_GetById(actor_id), 0x1C) = value;
        break;
    case BATTLE_PROPERTY_HIT_FLAG_07_INVERTED: {
        BattleHitDescriptor *hit = BattleHitDescriptor_GetByActorId(actor_id);
        FIELD_FLAGS16(hit, 0x0C).bits.bit_07 = value == 0;
        break;
    }
    case BATTLE_PROPERTY_HIT_FLAG_08_INVERTED: {
        BattleHitDescriptor *hit = BattleHitDescriptor_GetByActorId(actor_id);
        FIELD_FLAGS16(hit, 0x0C).bits.bit_08 = value == 0;
        break;
    }
    case BATTLE_PROPERTY_ENEMY_FLAG_03: {
        BattleEnemyScriptPropertyView *enemy =
            (BattleEnemyScriptPropertyView *)BattleActor_GetEnemySlot(actor_id);
        enemy->flags_29c.bits.flag_03 = value != 0;
        break;
    }
    case BATTLE_PROPERTY_SCENE_FC:
        FIELD_U8(BattleSceneObject_GetById(actor_id), 0xFC) = value;
        break;
    case BATTLE_PROPERTY_SCENE_FD:
        FIELD_U8(BattleSceneObject_GetById(actor_id), 0xFD) = value;
        break;
    case BATTLE_PROPERTY_SCENE_F8:
        FIELD_U16(BattleSceneObject_GetById(actor_id), 0xF8) = value;
        break;
    case BATTLE_PROPERTY_SCENE_FE:
        FIELD_U8(BattleSceneObject_GetById(actor_id), 0xFE) = value;
        break;
    case BATTLE_PROPERTY_SCENE_FF:
        FIELD_U8(BattleSceneObject_GetById(actor_id), 0xFF) = value;
        break;
    case BATTLE_PROPERTY_SCENE_FA:
        FIELD_U16(BattleSceneObject_GetById(actor_id), 0xFA) = value;
        break;
    case BATTLE_PROPERTY_SCENE_100:
        FIELD_U8(BattleSceneObject_GetById(actor_id), 0x100) = value;
        break;
    case BATTLE_PROPERTY_SCENE_101:
        FIELD_U8(BattleSceneObject_GetById(actor_id), 0x101) = value;
        break;
    case BATTLE_PROPERTY_PARTY_FLAGS_06_07: {
        u16 property_value = value;
        BattleActor *party = BattleActor_GetPartySlot(actor_id);
        FIELD_U16(party, 0x74) = (FIELD_U16(party, 0x74) & ~0xC0) |
                                 ((property_value & 3) << 6);
        break;
    }
    case BATTLE_PROPERTY_SCENE_FLAGS_24_26: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        if (object->flags.bits.script_mode != value) {
            BattleSceneObject_SetAnimation(object, -1, -1);
            object->flags.bits.script_mode = value;
            switch (value) {
            case 0:
            case 1:
                return;
            case 2:
                func_ov002_020bc73c(object, 1023);
                break;
            default:
                return;
            }
        }
        break;
    }
    case BATTLE_PROPERTY_USE_RAW_POSITION: {
        int enabled;
        BattleSceneObject *object;
        if (value != 0) enabled = 1;
        else enabled = 0;
        object = BattleSceneObject_GetById(actor_id);
        object->flags.bits.use_raw_position = enabled;
        break;
    }
    case BATTLE_PROPERTY_SCENE_E4_MINUS_E8:
        return;
    case BATTLE_PROPERTY_ENEMY_29A:
        ((BattleEnemyScriptPropertyView *)
         BattleActor_GetEnemySlot(actor_id))->property_29a = value;
        break;
    case BATTLE_PROPERTY_ACTOR_KNOCKED_OUT: {
        BattleActor *actor = BattleActor_GetById(actor_id);
        actor->flag_bits.knocked_out = value != 0;
        break;
    }
    case BATTLE_PROPERTY_PARTY_FLAG_12: {
        BattleActor *party = BattleActor_GetPartySlot(actor_id);
        FIELD_FLAGS16(party, 0x74).bits.bit_12 = value != 0;
        break;
    }
    case BATTLE_PROPERTY_EXPECTED_PARTY_RESOURCE:
        return;
    case BATTLE_PROPERTY_PARTY_82:
        FIELD_U16(BattleActor_GetPartySlot(actor_id), 0x82) = value;
        break;
    case BATTLE_PROPERTY_SCENE_F0:
        FIELD_U16(BattleSceneObject_GetById(actor_id), 0xF0) = value;
        break;
    case BATTLE_PROPERTY_ENEMY_FLAG_04: {
        BattleEnemyScriptPropertyView *enemy =
            (BattleEnemyScriptPropertyView *)BattleActor_GetEnemySlot(actor_id);
        enemy->flags_29c.bits.flag_04 = value != 0;
        break;
    }
    case BATTLE_PROPERTY_SCENE_OPERATION_77:
        func_ov002_020baf90(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_78:
        func_ov002_020baf64(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_79:
        func_ov002_020baf38(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_80:
        func_ov002_020bae7c(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_81:
        func_ov002_020bae44(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_82:
        func_ov002_020baf18(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_ACTOR_FLAG_14: {
        int enabled;
        BattleActor *actor;
        if (value != 0) enabled = 1;
        else enabled = 0;
        actor = BattleActor_GetById(actor_id);
        FIELD_FLAGS16(actor, 0x24).bits.bit_14 = enabled;
        break;
    }
    case BATTLE_PROPERTY_RESET_MODEL_TRANSFORM: {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(actor_id));
        model->flags &= 0xE7FFFFFF;
        FIELD_U16(model, 0x78) = 0;
        FIELD_U16(model, 0x74) = 256;
        FIELD_U16(model, 0x76) = 256;
        break;
    }
    case BATTLE_PROPERTY_SCENE_OPERATION_85:
        func_ov002_020ba0d8(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_86:
        func_ov002_020ba0b0(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_ACTOR_0E:
        FIELD_U16(BattleActor_GetById(actor_id), 0x0E) = value;
        break;
    case BATTLE_PROPERTY_SCENE_1A_ALIAS:
    {
        int enabled;
        if (value != 0) enabled = 1;
        else enabled = 0;
        BattleSceneObject_GetById(actor_id)->unk_01a = enabled;
        break;
    }
    case BATTLE_PROPERTY_SCENE_16:
        BattleSceneObject_GetById(actor_id)->unk_016 = value;
        break;
    case BATTLE_PROPERTY_REPLACE_PRIMARY_MODEL: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        BattleModel *model = object->primary_model;
        if (model != 0) {
            func_ov002_0206f1b8(model);
            object->primary_model->flags &= ~0x100;
            model = object->primary_model;
            if (model != 0) {
                ((BattleModelDestroyVTable *)model->vtable)->destroy(model);
            }
        }
        object->primary_model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById((u16)value));
        break;
    }
    case BATTLE_PROPERTY_ENEMY_FLAG_05: {
        int enabled;
        BattleEnemyScriptPropertyView *enemy;
        if (value != 0) enabled = 1;
        else enabled = 0;
        enemy = (BattleEnemyScriptPropertyView *)
            BattleActor_GetEnemySlot(actor_id);
        enemy->flags_29c.bits.flag_05 = enabled;
        break;
    }
    case BATTLE_PROPERTY_SCENE_OPERATION_103: {
        BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
        BattleSceneObject *other = BattleSceneObject_GetById((u16)value);
        func_ov002_020b8b14(object, other, 3584, 56, 6);
        func_ov002_020b8ac8(object, 1, 1);
        func_ov002_020b8a58(object, 112);
        func_ov002_020b8a3c(object, 64);
        func_ov002_020b8a28(object, 0);
        break;
    }
    case BATTLE_PROPERTY_SCENE_OPERATION_104:
        func_ov002_020b8ac8(BattleSceneObject_GetById(actor_id), 1, value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_105:
        func_ov002_020b8aa0(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_106:
        func_ov002_020b8a90(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_107:
        func_ov002_020b8a80(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_108:
        func_ov002_020b8a18(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_109:
        func_ov002_020b8a08(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_110:
        func_ov002_020b8128(value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_111:
        func_ov002_020b815c(BattleSceneObject_GetById(actor_id));
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_112:
        func_ov002_020b8114(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_113:
        func_ov002_020b80fc(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_114:
        func_ov002_020b8088(BattleSceneObject_GetById(actor_id), value, 0);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_115:
        func_ov002_020b8088(BattleSceneObject_GetById(actor_id), value, 1);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_116:
        func_ov002_020b8088(BattleSceneObject_GetById(actor_id), value, 2);
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_120:
        func_ov002_020b6a5c(BattleSceneObject_GetById(actor_id));
        break;
    case BATTLE_PROPERTY_SCENE_OPERATION_121:
        func_ov002_020b6a08(BattleSceneObject_GetById(actor_id), value);
        break;
    case BATTLE_PROPERTY_SCENE_CHANNEL_0:
        func_ov002_020b68e0(BattleSceneObject_GetById(actor_id), 0, value);
        break;
    case BATTLE_PROPERTY_SCENE_CHANNEL_1:
        func_ov002_020b68e0(BattleSceneObject_GetById(actor_id), 1, value);
        break;
    case BATTLE_PROPERTY_SCENE_CHANNEL_2:
        func_ov002_020b68e0(BattleSceneObject_GetById(actor_id), 2, value);
        break;
    case BATTLE_PROPERTY_SCENE_CHANNEL_3:
        func_ov002_020b68e0(BattleSceneObject_GetById(actor_id), 3, value);
        break;
    case BATTLE_PROPERTY_ACTOR_FLAG_15: {
        int enabled;
        BattleActor *actor;
        if (value != 0) enabled = 1;
        else enabled = 0;
        actor = BattleActor_GetById(actor_id);
        FIELD_FLAGS16(actor, 0x24).bits.bit_15 = enabled;
        break;
    }
    default:
        break;
    }
}
