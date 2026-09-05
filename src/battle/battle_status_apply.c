#include <game/battle_effect.h>
#include <game/battle_enemy_data.h>
#include <game/battle_status.h>
#include <game/item.h>
#include <game/save_data.h>

extern int Random_NextModulo(int modulus);

enum BattleStatusApplyConstant {
    SAVE_EQUIPMENT_RECORD_SIZE = 0x24,
    SAVE_EQUIPMENT_EFFECT_OFFSET = 0x418,
    EQUIPMENT_EFFECT_STATUS_GUARD = 0x3003,
    EQUIPMENT_EFFECT_POWER_SPECIAL = 0x3024,
    BATTLE_STATUS_CHANCE_BASE = 100,
    BATTLE_STATUS_CHANCE_DOUBLE = 200,
    BATTLE_STATUS_CHANCE_HALF = 50,
    BATTLE_STATUS_POWER_SPECIAL_CHANCE = 101,
    BATTLE_STATUS_VISUAL_ACTIVE = 0x8000,
    BATTLE_STATUS_VISUAL_PARAMETER_MASK = 0x7F00,
    BATTLE_STAT_PERCENT_BASE = 100,
    BATTLE_STAT_ROUNDING_BIAS = 50,
    BATTLE_STAT_MAX = 999
};

enum BattleStatusResistance {
    BATTLE_STATUS_RESISTANCE_NORMAL = 0,
    BATTLE_STATUS_RESISTANCE_WEAK = 1,
    BATTLE_STATUS_RESISTANCE_STRONG = 2,
    BATTLE_STATUS_RESISTANCE_IMMUNE = 3
};

enum BattleStatusSound {
    BATTLE_STATUS_SOUND_POSITIVE_STAT = 107,
    BATTLE_STATUS_SOUND_NEGATIVE_STAT = 112,
    BATTLE_STATUS_SOUND_AILMENT_3 = 120,
    BATTLE_STATUS_SOUND_AILMENT_2 = 140,
    BATTLE_STATUS_SOUND_AILMENT_4 = 158,
    BATTLE_STATUS_SOUND_AILMENT_5 = 159
};

