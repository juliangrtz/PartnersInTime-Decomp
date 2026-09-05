#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_damage.h>
#include <game/battle_global_properties.h>
#include <game/battle_scene.h>
#include <game/item.h>
#include <game/save_data.h>

enum BattleDamageObjectConstant {
    SAVE_EQUIPMENT_RECORD_SIZE = 0x24,
    SAVE_EQUIPMENT_EFFECT_OFFSET = 0x418,
    EQUIPMENT_EFFECT_ROUGH_BADGE = 0x301B,
    BATTLE_DAMAGE_SCALE_ONE = 0x100,
    BATTLE_DAMAGE_SCALE_MODE_ZERO = 0x126,
    BATTLE_DAMAGE_SCALE_MODE_ONE = 0x10C,
    BATTLE_DAMAGE_SCALE_ENEMY_DEFAULT = 0x6400,
    BATTLE_DAMAGE_ROUGH_BADGE_PERCENT = 140,
    BATTLE_DAMAGE_PERCENT_BASE = 100,
    BATTLE_DAMAGE_MAX = 999
};

int BattleDamage_CalculateByObject(u32 attacker_object_id,
                                   u32 defender_object_id) {
    u32 attacker_id =
        BattleSceneObject_GetById(attacker_object_id)->linked_actor_id;
    u32 defender_id =
        BattleSceneObject_GetById(defender_object_id)->linked_actor_id;
    BattleActor *attacker;
    BattleActor *defender;
    int damage;
    int equipment_condition;
    u32 active_actor_id;

    if ((attacker_id >= BATTLE_ACTOR_PARTY_FIRST &&
         attacker_id < BATTLE_ACTOR_PARTY_FIRST + BATTLE_ACTOR_PARTY_COUNT) ||
        (attacker_id >= BATTLE_ACTOR_ENEMY_FIRST &&
         attacker_id < BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT)) {
        attacker = BattleActor_GetById(attacker_id);
    } else {
        return 0;
    }

    if ((defender_id >= BATTLE_ACTOR_PARTY_FIRST &&
         defender_id < BATTLE_ACTOR_PARTY_FIRST + BATTLE_ACTOR_PARTY_COUNT) ||
        (defender_id >= BATTLE_ACTOR_ENEMY_FIRST &&
         defender_id < BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT)) {
        defender = BattleActor_GetById(defender_id);
    } else {
        return 0;
    }
    if (defender->damage_flag_bits.force_one_damage) {
        return 1;
    }

    damage = 0;
    equipment_condition = 0;
    active_actor_id = *(u16 *)(gBattleContext + BATTLE_GLOBAL_09_OFFSET);

    if (active_actor_id < BATTLE_ACTOR_PARTY_FIRST ||
        active_actor_id >=
            BATTLE_ACTOR_PARTY_FIRST + BATTLE_ACTOR_PARTY_COUNT) {
        if (active_actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
            active_actor_id <
                BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT) {
            if (attacker_id >= BATTLE_ACTOR_ENEMY_FIRST &&
                attacker_id <
                    BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT) {
                damage = attacker->damage_scale_q8;
                equipment_condition = 0;
                if (damage <= 0) {
                    damage = BATTLE_DAMAGE_SCALE_ENEMY_DEFAULT;
                }
            }

            if (attacker_id >= BATTLE_ACTOR_PARTY_FIRST &&
                attacker_id <
                    BATTLE_ACTOR_PARTY_FIRST + BATTLE_ACTOR_PARTY_COUNT) {
                equipment_condition = 1;
                switch (*(s16 *)(gBattleContext + BATTLE_GLOBAL_12_OFFSET)) {
                case 0:
                    switch (((BattlePartyActor *)attacker)->formation_index) {
                    case BATTLE_FORMATION_MARIO:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ZERO;
                        break;
                    case BATTLE_FORMATION_LUIGI:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ZERO;
                        break;
                    case BATTLE_FORMATION_BABY_MARIO:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ZERO;
                        break;
                    case BATTLE_FORMATION_BABY_LUIGI:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ZERO;
                        break;
                    case BATTLE_FORMATION_MARIO_CARRYING:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ZERO;
                        break;
                    case BATTLE_FORMATION_LUIGI_CARRYING:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ZERO;
                        break;
                    }
                    break;
                case 1:
                    switch (((BattlePartyActor *)attacker)->formation_index) {
                    case BATTLE_FORMATION_MARIO:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ONE;
                        break;
                    case BATTLE_FORMATION_LUIGI:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ONE;
                        break;
                    case BATTLE_FORMATION_BABY_MARIO:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ONE;
                        break;
                    case BATTLE_FORMATION_BABY_LUIGI:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ONE;
                        break;
                    case BATTLE_FORMATION_MARIO_CARRYING:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ONE;
                        break;
                    case BATTLE_FORMATION_LUIGI_CARRYING:
                        damage = BATTLE_DAMAGE_SCALE_MODE_ONE;
                        break;
                    }
                    break;
                }
            }

            damage = BattleDamage_ApplyEquipmentModifiers(
                attacker_id, defender_id, damage, equipment_condition);
        } else if (active_actor_id == 0) {
            switch (((BattlePartyActor *)attacker)->formation_index) {
            case BATTLE_FORMATION_MARIO:
                damage = BATTLE_DAMAGE_SCALE_ONE;
                break;
            case BATTLE_FORMATION_LUIGI:
                damage = BATTLE_DAMAGE_SCALE_ONE;
                break;
            case BATTLE_FORMATION_BABY_MARIO:
                damage = BATTLE_DAMAGE_SCALE_ONE;
                break;
            case BATTLE_FORMATION_BABY_LUIGI:
                damage = BATTLE_DAMAGE_SCALE_ONE;
                break;
            case BATTLE_FORMATION_MARIO_CARRYING:
                damage = BATTLE_DAMAGE_SCALE_ONE;
                break;
            case BATTLE_FORMATION_LUIGI_CARRYING:
                damage = BATTLE_DAMAGE_SCALE_ONE;
                break;
            }

            damage =
                BattleDamage_CalculateBase(attacker_id, defender_id, damage);
            {
                u8 *equipment_record =
                    gSaveData + SAVE_EQUIPMENT_RECORD_SIZE * (u8) *
                                    (u16 *)attacker->resource_slot;
                int equipment_effect =
                    equipment_record[SAVE_EQUIPMENT_EFFECT_OFFSET] |
                    ITEM_ID_BADGE_TAG;

                if (equipment_effect == EQUIPMENT_EFFECT_ROUGH_BADGE) {
                    damage = (damage * BATTLE_DAMAGE_ROUGH_BADGE_PERCENT + 50) /
                             BATTLE_DAMAGE_PERCENT_BASE;
                }
            }
        }
    }

    if (damage > BATTLE_DAMAGE_MAX) {
        damage = BATTLE_DAMAGE_MAX;
    }
    return damage;
}
