#include <game/field_presentation.h>
extern "C" {

void FieldEntity3D_BindBoundsResource(FieldRuntimeEntity *entity, const u16 *resource)
{
    entity->bounds_resource = resource;
    if (resource) {
        entity->body_bounds = (const FieldInteractionBounds *)((const u8 *)resource + resource[0] + 1);
        entity->navigation_bounds = (const FieldInteractionBounds *)((const u8 *)resource + resource[1] + 1);
        entity->interaction_bounds = (const FieldInteractionBounds *)((const u8 *)resource + resource[2] + 1);
        entity->body_bounds_lookup = (const u8 *)resource + resource[3] + 1;
        entity->navigation_bounds_lookup = (const u8 *)resource + resource[4] + 1;
        entity->animation_bounds =
            (const FieldAnimationBoundsIndex *)((const u8 *)resource + resource[5] + 1);
        entity->bounds_animation_count = *((const u8 *)resource + resource[5]);
    } else {
        entity->body_bounds = 0;
        entity->navigation_bounds = 0;
        entity->interaction_bounds = 0;
        entity->body_bounds_lookup = 0;
        entity->navigation_bounds_lookup = 0;
        entity->animation_bounds = 0;
        entity->bounds_animation_count = 0;
    }
    entity->base.update_bounds();
}
}