int BattleStatus_TryApply(BattleActor *actor, int status_id, s16 duration,
                          int magnitude_percent, int chance_percent) {
    u32 actor_id = actor->scene_object->actor_id;
    int is_enemy =
        actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
        actor_id < BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT;
    BattleStatusState *state;
    int equipment_effect;

    if (is_enemy) {
        BattleEnemyDataRequest *enemy_data =
            (BattleEnemyDataRequest *)actor->resource_slot;
        BattleEnemyStatRecord *stats;
        u32 resistance;

        if (enemy_data == 0) {
            return BATTLE_STATUS_NONE;
        }
        stats = enemy_data->stats;
        resistance = BATTLE_STATUS_RESISTANCE_NORMAL;

        switch (status_id) {
        case BATTLE_STATUS_AILMENT_1:
            return BATTLE_STATUS_NONE;
        case BATTLE_STATUS_AILMENT_2:
            resistance = stats->resistance_bits.status_2_resistance;
            break;
        case BATTLE_STATUS_AILMENT_3:
            resistance = stats->resistance_bits.status_3_resistance;
            break;
        case BATTLE_STATUS_AILMENT_4:
        case BATTLE_STATUS_AILMENT_5:
            resistance = stats->resistance_bits.status_4_5_resistance;
            break;
        case BATTLE_STATUS_POWER_CHANGE:
        case BATTLE_STATUS_DEFENSE_CHANGE:
        case BATTLE_STATUS_SPEED_CHANGE:
            resistance = stats->resistance_bits.status_6_8_resistance;
            break;
        }

        switch (resistance) {
        case BATTLE_STATUS_RESISTANCE_NORMAL:
            break;
        case BATTLE_STATUS_RESISTANCE_WEAK:
            chance_percent = chance_percent * BATTLE_STATUS_CHANCE_DOUBLE /
                             BATTLE_STATUS_CHANCE_BASE;
            break;
        case BATTLE_STATUS_RESISTANCE_STRONG:
            chance_percent = chance_percent * BATTLE_STATUS_CHANCE_HALF /
                             BATTLE_STATUS_CHANCE_BASE;
            break;
        case BATTLE_STATUS_RESISTANCE_IMMUNE:
            return BATTLE_STATUS_NONE;
        }
    }

    if (chance_percent <= Random_NextModulo(BATTLE_STATUS_CHANCE_BASE)) {
        return BATTLE_STATUS_NONE;
    }
    {
        int play_stat_sound = 1;

        if (!is_enemy) {
            equipment_effect = *(gSaveData +
                                 SAVE_EQUIPMENT_RECORD_SIZE * (u8) *
                                     (u16 *)actor->resource_slot +
                                 SAVE_EQUIPMENT_EFFECT_OFFSET) |
                               ITEM_ID_BADGE_TAG;

            if (equipment_effect == EQUIPMENT_EFFECT_STATUS_GUARD) {
                switch (status_id) {
                case BATTLE_STATUS_AILMENT_1:
                case BATTLE_STATUS_AILMENT_2:
                case BATTLE_STATUS_AILMENT_3:
                case BATTLE_STATUS_AILMENT_4:
                case BATTLE_STATUS_AILMENT_5:
                    return BATTLE_STATUS_NONE;
                case BATTLE_STATUS_POWER_CHANGE:
                    if (magnitude_percent < actor->power_change.kind) {
                        return BATTLE_STATUS_NONE;
                    }
                    break;
                case BATTLE_STATUS_DEFENSE_CHANGE:
                    if (magnitude_percent < actor->defense_change.kind) {
                        return BATTLE_STATUS_NONE;
                    }
                    break;
                case BATTLE_STATUS_SPEED_CHANGE:
                    if (magnitude_percent < actor->speed_change.kind) {
                        return BATTLE_STATUS_NONE;
                    }
                    break;
                default:
                    return BATTLE_STATUS_NONE;
                }
            }

            else if (equipment_effect == EQUIPMENT_EFFECT_POWER_SPECIAL &&
                     status_id == BATTLE_STATUS_POWER_CHANGE &&
                     !((BattlePartyActor *)actor)
                          ->state_flags.status_bits.power_special_disabled) {
                if (chance_percent != BATTLE_STATUS_POWER_SPECIAL_CHANCE) {
                    return BATTLE_STATUS_NONE;
                }
                play_stat_sound = 0;
            }
        }

        switch (status_id) {
        case BATTLE_STATUS_AILMENT_1:
            state = &actor->ailment_group_1;
            if (state->kind >= 1) {
                return BATTLE_STATUS_NONE;
            }
            chance_percent = 1;
            state->kind = 1;
            state->visual_flags &= ~BATTLE_STATUS_VISUAL_PARAMETER_MASK;
            state->parameter_2 = 0;
            state->parameter_1 = 0;
            break;
        case BATTLE_STATUS_AILMENT_2:
            state = &actor->ailment_group_1;
            if (state->kind >= 2) {
                return BATTLE_STATUS_NONE;
            }
            state->kind = 2;
            state->visual_flags =
                (state->visual_flags & ~BATTLE_STATUS_VISUAL_PARAMETER_MASK) |
                (60 << 8);
            state->parameter_2 = 20;
            state->parameter_1 = 2;
            chance_percent = 1;
            BattleSound_Play(BATTLE_STATUS_SOUND_AILMENT_2, 0, 0, 0);
            break;
        case BATTLE_STATUS_AILMENT_3:
            state = &actor->ailment_group_1;
            if (state->kind >= 3) {
                return BATTLE_STATUS_NONE;
            }
            state->kind = 3;
            state->visual_flags =
                (state->visual_flags & ~BATTLE_STATUS_VISUAL_PARAMETER_MASK) |
                (40 << 8);
            state->parameter_2 = 30;
            state->parameter_1 = 2;
            state->counter = 8;
            chance_percent = 1;
            BattleSound_Play(BATTLE_STATUS_SOUND_AILMENT_3, 0, 0, 0);
            break;
        case BATTLE_STATUS_AILMENT_4:
            state = &actor->ailment_group_2;
            if (state->kind >= 1) {
                return BATTLE_STATUS_NONE;
            }
            chance_percent = 1;
            state->kind = chance_percent;
            state->visual_flags =
                (state->visual_flags & ~BATTLE_STATUS_VISUAL_PARAMETER_MASK) |
                (20 << 8);
            state->parameter_2 = 10;
            state->parameter_1 = 4;
            state->counter = 5;
            BattleSound_Play(BATTLE_STATUS_SOUND_AILMENT_4, 0, 0, 0);
            break;
        case BATTLE_STATUS_AILMENT_5:
            state = &actor->ailment_group_2;
            if (state->kind >= 2) {
                return BATTLE_STATUS_NONE;
            }
            state->kind = 2;
            state->visual_flags =
                (state->visual_flags & ~BATTLE_STATUS_VISUAL_PARAMETER_MASK) |
                (10 << 8);
            state->parameter_2 = 5;
            state->parameter_1 = 4;
            state->counter = 10;
            chance_percent = 1;
            BattleSound_Play(BATTLE_STATUS_SOUND_AILMENT_5, 0, 0, 0);
            break;
        case BATTLE_STATUS_POWER_CHANGE:
            state = &actor->power_change;
            if (magnitude_percent == 0) {
                return BATTLE_STATUS_NONE;
            }
            state->kind = magnitude_percent;
            state->visual_flags =
                (state->visual_flags & ~BATTLE_STATUS_VISUAL_PARAMETER_MASK) |
                (10 << 8);
            state->parameter_2 = 10;
            state->parameter_1 = 4;
            actor->power = (actor->base_power *
                                (magnitude_percent + BATTLE_STAT_PERCENT_BASE) +
                            BATTLE_STAT_ROUNDING_BIAS) /
                           BATTLE_STAT_PERCENT_BASE;
            if (actor->power < 0) {
                actor->power = 0;
            }
            if (actor->power > BATTLE_STAT_MAX) {
                actor->power = BATTLE_STAT_MAX;
            }
            chance_percent = 1;
            if (play_stat_sound) {
                if (magnitude_percent < 0) {
                    BattleSound_Play(BATTLE_STATUS_SOUND_NEGATIVE_STAT, 0, 0,
                                     0);
                }
                if (magnitude_percent > 0) {
                    BattleSound_Play(BATTLE_STATUS_SOUND_POSITIVE_STAT, 0, 0,
                                     0);
                }
            }
            break;
        case BATTLE_STATUS_DEFENSE_CHANGE:
            state = &actor->defense_change;
            if (magnitude_percent == 0) {
                return BATTLE_STATUS_NONE;
            }
            state->kind = magnitude_percent;
            state->visual_flags =
                (state->visual_flags & ~BATTLE_STATUS_VISUAL_PARAMETER_MASK) |
                (10 << 8);
            state->parameter_2 = 10;
            state->parameter_1 = 4;
            actor->defense = (actor->base_defense * (magnitude_percent +
                                                     BATTLE_STAT_PERCENT_BASE) +
                              BATTLE_STAT_ROUNDING_BIAS) /
                             BATTLE_STAT_PERCENT_BASE;
            if (actor->defense < 0) {
                actor->defense = 0;
            }
            if (actor->defense > BATTLE_STAT_MAX) {
                actor->defense = BATTLE_STAT_MAX;
            }
            chance_percent = 1;
            if (magnitude_percent < 0) {
                BattleSound_Play(BATTLE_STATUS_SOUND_NEGATIVE_STAT, 0, 0, 0);
            }
            if (magnitude_percent > 0) {
                BattleSound_Play(BATTLE_STATUS_SOUND_POSITIVE_STAT, 0, 0, 0);
            }
            break;
        case BATTLE_STATUS_SPEED_CHANGE:
            state = &actor->speed_change;
            if (magnitude_percent == 0) {
                return BATTLE_STATUS_NONE;
            }
            state->kind = magnitude_percent;
            state->visual_flags =
                (state->visual_flags & ~BATTLE_STATUS_VISUAL_PARAMETER_MASK) |
                (10 << 8);
            state->parameter_2 = 10;
            state->parameter_1 = 4;
            actor->speed = (actor->base_speed *
                                (magnitude_percent + BATTLE_STAT_PERCENT_BASE) +
                            BATTLE_STAT_ROUNDING_BIAS) /
                           BATTLE_STAT_PERCENT_BASE;
            if (actor->speed < 0) {
                actor->speed = 0;
            }
            if (actor->speed > BATTLE_STAT_MAX) {
                actor->speed = BATTLE_STAT_MAX;
            }
            chance_percent = 1;
            if (magnitude_percent < 0) {
                BattleSound_Play(BATTLE_STATUS_SOUND_NEGATIVE_STAT, 0, 0, 0);
            }
            if (magnitude_percent > 0) {
                BattleSound_Play(BATTLE_STATUS_SOUND_POSITIVE_STAT, 0, 0, 0);
            }
            break;
        default:
            return BATTLE_STATUS_NONE;
        }

        state->duration = duration;
        state->visual_flags |= BATTLE_STATUS_VISUAL_ACTIVE;
        if (chance_percent != 0) {
            BattleStatus_SpawnEffect(actor_id, status_id);
        }
        return status_id;
    }
}
