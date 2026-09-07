#include <game/field_entity_lifecycle.h>
#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
extern "C" {
#include <game/heap.h>
extern FieldEntityVTable data_ov000_020c10b0;
extern void func_0202cbd4(void *, int, u32);

static inline FieldRuntimeEntity *InitEntity3D(FieldRuntimeEntity *entity)
{
    FieldEntity2D_Init(entity);
    *(FieldEntityVTable **)entity = &data_ov000_020c10b0;
    func_0202cbd4((u8 *)entity + 4, 0, FIELD_ENTITY_3D_SIZE - 4);
    return entity;
}

FieldRuntimeEntity *FieldEntity3D_Init(FieldRuntimeEntity *entity) { return InitEntity3D(entity); }
FieldRuntimeEntity *FieldEntity3D_InitBase(FieldRuntimeEntity *entity) { return InitEntity3D(entity); }

FieldRuntimeEntity *FieldEntity3D_Destroy(FieldRuntimeEntity *entity)
{
    *(FieldEntityVTable **)entity = &data_ov000_020c10b0;
    FieldEntity2D_DestroyBase(entity);
    return entity;
}

FieldRuntimeEntity *FieldEntity3D_Delete(FieldRuntimeEntity *entity)
{
    *(FieldEntityVTable **)entity = &data_ov000_020c10b0;
    FieldEntity2D_DestroyBase(entity);
    GameHeap_Delete(entity);
    return entity;
}

FieldRuntimeEntity *FieldEntity3D_DestroyBase(FieldRuntimeEntity *entity)
{
    *(FieldEntityVTable **)entity = &data_ov000_020c10b0;
    FieldEntity2D_DestroyBase(entity);
    return entity;
}

void FieldEntity3D_ResetMotionParameters(FieldRuntimeEntity *entity)
{
    FieldEntity_ResetLocomotionParameters(entity);
    entity->default_vertical_launch_velocity = entity->initial_vertical_launch_velocity;
    entity->default_gravity = entity->initial_gravity;
    entity->terminal_fall_velocity = entity->initial_terminal_fall_velocity;
    entity->unknown_354 = 0;
}

void FieldEntity_SetVerticalParameters(FieldRuntimeEntity *entity, fx32 launch_velocity, fx32 gravity, fx32 terminal_velocity)
{
    if (launch_velocity != -1) entity->default_vertical_launch_velocity = launch_velocity;
    if (gravity != -1) entity->default_gravity = gravity;
    if (terminal_velocity) entity->terminal_fall_velocity = terminal_velocity;
    entity->unknown_354 = 0;
}

void FieldEntity3D_AllocateRenderers(FieldRuntimeEntity *entity)
{
    FieldEntity2D_AllocateRenderer(entity);
    if (!entity->saved_presentation_flag_bits.skip_auxiliary_renderer) {
        FieldAnimationRenderer *renderer = (FieldAnimationRenderer *)GameHeap_New(
            sizeof(FieldAnimationRenderer), entity->base.property_00a_bits.heap, 0, 1);
        if (renderer) renderer = FieldAnimationRenderer_InitBase(renderer);
        entity->auxiliary_render_object = (FieldRenderObject *)renderer;
    }
}

void FieldEntity3D_ReleaseRenderers(FieldRuntimeEntity *entity)
{
    FieldEntity2D_ReleaseRenderer(entity);
    if (entity->auxiliary_render_object) {
        if (entity->auxiliary_render_object->state_flag_bits.render_linked)
            entity->auxiliary_render_object->stop();
        if (entity->auxiliary_render_object->texture_offsets) {
            GameHeap_DeleteArray((void *)entity->auxiliary_render_object->texture_offsets);
            entity->auxiliary_render_object->texture_offsets = 0;
        }
        if (entity->auxiliary_render_object)
            entity->auxiliary_render_object->delete_self();
        entity->auxiliary_render_object = 0;
    }
}
}
