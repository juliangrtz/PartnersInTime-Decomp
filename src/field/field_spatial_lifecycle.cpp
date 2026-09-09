#include <game/field_entity_lifecycle.h>
#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
#include <game/field_resources.h>
#include <game/field_presentation.h>
#include <nitro/fx_atan.h>
#include <game/battle_scene.h>
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

void FieldEntity3D_ConfigureAuxiliaryResources(FieldRuntimeEntity *entity, const FieldPrimaryResource *primary,
                        const FieldSecondaryResource *secondary, FieldPaletteResource *palette)
{
    FieldResourceDescriptor descriptor;
    entity->auxiliary_primary_resource = primary;
    entity->auxiliary_secondary_resource = secondary;
    entity->auxiliary_palette_resource = palette;
    if (entity->auxiliary_render_object) {
        BattleModel_InitDescriptor(&descriptor, 0);
        int size = BattleModel_GetScreenTextureConversionSize(
            (u8)entity->base.property_00a_bits.property_00a_unknown_01, 1,
            (const GameGraphicsResource *)primary->animation);
        if (size)
            descriptor.conversion_buffer = GameHeap_NewArray(size, entity->base.property_00a_bits.heap, 0, 1);
        descriptor.flags.resource_set = 1;
        descriptor.flags.unknown_23 = 0;
        descriptor.animation = primary->animation;
        descriptor.graphics = primary->graphics;
        descriptor.graphics_size = primary->bits.graphics_size;
        descriptor.primary_id = primary->id;
        /* The original writes this flag twice across primary-ID setup. */
        descriptor.resource_flags.alternate = 1;
        descriptor.resource_flags.alternate = 1;
        descriptor.secondary_data = secondary->data;
        descriptor.secondary_extent = secondary->bits.extent;
        descriptor.secondary_id = secondary->id;
        descriptor.resource_flags.unknown_00 = 1;
        descriptor.flags.palette_allocation = palette->allocation_result;
        descriptor.flags.unknown_16_18 = 1;
        descriptor.first_texture_tile = 0;
        descriptor.texture_tile_count = 0;
        descriptor.palette = &palette->palette;
        descriptor.resource_flags.screen = (u8)entity->base.property_00a_bits.property_00a_unknown_01;
        descriptor.resource_animation = 0;
        descriptor.animation_id = 0;
        descriptor.animation_offset_x = 80;
        descriptor.animation_offset_y = 80;
        descriptor.animation_speed = 256;
        descriptor.flags.animation_active = 0;
        descriptor.flags.unknown_09 = 1;
        descriptor.flags.behavior_state = 0;
        descriptor.flags.unknown_10 = 0;
        descriptor.flags.unknown_11 = 0;
        descriptor.unknown_30 = 0;
        descriptor.flags.unknown_05_07 = 0;
        descriptor.scale_x = 256;
        descriptor.scale_y = 256;
        descriptor.rotation = 0;
        entity->auxiliary_render_object->unknown_5c(&descriptor, 0, 0);
        entity->auxiliary_render_object->unknown_24();
    }
}

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

