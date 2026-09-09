#include <game/field_party.h>
#include <game/field_auxiliary.h>

extern "C" {
void func_ov000_020a9d1c(FieldRuntimeEntity *, u8);

void FieldPartyEntity_CopyPartnerPlanarBounds(FieldPartyEntity *member)
{
    fx32 dx = member->partner->entity.position_x - member->entity.position_x;
    fx32 dy = member->partner->entity.position_y - member->entity.position_y;
    member->entity.body_min_x = member->partner->entity.body_min_x + dx;
    member->entity.body_max_x = member->partner->entity.body_max_x + dx;
    member->entity.body_min_y = member->partner->entity.body_min_y + dy;
    member->entity.body_max_y = member->partner->entity.body_max_y + dy;
    member->entity.navigation_min_x = member->partner->entity.navigation_min_x + dx;
    member->entity.navigation_max_x = member->partner->entity.navigation_max_x + dx;
    member->entity.navigation_min_y = member->partner->entity.navigation_min_y + dy;
    member->entity.navigation_max_y = member->partner->entity.navigation_max_y + dy;
}

void FieldPartyEntity_UpdateRenderers(FieldPartyEntity *member, u8 default_priority)
{
    func_ov000_020a9d1c(&member->entity, default_priority);
    for (int i = 0; i < 6; i++)
        if (member->auxiliaries[i])
            member->auxiliaries[i]->entity.base.update_overlap_priorities(default_priority);
}
}
