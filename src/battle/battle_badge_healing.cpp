/* Battle healing uses the item user's badge, even when healing another actor.
 * Preserve the integer rounding before clamping to the target's maximum HP. */
#include <game/battle_item_effect.h>
#include <game/item.h>
#include <game/battle_ai.h>
#include <game/save_state_transfer.h>

extern "C" int BattleItemEffect_ApplyBadgeBoost(BattleActor *target, int hp_after)
{
    BattleActor *user = BattleActor_GetPartySlot(*(u16 *)(gBattleContext + 0x20));
    u8 member = (u8)*(u16 *)user->resource_slot;
    int badge = ((SaveLiveTransferView *)gSaveData)->party[member].badge_id | ITEM_ID_BADGE_TAG;
    switch (badge) {
    case 0x3010:
        hp_after = target->current_hp + (200 * (hp_after - target->current_hp) + 50) / 100;
        if (target->max_hp < hp_after) {
            hp_after = target->max_hp;
        }
        break;
    case 0x3008:
        hp_after = target->current_hp + (150 * (hp_after - target->current_hp) + 50) / 100;
        if (target->max_hp < hp_after) {
            hp_after = target->max_hp;
        }
        break;
    }
    return hp_after;
}
