#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_damage.h>
#include <game/battle_damage_number.h>
#include <game/battle_damage_reaction.h>
#include <game/battle_effect.h>
#include <game/battle_hit.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>
#include <game/item.h>
#include <game/save_data.h>

enum BattleDamageApplicationConstant {
    BATTLE_DAMAGE_MAX = 999,
    BATTLE_DAMAGE_SCALE = 256,
    BATTLE_ENEMY_HIT_MODE_OFFSET = 0xCB78,
    SAVE_EQUIPMENT_RECORD_SIZE = 0x24,
    SAVE_EQUIPMENT_EFFECT_OFFSET = 0x418
};

enum BattleDamageApplicationEffect {
    BATTLE_ENEMY_REACTION_EFFECT_NORMAL = 0x11,
    BATTLE_ENEMY_REACTION_EFFECT_ALTERNATE = 0x13,
    BATTLE_ENEMY_POPUP_EFFECT = 7,
    EQUIPMENT_EFFECT_POWER_SPECIAL = 0x3024
};

enum BattleDamageApplicationResource {
    BATTLE_DAMAGE_SPECIAL_RESOURCE_1 = (s32)0xC1000082,
    BATTLE_DAMAGE_SPECIAL_RESOURCE_2 = (s32)0xC10000A4
};

typedef struct BattleDamagePopupControl {
    u8 unknown_00[4];
    u16 flags;
} BattleDamagePopupControl;

typedef struct BattleDamageActorResource {
    BattleDamagePopupControl *popup_control;
} BattleDamageActorResource;

void BattleDamage_DispatchHit(BattleHitRecord *record) {
    u32 actor_id = record->target_id;
    int actor_kind;
    BattleSceneObject *object;
    BattleActor *actor;
    int damage;
    int offset_x;
    int offset_y;

    if (actor_id >= BATTLE_ACTOR_PARTY_FIRST &&
        actor_id < BATTLE_ACTOR_PARTY_FIRST + BATTLE_ACTOR_PARTY_COUNT) {
        actor_kind = 0;
    } else {
        if (actor_id < BATTLE_ACTOR_ENEMY_FIRST) {
            return;
        }
        if (actor_id < BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT) {
            actor_kind = 1;
        } else {
            return;
        }
    }

    object = BattleSceneObject_GetById(actor_id);
    actor = BattleActor_GetById(actor_id);
    offset_x = record->x - object->x;
    offset_y = (record->y - object->y) - (record->z - object->z);
    damage = actor->pending_damage;

    if (actor_kind != 0) {
        int enemy_hit_mode =
            *(s16 *)(gBattleContext + BATTLE_ENEMY_HIT_MODE_OFFSET);
        int reaction_effect;

        if (enemy_hit_mode != 0 && enemy_hit_mode != 1) {
            return;
        }
        reaction_effect = enemy_hit_mode == 0
                              ? BATTLE_ENEMY_REACTION_EFFECT_NORMAL
                              : BATTLE_ENEMY_REACTION_EFFECT_ALTERNATE;
        BattleDamage_ApplyToEnemy(object, offset_x, offset_y, damage,
                                  reaction_effect, BATTLE_ENEMY_POPUP_EFFECT,
                                  0);
        return;
    }

    if (BattleActor_GetById(actor_id)->current_hp <= 0) {
        return;
    }
    BattleDamage_ApplyToParty(object, offset_x, offset_y, damage,
                              record->kind == 2, record);
}

void BattleActor_HandleHitEquipmentEffect(int actor_id) {
    BattlePartyActor *actor =
        (BattlePartyActor *)BattleActor_GetPartySlot((u16)actor_id);

    if (actor->state_flags.status_bits.power_special_disabled) {
        return;
    }
    actor->state_flags.status_bits.power_special_disabled = 1;
    {
        u8 *save_cursor = gSaveData;
        int equipment_index = (u8) * (u16 *)actor->actor.resource_slot;
        u8 *equipment_record =
            save_cursor + SAVE_EQUIPMENT_RECORD_SIZE * equipment_index;
        int equipment_effect =
            equipment_record[SAVE_EQUIPMENT_EFFECT_OFFSET] | ITEM_ID_BADGE_TAG;

        if (equipment_effect != EQUIPMENT_EFFECT_POWER_SPECIAL) {
            return;
        }
    }
    BattleStatus_ClearEffect(&actor->actor, BATTLE_STATUS_POWER_CHANGE);
}

