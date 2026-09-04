#include <game/battle_actor.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

enum {
    BATTLE_DAMAGE_SCALE_ONE = 256,
    BATTLE_DAMAGE_ROUNDING_BIAS = BATTLE_DAMAGE_SCALE_ONE / 2,
    BATTLE_DAMAGE_MAX = 999
};

int BattleDamage_CalculateBase(int attacker_id, int defender_id, int scale_q8) {
    BattleActor *attacker = BattleActor_GetById(attacker_id);
    BattleActor *defender = BattleActor_GetById(defender_id);
    s32 scaled_power = attacker->power * scale_q8;
    u32 attacker_level = (u32)(attacker->flags << 25) >> 25;
    s32 damage = _s32_div_f(attacker_level * scaled_power,
                            defender->defense);

    damage = (damage + BATTLE_DAMAGE_ROUNDING_BIAS) / BATTLE_DAMAGE_SCALE_ONE;
    if (damage > BATTLE_DAMAGE_MAX) {
        damage = BATTLE_DAMAGE_MAX;
    }
    return damage;
}