static inline void SetBodyBounds(FieldRuntimeEntity *entity, int x, int y, int width, int height, int depth)
{
    entity->body_min_x = x << 12;
    entity->body_max_x = entity->body_min_x + (width << 12);
    entity->body_max_y = y << 12;
    entity->body_min_y = entity->body_max_y - (height << 12);
    entity->body_vertical_extent = depth << 12;
}
static inline void SetNavigationBounds(FieldRuntimeEntity *entity, int x, int y, int width, int height,
                                       int depth)
{
    entity->navigation_min_x = x << 12;
    entity->navigation_max_x = entity->navigation_min_x + (width << 12);
    entity->navigation_max_y = y << 12;
    entity->navigation_min_y = entity->navigation_max_y - (height << 12);
    entity->navigation_vertical_extent = depth << 12;
}
static inline void SetBox(FieldBoundsRectangle *box, int x, int y, int width, int height)
{
    box->min_x = x << 12;
    box->max_x = box->min_x + (width << 12);
    box->max_y = y << 12;
    box->min_y = box->max_y - (height << 12);
}
static inline void SetBodyAngles(FieldRuntimeEntity *entity)
{
    entity->body_center_y = (entity->body_min_y + entity->body_max_y) / 2;
    entity->body_corner_angles[0] = FX_Atan2(entity->body_min_x, entity->body_center_y);
    entity->body_corner_angles[1] = FX_Atan2(entity->body_min_x, -entity->body_center_y);
    entity->body_corner_angles[2] = FX_Atan2(entity->body_max_x, -entity->body_center_y);
    entity->body_corner_angles[3] = FX_Atan2(entity->body_max_x, entity->body_center_y);
}
void FieldEntity3D_UpdateBounds(FieldRuntimeEntity *entity)
{
    if (entity->bounds_resource) {
        int animation = entity->animation_id;
        /* Spatial bounds use signed indices in the six-byte animation record. */
        const s8 *indices = (const s8 *)entity->animation_bounds;
        int body_index, navigation_index, interaction_index, body_lookup, navigation_lookup;
        const FieldInteractionBounds *body, *navigation, *interaction;
        int previous_min_x, previous_max_x, previous_max_y, previous_min_y;
        if (animation >= entity->bounds_animation_count)
            animation = 0;
        body_index = indices[animation * (int)sizeof(FieldAnimationBoundsIndex) + 0];
        body = &entity->body_bounds[body_index];
        navigation_index = indices[animation * (int)sizeof(FieldAnimationBoundsIndex) + 1];
        interaction_index = indices[animation * (int)sizeof(FieldAnimationBoundsIndex) + 2];
        previous_min_x = entity->navigation_min_x;
        previous_max_x = entity->navigation_max_x;
        previous_max_y = entity->navigation_max_y;
        previous_min_y = entity->navigation_min_y;
        body_lookup = indices[animation * (int)sizeof(FieldAnimationBoundsIndex) + 3];
        navigation_lookup = indices[animation * (int)sizeof(FieldAnimationBoundsIndex) + 4];
        navigation = &entity->navigation_bounds[navigation_index];
        interaction = &entity->interaction_bounds[interaction_index];
        SetBodyBounds(entity, body->minimum_x, body->maximum_y, body->width, body->height,
                      body->vertical_extent);
        SetBodyAngles(entity);
        if (body_lookup == -1) {
            SetBox(&entity->body_lookup_bounds, body->minimum_x, body->maximum_y, body->width, body->height);
        } else {
            const FieldBoundsLookup *bounds =
                (const FieldBoundsLookup *)entity->body_bounds_lookup + body_lookup;
            SetBox(&entity->body_lookup_bounds, bounds->min_x, bounds->max_y, bounds->width, bounds->height);
        }
        SetNavigationBounds(entity, navigation->minimum_x, navigation->maximum_y, navigation->width,
                            navigation->height, navigation->vertical_extent);
        if (navigation_lookup == -1) {
            SetBox(&entity->navigation_lookup_bounds, navigation->minimum_x, navigation->maximum_y,
                   navigation->width, navigation->height);
        } else {
            const FieldBoundsLookup *bounds =
                (const FieldBoundsLookup *)entity->navigation_bounds_lookup + navigation_lookup;
            SetBox(&entity->navigation_lookup_bounds, bounds->min_x, bounds->max_y, bounds->width,
                   bounds->height);
        }
        FieldEntity_SetInteractionBounds(&entity->base, interaction->minimum_x, interaction->maximum_y,
                                         (u16)interaction->width, interaction->height,
                                         interaction->vertical_extent);
        if (entity->navigation_surfaces && entity->base.property_00a_bits.subtype != 10) {
            if (entity->runtime_flag_bits.sync_horizontal &&
                (entity->navigation_min_x != previous_min_x || entity->navigation_min_y != previous_min_y ||
                 entity->navigation_max_x != previous_max_x || entity->navigation_max_y != previous_max_y)) {
                entity->collision_state_flags |= 0x200000;
                entity->runtime_flags |= 0x40;
            }
            entity->runtime_flags |= 0x20;
        }
        entity->body_bounds_index = body_index;
        entity->navigation_bounds_index = navigation_index;
        entity->bounds_index = interaction_index;
        entity->body_lookup_index = body_lookup;
        entity->navigation_lookup_index = navigation_lookup;
    } else {
        SetBodyBounds(entity, -8, 0, 16, 8, 32);
        SetBodyAngles(entity);
        SetNavigationBounds(entity, -8, 0, 16, 8, 32);
        FieldEntity_SetInteractionBounds(&entity->base, -8, 0, 16, 8, 32);
    }
}
}
