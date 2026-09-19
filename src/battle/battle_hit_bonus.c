/*
 * Hit bonus roll (overlay 2, 0x020720FC-0x020722AC).
 *
 * Rolls the Stache-based bonus applied to a hit. The arithmetic widths and the
 * truncation points are load-bearing; see the reconstruction notes.
 */

#include <game/battle_actor.h>
#include <game/battle_damage.h>
#include <game/save_data.h>
#include <game/random.h>

extern const u8 data_ov002_020be8fc[];

int BattleParty_RollHitBonus(BattlePartyActor *user, BattleActor *target) {
    int user_term;
    int target_term;
    int chance;

    if (!target) {
        target = BattleActor_GetById((u16)user->actor.target_actor_id);
    }
    if (user->actor.unk_00e == 0) {
        return 0;
    }
    /* Preserve the native floating-point operations and each truncation.
     * Battle entry fills unk_00e from the saved Stache stat. */
    user_term = (int)(256.0 * user->actor.unk_00e);
    target_term = (int)(128.0 * (u32)target->flag_bits.level);
    chance = (int)(64.0 * (user_term - target_term) / 65536.0);
    if (user->actor.scene_object) {
        int member = data_ov002_020be8fc[user->formation_index];
        int badge = *(u8 *)(gSaveData + 36 * member + 1048) | 0x3000;

        /* Keep the native fallback block before the 0x300E special case. */
        if (badge != 0x300E) {
            if (badge == 0x3028) {
                chance = (int)(1152.0 * chance / 256.0);
            }
        } else {
            chance = (int)(640.0 * chance / 256.0);
        }
    }
    if (chance < 1) {
        chance = 1;
    } else if (chance > 99) {
        chance = 99;
    }
    return Random_NextModulo(100) < chance;
}