void BattleDamage_ApplyToParty(BattleSceneObject *object, int popup_offset_x,
                               int popup_offset_y, int damage, int hit_kind,
                               BattleHitRecord *status_record) {
    BattlePartyActor *party =
        (BattlePartyActor *)BattleActor_GetPartySlot(object->actor_id);
    BattlePosition position;
    BattleDamageNumberEffect *number;

    if (damage < 1) {
        damage = 1;
    }
    if (damage > BATTLE_DAMAGE_MAX) {
        damage = BATTLE_DAMAGE_MAX;
    }
    BattleActor_HandleHitEquipmentEffect((s16)object->actor_id);
    BattleStatus_ClearEffect(&party->actor, BATTLE_STATUS_AILMENT_1);

    if (hit_kind != 2 || damage < party->actor.current_hp) {
        BattleActor_ApplyDamage(object, damage);
    }
    if (hit_kind != 2) {
        BattleDamage_StartPartyReaction(object);
    }

    BattlePosition_StoreViewRelative(
        &position, object->x, (s16)(object->y - object->z),
        (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
        object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model);
    BattleEntity_BindResource(object->actor_id,
                              object->actor_id != BATTLE_ACTOR_MARIO);

    number = 0;
    switch (hit_kind) {
    case 0:
        BattleSound_Play(0x24, 0, 0, 0);
        BattleScreenEffect_StartPrimary(3, 6, 0);
        number = (BattleDamageNumberEffect *)BattleDamage_SpawnNumber(
            damage, position.x + popup_offset_x, position.y + popup_offset_y, 4,
            0);
        BattleSceneObject_SetAnimation(object, 0x0A, -1);
        break;
    case 1:
        BattleSound_Play(0x24, 0, 0, 0);
        BattleScreenEffect_StartPrimary(3, 6, 0);
        number = (BattleDamageNumberEffect *)BattleDamage_SpawnNumber(
            damage, position.x + popup_offset_x, position.y + popup_offset_y, 5,
            0);
        BattleSceneObject_SetAnimation(object, 0x0B, -1);
        break;
    case 2:
        BattleSound_Play(0x25, 0, 0, 0);
        BattleScreenEffect_StartPrimary(1, 0x3C, 0);
        number = (BattleDamageNumberEffect *)BattleDamage_SpawnNumber(
            damage, position.x + popup_offset_x, position.y + popup_offset_y, 4,
            0);
        BattleSceneObject_SetAnimation(object, 0x0C, -1);
        BattleModelEffect_Spawn(0x1B, object, (s16)popup_offset_x,
                                (s16)popup_offset_y, 0, BATTLE_DAMAGE_SCALE);
        BattleSpriteEffect_Spawn(0x0C, (s16)(position.x + popup_offset_x),
                                 (s16)(position.y + popup_offset_y), position.z,
                                 BATTLE_DAMAGE_SCALE);
        break;
    case 3:
        BattleScreenEffect_StartPrimary(1, 0x28, 0);
        number = (BattleDamageNumberEffect *)BattleDamage_SpawnNumber(
            damage, position.x + popup_offset_x, position.y + popup_offset_y, 4,
            0);
        BattleSceneObject_SetAnimation(object, 0x1F, -1);
        break;
    case 4:
        BattleScreenEffect_StartPrimary(1, 0x28, 0);
        number = (BattleDamageNumberEffect *)BattleDamage_SpawnNumber(
            damage, position.x + popup_offset_x, position.y + popup_offset_y, 4,
            0);
        BattleSceneObject_SetAnimation(object, 0x21, -1);
        break;
    case 5:
        BattleScreenEffect_StartPrimary(1, 0x28, 0);
        number = (BattleDamageNumberEffect *)BattleDamage_SpawnNumber(
            damage, position.x + popup_offset_x, position.y + popup_offset_y, 4,
            0);
        BattleSceneObject_SetAnimation(object, 0x21, -1);
        break;
    }

    if (number != 0) {
        if (object->actor_id == BATTLE_ACTOR_MARIO) {
            number->party_style = 0x0C;
            number->digit_offset = 8;
        }
        if (object->actor_id == BATTLE_ACTOR_LUIGI) {
            number->party_style = 0x0D;
            number->digit_offset = 0;
        }
    }

    if (status_record != 0 &&
        BattleActor_CanReceiveStatus(BattleActor_GetById(object->actor_id))) {
        BattleStatus_TryApply(
            BattleActor_GetPartySlot(object->actor_id),
            status_record->status_id,
            ((BattleContext *)gBattleContext)->status_duration,
            status_record->status_magnitude, status_record->status_chance);
    }
}

void BattleDamage_ApplyToEnemy(BattleSceneObject *object, int popup_offset_x,
                               int popup_offset_y, int damage,
                               int reaction_effect, int popup_effect,
                               u16 attach_number) {
    BattleEnemyActor *enemy =
        (BattleEnemyActor *)BattleActor_GetEnemySlot(object->actor_id);
    BattleDamageActorResource *actor_resource;
    BattlePosition position;

    if (enemy->actor.current_hp <= 0) {
        return;
    }
    if (damage < 1) {
        damage = 1;
    }
    if (damage > BATTLE_DAMAGE_MAX) {
        damage = BATTLE_DAMAGE_MAX;
    }

    if (!enemy->state_flag_bits.damage_immune) {
        BattleActor_ApplyDamage(object, damage);
    }
    if (!enemy->state_flag_bits.damage_immune) {
        s16 saved_animation = object->previous_base_animation_id;

        if (object->resource->resource_id != BATTLE_DAMAGE_SPECIAL_RESOURCE_1 &&
            object->resource->resource_id != BATTLE_DAMAGE_SPECIAL_RESOURCE_2) {
            enemy->reaction_state.impact_offset_x += object->property_102;
            enemy->reaction_state.impact_offset_y += object->property_103;
            object->property_102 = 0;
            object->property_103 = 0;
        }
        BattleSceneObject_SetAnimation(object, enemy->hit_animation_id, -1);
        if (BattleActor_GetById(object->actor_id)->flag_bits.hit_locked) {
            object->previous_base_animation_id = saved_animation;
        }
    }

    BattleDamage_StartEnemyReaction(object, reaction_effect);
    if (enemy->state_flag_bits.damage_immune) {
        return;
    }

    BattlePosition_StoreViewRelative(
        &position, object->x, (s16)(object->y - object->z),
        (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
        object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model);

    actor_resource = (BattleDamageActorResource *)enemy->actor.resource_slot;
    if (actor_resource == 0 ||
        ((u32)(actor_resource->popup_control->flags << 30) >> 31) == 0) {
        BattleDamage_SpawnNumber(damage, position.x + popup_offset_x,
                                 position.y + popup_offset_y, popup_effect,
                                 attach_number ? object->actor_id : 0);
    }

    if (reaction_effect == BATTLE_ENEMY_REACTION_EFFECT_NORMAL) {
        BattleModelEffect_Spawn(0x10, object, (s16)popup_offset_x,
                                (s16)popup_offset_y, 0, BATTLE_DAMAGE_SCALE);
        BattleSpriteEffect_Spawn(0x0A, (s16)(position.x + popup_offset_x),
                                 (s16)(position.y + popup_offset_y), position.z,
                                 BATTLE_DAMAGE_SCALE);
    }
    if (reaction_effect == BATTLE_ENEMY_REACTION_EFFECT_ALTERNATE) {
        BattleModelEffect_Spawn(0x12, object, (s16)popup_offset_x,
                                (s16)popup_offset_y, 0, BATTLE_DAMAGE_SCALE);
        BattleSpriteEffect_Spawn(0x0B, (s16)(position.x + popup_offset_x),
                                 (s16)(position.y + popup_offset_y), position.z,
                                 BATTLE_DAMAGE_SCALE);
    }
}
