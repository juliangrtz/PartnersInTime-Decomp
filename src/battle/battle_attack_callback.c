/* Install the next attack callback and reset its shared timer/flag. */
#include <game/battle_actor.h>

/* Attack allocations share these leading fields; later fields vary by attack. */
typedef struct BattleAttackControlPrefix {
    int timer;
    u8 unknown_04[12];
    u8 flags;
} BattleAttackControlPrefix;
typedef char BattleAttackControlPrefixSizeCheck[
    sizeof(BattleAttackControlPrefix) == 20 ? 1 : -1];

extern BattleAttackControlPrefix *data_ov002_020c0710;

void BattleAttack_SetCallback(BattlePartyActor *actor,
                             void (*callback)(BattlePartyActor *))
{
    data_ov002_020c0710->timer = 0;
    data_ov002_020c0710->flags &= ~1;
    actor->attack_callback = callback;
}
