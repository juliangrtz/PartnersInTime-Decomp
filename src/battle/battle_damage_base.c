#include <game/battle_actor.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

int BattleDamage_CalculateBase(int attacker_id, int defender_id, int scale_q8) {
    BattleActor *attacker = BattleActor_GetById(attacker_id);
    BattleActor *defender = BattleActor_GetById(defender_id);
    s32 scaled_power = attacker->power * scale_q8;
    s32 damage = _s32_div_f((attacker->flags & 0x7F) * scaled_power,
                            defender->defense);

    damage = (damage + 128) / 256;
    if (damage > 999) {
        damage = 999;
    }
    return damage;
}
