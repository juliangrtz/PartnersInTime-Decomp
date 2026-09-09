#include <game/field_presentation.h>
#include <game/field_timed_renderer.h>
#include <game/sprite_output.h>
#include <game/field_resources.h>
#include <game/battle_scene.h>
extern "C" {
#include <game/heap.h>
void func_0200940c(FieldRenderObject *, int);
void FieldEntity_ConfigureRendererResources(FieldRuntimeEntity *entity, const FieldPrimaryResource *primary,
                                            const FieldSecondaryResource *secondary,
                                            FieldPaletteResource *palette, int speed)
{
    FieldResourceDescriptor descriptor;
    if (!speed)
        speed = entity->animation_speed;
    else
        entity->animation_speed = speed;
    entity->primary_resource = primary;
    entity->secondary_resource = secondary;
    entity->palette_resource = palette;
    entity->primary_resource_record = primary->record;
    BattleModel_InitDescriptor(&descriptor, 0);
    int size =
        BattleModel_GetScreenTextureConversionSize((u8)entity->base.property_00a_bits.property_00a_unknown_01,
                                                   (u8)entity->saved_presentation_flag_bits.resource_set,
                                                   (const GameGraphicsResource *)primary->animation);
    if (size)
        descriptor.conversion_buffer = GameHeap_NewArray(size, entity->base.property_00a_bits.heap, 0, 1);
    descriptor.flags.resource_set = entity->saved_presentation_flag_bits.resource_set;
    descriptor.flags.unknown_23 = 0;
    descriptor.animation = primary->animation;
    descriptor.graphics = primary->graphics;
    descriptor.graphics_size = primary->bits.graphics_size;
    descriptor.primary_id = primary->id;
    descriptor.resource_flags.alternate =
        (u8)(entity->saved_presentation_flag_bits.resource_set == 1 ? 1 : 0);
    descriptor.secondary_data = secondary->data;
    descriptor.secondary_extent = secondary->bits.extent;
    descriptor.secondary_id = secondary->id;
    descriptor.resource_flags.unknown_00 = 1;
    descriptor.flags.palette_allocation = palette->allocation_result;
    descriptor.palette = palette;
    descriptor.flags.unknown_16_18 = entity->saved_presentation_flag_bits.resource_set == 1 ? 1 : 0;
    descriptor.first_texture_tile = 0;
    descriptor.texture_tile_count = 0;
    descriptor.resource_flags.screen = (u8)entity->base.property_00a_bits.property_00a_unknown_01;
    descriptor.resource_animation = entity->animation_id;
    descriptor.animation_id = entity->saved_animation_frame;
    descriptor.animation_speed = speed;
    descriptor.animation_offset_x = 80;
    descriptor.animation_offset_y = 80;
    descriptor.flags.animation_active = entity->base_state_flag_bits.animation_wait_enabled;
    descriptor.flags.unknown_09 = 0;
    descriptor.flags.behavior_state = 0;
    descriptor.flags.unknown_10 = 0;
    descriptor.flags.unknown_11 = 0;
    descriptor.unknown_30 = 0;
    descriptor.flags.unknown_05_07 = 0;
    descriptor.rotation = 0;
    descriptor.scale_x = 256;
    descriptor.scale_y = 256;
    entity->render_object->unknown_5c(&descriptor, secondary->auxiliary, 0);
    entity->render_object->unknown_24();
    if (!entity->base_state_flag_bits.unknown_01) {
        int mode = entity->primary_resource_record->flag_bits.direction_mode;
        if (mode == 1)
            entity->base_state_flag_bits.facing_direction = 2 * (entity->animation_id & 3);
        else if (mode == 2)
            entity->base_state_flag_bits.facing_direction = entity->animation_id & 7;
    }
    entity->base.unknown_20(primary->bounds);
    entity->saved_presentation_flag_bits.unknown_06 = 1;
}

void FieldEntity_RebindRendererResources(FieldRuntimeEntity *entity, const FieldPrimaryResource *primary,
                                         const FieldSecondaryResource *secondary,
                                         FieldPaletteResource *palette, int animation, u8 update_bounds,
                                         int speed)
{
    FieldResourceDescriptor descriptor;
    if (!speed)
        speed = entity->animation_speed;
    else
        entity->animation_speed = speed;
    entity->primary_resource = primary;
    if (secondary)
        entity->secondary_resource = secondary;
    else
        secondary = entity->secondary_resource;
    if (palette)
        entity->palette_resource = palette;
    else
        palette = entity->palette_resource;
    entity->primary_resource_record = primary->record;
    BattleModel_InitDescriptor(&descriptor, 1);
    descriptor.flags.resource_set = entity->saved_presentation_flag_bits.resource_set;
    descriptor.flags.unknown_23 = 0;
    descriptor.animation = primary->animation;
    descriptor.conversion_buffer = 0;
    descriptor.graphics = primary->graphics;
    descriptor.graphics_size = primary->bits.graphics_size;
    descriptor.primary_id = primary->id;
    descriptor.resource_flags.alternate =
        (u8)(entity->saved_presentation_flag_bits.resource_set == 1 ? 1 : 0);
    descriptor.secondary_data = secondary->data;
    descriptor.secondary_extent = secondary->bits.extent;
    descriptor.secondary_id = secondary->id;
    descriptor.resource_flags.unknown_00 = 1;
    descriptor.flags.palette_allocation = palette->allocation_result;
    descriptor.palette = palette;
    descriptor.flags.unknown_16_18 = entity->saved_presentation_flag_bits.resource_set == 1 ? 1 : 0;
    descriptor.first_texture_tile = 0;
    descriptor.texture_tile_count = 0;
    descriptor.resource_flags.screen = (u8)entity->base.property_00a_bits.property_00a_unknown_01;
    descriptor.unknown_1f = 0;
    entity->render_object->unknown_64(&descriptor, secondary->auxiliary, 0);
    if (animation != -1)
        entity->animation_id = animation;
    if (entity->animation_id >= entity->render_object->unknown_40())
        entity->animation_id = 0;
    if (!entity->saved_presentation_flag_bits.resource_set)
        GameSpriteAllocation_Compact((u8)entity->base.property_00a_bits.property_00a_unknown_01, 0, 0);
    func_0200940c(entity->render_object, speed);
    entity->render_object->set_animation((u8)entity->animation_id, 0, 1);
    FieldTimedRenderer_ResetAnimationControl((FieldAnimationRenderer *)entity->render_object);
    if (animation == -1)
        entity->base.update_animation(0, 0);
    entity->saved_presentation_flag_bits.unknown_14 = 1;
    if (update_bounds)
        entity->base.unknown_20(primary->bounds);
}

void FieldEntity_StopRenderer(FieldRuntimeEntity *entity)
{
    if (entity->saved_presentation_flag_bits.unknown_06) {
        entity->render_object->stop();
        entity->render_object->unknown_1c();
        if (!entity->saved_presentation_flag_bits.resource_set)
            GameSpriteAllocation_Compact((u8)entity->base.property_00a_bits.property_00a_unknown_01, 0, 0);
        entity->saved_presentation_flag_bits.unknown_06 = 0;
    }
}

void FieldEntity_SetResourceAnimation(FieldRuntimeEntity *entity, int resource_animation, int animation,
                                      int reset_control, u8 update_bounds)
{
    if (reset_control)
        FieldTimedRenderer_ResetAnimationControl((FieldAnimationRenderer *)entity->render_object);
    entity->render_object->set_animation(resource_animation, (s16)animation, 1);
    entity->animation_id = resource_animation;
    if (update_bounds)
        entity->base.update_bounds();
}

void FieldEntity2D_BindBoundsResource(FieldRuntimeEntity *entity, const u16 *resource)
{
    entity->bounds_resource = resource;
    if (resource) {
        entity->interaction_bounds = (const FieldInteractionBounds *)((const u8 *)resource + resource[2] + 1);
        entity->animation_bounds =
            (const FieldAnimationBoundsIndex *)((const u8 *)resource + resource[5] + 1);
        entity->bounds_animation_count = *((const u8 *)resource + resource[5]);
    } else {
        entity->interaction_bounds = 0;
        entity->animation_bounds = 0;
        entity->bounds_animation_count = 0;
    }
    entity->base.update_bounds();
}
}
