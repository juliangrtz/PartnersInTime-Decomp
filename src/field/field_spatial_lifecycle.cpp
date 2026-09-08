#include <game/field_entity_lifecycle.h>
#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
extern "C" {
#include <game/heap.h>
extern FieldEntityVTable data_ov000_020c10b0;
extern void func_0202cbd4(void *, int, u32);

/* Keep constant channels visible through the compiler's one-level inline limit.
 * The original masks are signed 32-bit values extended to the 64-bit policy. */
#define SetCollisionChannel(entity, shift, policy) do { \
    (entity)->collision_policy &= ~(s32)(7u << (shift)); \
    if (policy) (entity)->collision_policy |= (s32)((u32)(policy) << (shift)); \
} while (0)

static inline FieldRuntimeEntity *InitializeSpatialPlacement(FieldRuntimeEntity *entity,
    int entity_index, const FieldSpawnRecord *spawn, int argument_3, int resource_set, u8 enabled)
{
    FieldEntity2D_InitPlacement(entity, entity_index, spawn, argument_3, resource_set, enabled);
    *(FieldEntityVTable **)entity = &data_ov000_020c10b0;
    func_0202cbd4(&entity->navigation_surfaces, 0, FIELD_ENTITY_3D_SIZE - FIELD_ENTITY_2D_SIZE);
    if (spawn) {
        bool subtype_enabled;
        s8 priorities;
        entity->position_z = (spawn->offset_z + 8 * spawn->cell_z) << 12;
        entity->field_state_flag_bits.contact_mode = spawn->flags.bits.contact_mode;
        entity->runtime_flag_bits.sync_horizontal = spawn->flags.bits.synchronize;
        entity->runtime_flag_bits.sync_vertical = entity->runtime_flag_bits.sync_horizontal;
        entity->runtime_flags |= 4;
        entity->collision_state_flag_bits.unknown_00 = spawn->flags.bits.collision_flag_00;
        entity->collision_state_flag_bits.unknown_01 = spawn->flags.bits.collision_flag_01;
        entity->collision_state_flag_bits.unknown_02 = spawn->flags.bits.collision_flags_02_03;
        entity->collision_state_flag_bits.unknown_03 = spawn->flags.bits.collision_flags_02_03;
        entity->collision_state_flag_bits.unknown_04 = spawn->flags.bits.collision_flag_04;
        entity->collision_state_flag_bits.unknown_05 = 0;
        entity->collision_state_flag_bits.unknown_06 = spawn->flags.bits.collision_flag_06;
        entity->collision_state_flag_bits.unknown_07 = 0;
        entity->collision_state_flags |= 0x10000;
        entity->collision_state_flags |= 0x20000;
        entity->field_state_flag_bits.ignore_navigation_obstacle = spawn->flags.bits.ignore_navigation_obstacle;
        if (spawn->flags.bits.auto_priorities) priorities = 31;
        else priorities = 0;
        entity->priority_flag_bits.auto_priority_mask = priorities;
        entity->runtime_flag_bits.alternate_collision_faces = spawn->flags.bits.alternate_collision_faces;
        entity->field_state_flags |= 8;
        entity->field_state_flags &= ~16;
        entity->field_state_flag_bits.turn_to_interactor = spawn->flags.bits.turn_to_interactor;
        entity->field_state_flags |= 0x20000;
        entity->field_state_flag_bits.shadow_style = spawn->flags.bits.shadow_style;
        entity->field_state_flag_bits.shadow_enabled = entity->field_state_flag_bits.shadow_style != 0;
        entity->collision_state_flag_bits.unknown_18_19 = spawn->flags.bits.collision_mode;
        subtype_enabled = true;
        switch (entity->base.property_00a_bits.subtype) {
        case 0:
        case 1:
            break;
        default:
            subtype_enabled = false;
            break;
        }
        entity->runtime_flag_bits.unknown_21 = subtype_enabled;
        entity->runtime_flag_bits.unknown_22 = 0;
        entity->field_state_flag_bits.track_ground = spawn->flags.bits.track_ground;
        entity->collision_flag_bytes.saved = entity->collision_flag_bytes.current;
        entity->field_state_flag_bits.unknown_21_25 = 15;
        entity->unknown_3a0_bits.unknown_26_31 = 63;
        entity->contact_direction_flags.enabled_mask = 63;
        entity->unknown_3d4 = spawn->unknown_12;
        entity->unknown_3c8_bits.unknown_00 = 1;
    }
    entity->support_entity_index = -1;
    entity->previous_support_entity_index = -1;
    entity->initial_vertical_launch_velocity = 21496;
    entity->initial_gravity = 1296;
    entity->initial_terminal_fall_velocity = 0;
    entity->unknown_2e4[0] = -1;
    entity->unknown_2e4[1] = -1;
    entity->unknown_2e4[2] = -1;
    entity->unknown_2e4[3] = -1;
    entity->field_state_flags |= 0x4000;
    entity->base.reset_motion_parameters();
    entity->unknown_370 = 10;
    entity->collision_category_mask = (entity->collision_category_mask & ~0x7FF) | 0x4FF;
    SetCollisionChannel(entity, 0, 7);
    SetCollisionChannel(entity, 3, 7);
    SetCollisionChannel(entity, 6, 7);
    SetCollisionChannel(entity, 9, 7);
    SetCollisionChannel(entity, 12, 7);
    SetCollisionChannel(entity, 15, 7);
    SetCollisionChannel(entity, 18, 7);
    SetCollisionChannel(entity, 21, 7);
    SetCollisionChannel(entity, 24, 0);
    SetCollisionChannel(entity, 27, 0);
    SetCollisionChannel(entity, 30, 7);
    entity->roaming_flag_bits.clamp_to_boundary = 1;
    entity->roaming_flag_bits.unknown_08_13 = 63;
    entity->roaming_flag_bits.unknown_14_19 = 63;
    entity->unknown_3e4_bits.unknown_10_15 = 0;
    entity->unknown_3e4_bits.unknown_16_21 = 0;
    return entity;
}

FieldRuntimeEntity *FieldEntity3D_InitPlacement(FieldRuntimeEntity *entity,
    int entity_index, const FieldSpawnRecord *spawn, int argument_3, int resource_set, u8 enabled)
{
    return InitializeSpatialPlacement(entity, entity_index, spawn, argument_3, resource_set, enabled);
}

FieldRuntimeEntity *FieldEntity3D_InitPlacementBase(FieldRuntimeEntity *entity,
    int entity_index, const FieldSpawnRecord *spawn, int argument_3, int resource_set, u8 enabled)
{
    return InitializeSpatialPlacement(entity, entity_index, spawn, argument_3, resource_set, enabled);
}

#undef SetCollisionChannel

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
    entity->vertical_velocity = 0;
}

void FieldEntity_SetVerticalParameters(FieldRuntimeEntity *entity, fx32 launch_velocity, fx32 gravity, fx32 terminal_velocity)
{
    if (launch_velocity != -1) entity->default_vertical_launch_velocity = launch_velocity;
    if (gravity != -1) entity->default_gravity = gravity;
    if (terminal_velocity) entity->terminal_fall_velocity = terminal_velocity;
    entity->vertical_velocity = 0;
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
