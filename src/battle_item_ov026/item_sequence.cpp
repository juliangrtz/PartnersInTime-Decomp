#include "item_internal.h"

static inline UsableItemRecord *Overlay26Item_GetRecord(u16 item)
{
    if ((item & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG)
        return 0;
    return &gItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline unsigned Overlay26Item_GetKind(const UsableItemRecord *item)
{
    return (item->effect_flags << 1) >> 25;
}

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

void Overlay26Attack_UpdateEntry(Overlay26ItemUser *user)
{
    BattleSceneObject *actor = user->party.actor.scene_object;
    BattleSceneObject *object = BattleSceneObject_GetById(40);
    BattleModel *model = BattleSceneObject_GetActiveModel(actor);
    BattleSceneObject_GetActiveModel(object);
    UsableItemRecord *item = Overlay26Item_GetRecord(user->item);
    switch (gOverlay26AttackState->phase) {
    case 0:
        if (!Overlay26Attack_IsResourceLoadPending()) {
            if (Overlay26Item_GetKind(item) == 2)
                gOverlay26AttackState->phase = 3;
            else {
                BattleEntity_BindResource(actor->actor_id, 53);
                BattleSceneObject_SetAnimation(actor, 0, -1);
                BattleSceneObject_SetStateFlags(actor, 1, 0);
                ++gOverlay26AttackState->phase;
            }
        }
        break;
    case 1:
        if ((u8)model->flag_bits.unknown_09) {
            BattleEntity_BindResource(40, 52);
            BattleSceneObject_SetAnimation(object, item->unknown_08[1], -1);
            s16 position[4];
            Overlay26Attack_GetPhasePosition(position, 0, &user->party, 1);
            BattleSceneObject_AdjustPosition(object, position[0] - object->x, position[1] - object->y,
                                             position[2] - object->z);
            object->flags.bits.unk_20_23 = 2;
            gOverlay26AttackState->timer = 0;
            BattleMotion_StartAppear(object, 3, 16);
            ++gOverlay26AttackState->phase;
        }
        break;
    case 2:
        if (!BattleSceneObject_IsAnimationChannelActive(object, 3))
            ++gOverlay26AttackState->phase;
        break;
    case 3:
        gOverlay26AttackState->timer = 0;
        gOverlay26AttackState->phase = gOverlay26AttackState->timer;
        switch (Overlay26Item_GetKind(item)) {
        case 0:
        case 3:
            func_ov026_020c57d0(user);
            break;
        case 1:
        case 4:
        case 5:
        case 6:
            func_ov026_020c49ac(user);
            break;
        case 2:
            func_ov026_020c3850(user);
            break;
        }
        break;
    }
}

void Overlay26Attack_LaunchItem(Overlay26ItemUser *user, int actor_id, int item_id, int velocity)
{
    BattleSceneObject_GetById(actor_id);
    BattleSceneObject *item = BattleSceneObject_GetById(item_id);
    BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
    s16 position[4];
    Overlay26Attack_GetActorPosition(position, actor, 1);
    int duration = BattleSceneObject_StartVerticalMotionTo(item, 3, velocity, -120, position[2], 1);
    BattleSceneObject_MoveTo(item, 2, position[0], position[1], position[2], duration);
    BattleSound_Play(117, 0, 0, 0);
}

void Overlay26Attack_LoadResource(u16 resource, int size, int resource_index, int resource_id, int compressed)
{
    BattleObjectData_AllocateLoadBuffer(resource, size);
    BattleObjectData_ConfigureLoad(resource, resource_index, compressed);
    BattleObjectData_EnsureLoaded(resource, resource_id);
}

void Overlay26Attack_PrepareResources(Overlay26ItemUser *user)
{
    BattlePartyActor *target =
        (BattlePartyActor *)BattleActor_GetPartySlot((u16)user->party.actor.target_actor_id);
    UsableItemRecord *item = Overlay26Item_GetRecord(user->item);
    Overlay26Attack_LoadResource(52, 16384, 2432, 0xc2000006, 1);
    Overlay26Attack_LoadResource(56, 6144, 2448, 0xc2000031, 0);
    Overlay26Attack_LoadResource(53, 65536, 2464, data_ov026_020c64c8[user->party.formation_index], 1);
    gOverlay26AttackState->resource_id = 53;
    if (!Overlay26Attack_IsValidPartnerTarget(&user->party)) {
        const u16 first_baby_offset = (u16)-2;
        if ((u16)(target->formation_index + first_baby_offset) <= 1) {
            u16 actor_id = user->party.actor.target_actor_id;
            if (actor_id >= 58)
                target = (BattlePartyActor *)BattleActor_GetPartySlot((u16)(actor_id - 2));
        }
        Overlay26Attack_LoadResource(54, 65536, 2528, data_ov026_020c64c8[target->formation_index], 1);
        gOverlay26AttackState->resource_id = 54;
    } else {
        switch (Overlay26Item_GetKind(item)) {
        case 1:
        case 4:
        case 5:
        case 6:
            BattlePartyActor *partner =
                (BattlePartyActor *)BattleActor_GetPartySlot(user->party.linked_object_id);
            Overlay26Attack_LoadResource(54, 65536, 2528, data_ov026_020c64c8[partner->formation_index], 1);
            gOverlay26AttackState->resource_id = 54;
            break;
        }
    }
    BattlePartyActor *original_target =
        (BattlePartyActor *)BattleActor_GetPartySlot((u16)user->party.actor.target_actor_id);
    if ((original_target->formation_index & ~1) == 4) {
        if (!BattleActor_CanReceiveStatus(BattleActor_GetById((u16)user->party.actor.target_actor_id))) {
            Overlay26Attack_LoadResource(55, 65536, 2592,
                                         data_ov026_020c6384[original_target->formation_index & 1], 1);
            gOverlay26AttackState->resource_id = 55;
        }
    }
}
}
