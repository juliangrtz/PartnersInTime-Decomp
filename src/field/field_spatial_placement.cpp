extern "C" {
#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
}
#include <game/field_entity_motion.h>
#include <game/field_presentation.h>
extern "C" {
void FieldEntity3D_SetPosition(FieldRuntimeEntity *entity, fx32 x, fx32 y, fx32 z)
{
    entity->position_x = x;
    entity->position_y = y;
    entity->position_z = z;
    entity->previous_position_x = x;
    entity->previous_position_y = y;
    entity->previous_position_z = z;
    entity->runtime_flag_bits.vertical_sync_dirty = 1;
    entity->support_entity_index = -1;
    entity->unknown_4f0 = 0;
}

void FieldEntity_FaceTarget(FieldRuntimeEntity *entity, FieldRuntimeEntity *target)
{
    u16 angle =
        FX_Atan2Idx(target->position_y - entity->position_y, target->position_x - entity->position_x) +
        0x4000;
    u16 direction = (angle + 0x1000) / 0x2000;
    FieldEntity_SetFacingDirection(entity, 0, direction, 1);
}

void FieldEntity3D_UpdateSupportClearance(FieldRuntimeEntity *entity)
{
    if (entity->auxiliary_render_object) {
        entity->support_clearance = entity->relative_height;
        FieldRuntimeEntity *support = entity->support_entity;
        if (support) {
            fx32 clearance = entity->position_z - (support->position_z + support->body_vertical_extent);
            if (entity->support_clearance >= clearance)
                entity->support_clearance = clearance;
            else
                entity->support_entity = 0;
        }
    }
}
}
