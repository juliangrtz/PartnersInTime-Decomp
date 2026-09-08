#include "pocket_chomp_internal.h"

extern "C" {
void PocketChompSupport_Initialize(PocketChompSupport *support, s16 index, s8 mode, int resource)
{
    support->object = BattleSceneObject_GetById((u16)(index + 44));
    support->variant_object = 0;
    support->actor = (BattlePartyActor *)BattleActor_GetPartySlot((u16)((mode & 1) + 58));
    support->linked_object = BattleSceneObject_GetById(43);
    support->resource_index = resource;
    support->timer = 0;
    support->bits.mode = mode;
    support->bits.variant = index;
    support->bits.phase = 0;
    support->action_flags.unknown_00 = 0;
    support->object->effect_anchor_z = -8 * (index + 1);
}

void PocketChompSupport_ClearAnchor(PocketChompSupport *support)
{
    if (support->object) {
        func_ov018_020c2e50(support, -1);
        support->object->effect_anchor_z = 0;
    }
}

void PocketChompSupport_Hide(PocketChompSupport *support)
{
    if (support->object) {
        func_ov018_020c2e50(support, -1);
        support->bits.phase = 0;
    }
}
}
