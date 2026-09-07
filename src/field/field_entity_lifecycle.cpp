#include <game/field_entity_lifecycle.h>
#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
#include <game/battle_scene.h>
extern "C" {
#include <game/heap.h>
extern FieldEntity *FieldEntity_InitializeB(FieldEntity *);
extern FieldEntity *FieldEntity_SetVTableA(FieldEntity *);
extern FieldEntityVTable data_ov000_020c1008;
extern FieldRenderObjectVTable data_ov000_020c1594;
extern void func_0202cbd4(void *, int, u32);

static inline FieldRuntimeEntity *InitEntity2D(FieldRuntimeEntity *entity)
{
    FieldEntity_InitializeB(&entity->base);
    *(FieldEntityVTable **)entity = &data_ov000_020c1008;
    func_0202cbd4((u8 *)entity + 4, 0, FIELD_ENTITY_2D_SIZE - 4);
    return entity;
}

FieldRuntimeEntity *FieldEntity2D_Init(FieldRuntimeEntity *entity) { return InitEntity2D(entity); }
FieldRuntimeEntity *FieldEntity2D_InitBase(FieldRuntimeEntity *entity) { return InitEntity2D(entity); }

FieldRuntimeEntity *FieldEntity2D_Destroy(FieldRuntimeEntity *entity)
{
    *(FieldEntityVTable **)entity = &data_ov000_020c1008;
    entity->base.release_renderers();
    FieldEntity_SetVTableA(&entity->base);
    return entity;
}

FieldRuntimeEntity *FieldEntity2D_Delete(FieldRuntimeEntity *entity)
{
    *(FieldEntityVTable **)entity = &data_ov000_020c1008;
    entity->base.release_renderers();
    FieldEntity_SetVTableA(&entity->base);
    GameHeap_Delete(entity);
    return entity;
}

FieldRuntimeEntity *FieldEntity2D_DestroyBase(FieldRuntimeEntity *entity)
{
    *(FieldEntityVTable **)entity = &data_ov000_020c1008;
    entity->base.release_renderers();
    FieldEntity_SetVTableA(&entity->base);
    return entity;
}

void FieldEntity_ResetLocomotionParameters(FieldRuntimeEntity *entity)
{
    entity->locomotion.starting_speed = entity->initial_locomotion.starting_speed;
    entity->locomotion.acceleration = entity->initial_locomotion.acceleration;
    entity->locomotion.maximum_speed = entity->initial_locomotion.maximum_speed;
    entity->locomotion.deceleration = entity->initial_locomotion.deceleration;
    entity->locomotion.reverse_deceleration = entity->initial_locomotion.reverse_deceleration;
    entity->locomotion.turn_speed_limit = entity->initial_locomotion.turn_speed_limit;
    entity->movement_speed = 0;
}

void FieldEntity_SetLocomotionParameters(FieldRuntimeEntity *entity, fx32 starting_speed, fx32 acceleration,
                       fx32 maximum_speed, fx32 deceleration, fx32 reverse_deceleration, fx32 turn_speed_limit)
{
    if (starting_speed != -1) entity->locomotion.starting_speed = starting_speed;
    if (acceleration != -1) entity->locomotion.acceleration = acceleration;
    if (maximum_speed != -1) entity->locomotion.maximum_speed = maximum_speed;
    if (deceleration != -1) entity->locomotion.deceleration = deceleration;
    if (reverse_deceleration != -1) entity->locomotion.reverse_deceleration = reverse_deceleration;
    if (turn_speed_limit != -1) entity->locomotion.turn_speed_limit = turn_speed_limit;
    entity->movement_speed = 0;
    entity->movement_velocity_x = 0;
    entity->movement_velocity_y = 0;
    entity->unknown_134 = 0;
}

void FieldEntity2D_AllocateRenderer(FieldRuntimeEntity *entity)
{
    FieldAnimationRenderer *renderer = (FieldAnimationRenderer *)GameHeap_New(
        sizeof(FieldAnimationRenderer), entity->base.property_00a_bits.heap, 0, 1);
    if (renderer) renderer = FieldAnimationRenderer_InitBase(renderer);
    entity->render_object = (FieldRenderObject *)renderer;
}

FieldAnimationRenderer *FieldAnimationRenderer_Delete(FieldAnimationRenderer *renderer)
{
    *(FieldRenderObjectVTable **)renderer = &data_ov000_020c1594;
    BattleModelController_DestroyBase((BattleModel *)renderer);
    GameHeap_Delete(renderer);
    return renderer;
}

void FieldEntity2D_ReleaseRenderer(FieldRuntimeEntity *entity)
{
    if (entity->render_object) {
        entity->saved_animation_frame = entity->render_object->animation_id;
        if (entity->render_object->state_flag_bits.render_linked)
            entity->render_object->stop();
        if (entity->render_object->texture_offsets) {
            GameHeap_DeleteArray((void *)entity->render_object->texture_offsets);
            entity->render_object->texture_offsets = 0;
        }
        if (entity->render_object) entity->render_object->delete_self();
        entity->render_object = 0;
        entity->saved_presentation_flags &= ~0x40;
    }
}
}
