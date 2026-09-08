#include "item_internal.h"

extern "C" {
Overlay26PartyEffectSlot *Overlay26Attack_GetPartyEffectSlot(int actor_id)
{
    BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
    Overlay26PartyEffectSlot *slot = 0;
    switch (actor->formation_index) {
    case 0:
    case 4:
        slot = (Overlay26PartyEffectSlot *)(gBattleContext + 25976);
        break;
    case 1:
    case 5:
        slot = (Overlay26PartyEffectSlot *)(gBattleContext + 26004);
        break;
    case 2:
        if (*(s16 *)(gSaveData + 1368) == 2)
            slot = (Overlay26PartyEffectSlot *)(gBattleContext + 26032);
        else
            slot = (Overlay26PartyEffectSlot *)(gBattleContext + 25976);
        break;
    case 3:
        if (*(s16 *)(gSaveData + 1368) == 2)
            slot = (Overlay26PartyEffectSlot *)(gBattleContext + 26060);
        else
            slot = (Overlay26PartyEffectSlot *)(gBattleContext + 26004);
        break;
    }
    return slot;
}

void Overlay26Attack_SpawnPartyEffect(int actor_id)
{
    Overlay26PartyEffectSlot *slot = Overlay26Attack_GetPartyEffectSlot(actor_id);
    BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
    int offset = 0;
    if (*(s16 *)(gSaveData + 1368) == 2) {
        const u16 first_baby_offset = (u16)-2;
        if ((u16)(actor->formation_index + first_baby_offset) <= 1)
            offset = 106;
    }
    gOverlay26AttackState->completion_link = (void **)&slot->effect;
    BattleModelEffect_SpawnAttached(&slot->effect, 866, slot->object, (s16)(offset + 12), -8, -2, 384);
}
}
