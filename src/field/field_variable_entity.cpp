extern "C" {

/*
 * Variable entity (overlay 0, 0x020BC684-0x020BD1EC).
 *
 * The entities a room places from its variable table: construction from a
 * placement record, the bobbing and bounce they play, and the enable flag a
 * script toggles them with.
 */

#include <game/field_variable_entity.h>
extern FieldEntityVTable data_ov000_020c1310;
extern const FieldSpawnRecord data_ov000_020c0c80;
extern const s8 data_ov000_020c0c64[];
extern void func_0202cbd4(void *, int, u32);
/* Match the original signed 32-bit masks extended to the 64-bit policy. */
#define SetCollisionChannel(entity, shift, policy)                                                           \
    do {                                                                                                     \
        (entity)->collision_policy &= ~(s32)(7u << (shift));                                                 \
        if (policy)                                                                                          \
            (entity)->collision_policy |= (s32)((u32)(policy) << (shift));                                   \
    } while (0)
FieldVariableEntity *FieldVariableEntity_InitPlacement(FieldVariableEntity *entity, int index,
                                                       const FieldSpawnRecord *spawn, int argument,
                                                       int resource_set, u8 animation_enabled)
{
    FieldEntity3D_InitPlacement(&entity->entity, index, spawn, argument, resource_set, animation_enabled);
    /* Install the entity's dispatch table. FieldEntity models it as virtual
     * functions in C++ mode, so the slot is written through the object's
     * first word, which is where the table pointer lives. */
    *(FieldEntityVTable **)&entity->entity.base = &data_ov000_020c1310;
    func_0202cbd4(&entity->state, 0, sizeof(entity->state));
    entity->state.flags.mode = 0;
    entity->state.flags.enabled = 1;
    entity->state.flags.bobbing = 1;
    entity->state.flags.unknown_05 = 0;
    entity->state.flags.bob_frame = 0;
    entity->state.flags.bounce_frame = -1;
    SetCollisionChannel(&entity->entity, 0, 7);
    SetCollisionChannel(&entity->entity, 3, 7);
    SetCollisionChannel(&entity->entity, 6, 7);
    SetCollisionChannel(&entity->entity, 9, 7);
    SetCollisionChannel(&entity->entity, 12, 0);
    SetCollisionChannel(&entity->entity, 15, 0);
    SetCollisionChannel(&entity->entity, 18, 0);
    SetCollisionChannel(&entity->entity, 21, 0);
    SetCollisionChannel(&entity->entity, 24, 0);
    SetCollisionChannel(&entity->entity, 27, 0);
    SetCollisionChannel(&entity->entity, 30, 7);
    entity->entity.unknown_3a0_bits.unknown_26_31 = 16;
    return entity;
}
FieldVariableEntity *FieldVariableEntity_InitVariablePlacement(FieldVariableEntity *entity, int index,
                                                               int argument,
                                                               const FieldVariablePlacement *placement)
{
    FieldEntity3D_InitPlacement(&entity->entity, index, &data_ov000_020c0c80, argument, 0, 1);
    *(FieldEntityVTable **)&entity->entity.base = &data_ov000_020c1310;
    func_0202cbd4(&entity->state, 0, sizeof(entity->state));
    entity->entity.position_x = placement->x << 12;
    entity->entity.position_y = placement->y << 12;
    entity->entity.position_z = placement->z << 12;
    /* Preserve the original wrapped 16-bit subtraction of variable bank 0x2000. */
    entity->entity.unknown_3d4 = placement->variable + (u16)-0x2000;
    entity->state.flags.mode = placement->flags.mode;
    entity->state.flags.enabled = VM_ReadVariable((u16)entity->entity.unknown_3d4, 0, 0) ^ 1;
    entity->state.flags.bobbing = 1;
    entity->state.flags.unknown_05 = 0;
    entity->state.flags.bob_frame = 0;
    entity->state.flags.bounce_frame = -1;
    entity->state.remaining_uses = placement->flags.parameter;
    entity->state.unknown_07 = placement->motion.amount_index;
    entity->state.motion.countdown = 0;
    entity->state.motion.remaining_frames =
        entity->state.unknown_06 * data_ov000_020c0c64[placement->motion.direction];
    entity->state.unknown_08 = placement->unknown_02;
    entity->state.variable = placement->variable;
    entity->entity.saved_presentation_flag_bits.behavior_mode = 1;
    SetCollisionChannel(&entity->entity, 0, 7);
    SetCollisionChannel(&entity->entity, 3, 7);
    SetCollisionChannel(&entity->entity, 6, 0);
    SetCollisionChannel(&entity->entity, 9, 7);
    SetCollisionChannel(&entity->entity, 12, 0);
    SetCollisionChannel(&entity->entity, 15, 0);
    SetCollisionChannel(&entity->entity, 18, 0);
    SetCollisionChannel(&entity->entity, 21, 0);
    SetCollisionChannel(&entity->entity, 24, 0);
    SetCollisionChannel(&entity->entity, 27, 0);
    SetCollisionChannel(&entity->entity, 30, 7);
    entity->entity.unknown_3a0_bits.unknown_26_31 = 16;
    return entity;
}
#undef SetCollisionChannel

}

extern "C" {
#include <game/script_vm.h>
#include <game/heap.h>
}
#include <game/field_variable_entity.h>
extern "C" {
extern const s8 data_ov000_020c0c68[4], data_ov000_020c0c6c[17];
void FieldEntity3D_UpdateMotion(FieldRuntimeEntity *, FieldRuntimeEntity **);
void func_020093b4(FieldRenderObject *, int);
FieldVariableEntity *FieldVariableEntity_InitEmpty(FieldVariableEntity *object) {
    FieldEntity3D_Init(&object->entity);
    *(FieldEntityVTable **)object = &data_ov000_020c1310;
    func_0202cbd4((u8 *)object + 4, 0, sizeof(*object) - 4);
    return object;
}

FieldVariableEntity *FieldVariableEntity_Destroy(FieldVariableEntity *object) {
    *(FieldEntityVTable **)object = &data_ov000_020c1310;
    FieldEntity3D_DestroyBase(&object->entity);
    return object;
}

FieldVariableEntity *FieldVariableEntity_Delete(FieldVariableEntity *object) {
    *(FieldEntityVTable **)object = &data_ov000_020c1310;
    FieldEntity3D_DestroyBase(&object->entity);
    GameHeap_Delete(object);
    return object;
}

void FieldVariableEntity_Update(FieldVariableEntity *object, FieldRuntimeEntity **entities) {
    if (!object->entity.saved_presentation_flag_bits.unknown_31 && object->state.flags.enabled) {
        /* Decrement through the native unsigned halfword conversion. */
        if (object->state.motion.countdown && object->state.motion.remaining_frames)
            object->state.motion.remaining_frames =
                (u16)(object->state.motion.remaining_frames + (u16)-1);
        int frame = object->state.flags.bounce_frame;
        if (frame == -1) {
            if (object->state.flags.bobbing) {
                object->entity.screen_offset_y =
                    data_ov000_020c0c68[object->state.flags.bob_frame >> 3];
                object->state.flags.bob_frame++;
                if (object->state.flags.bob_frame >= 32)
                    object->state.flags.bob_frame = 0;
            }
        } else if (frame >= 0) {
            object->entity.screen_offset_y = data_ov000_020c0c6c[frame];
            object->state.flags.bounce_frame++;
            if ((u32)object->state.flags.bounce_frame >= 17) {
                object->state.flags.bounce_frame = -1;
                if (object->state.flags.mode) {
                    if (object->state.flags.unknown_05)
                        object->state.flags.unknown_05 = 0;
                    else if (object->state.flags.disable_after_bounce &&
                             (!object->state.remaining_uses ||
                              !object->state.motion.remaining_frames))
                        FieldVariableEntity_SetEnabled(object, 0);
                }
            }
        }
    }
    FieldEntity3D_UpdateMotion(&object->entity, entities);
}

void FieldVariableEntity_StartBounce(FieldVariableEntity *object, u8 countdown,
                                     u8 disable_when_finished) {
    object->entity.screen_offset_y = 0;
    object->state.flags.bounce_frame = 0;
    if (object->state.flags.unknown_05)
        func_020093b4(object->entity.render_object, 1);
    else {
        object->state.motion.countdown = countdown;
        object->state.flags.disable_after_bounce = disable_when_finished;
    }
}

void FieldVariableEntity_SetBobbing(FieldVariableEntity *object, int enabled) {
    if (object->state.flags.bobbing != enabled) {
        object->state.flags.bobbing = enabled;
        if (!enabled) {
            object->entity.screen_offset_y = 0;
            object->state.flags.bob_frame = 0;
        }
    }
}

void FieldVariableEntity_SetEnabled(FieldVariableEntity *object, int enabled) {
    if (object->state.flags.enabled != enabled) {
        int animation;
        object->state.flags.enabled = enabled;
        object->state.saved_animation = object->entity.animation_id;
        if (enabled)
            animation = object->state.saved_animation;
        else {
            object->entity.screen_offset_y = 0;
            object->state.flags.bob_frame = 0;
            object->state.motion.countdown = 0;
            object->state.motion.remaining_frames = 0;
            animation = object->entity.render_object->unknown_40() - 1;
            object->entity.saved_animation_frame = 0;
            object->entity.saved_presentation_flag_bits.behavior_mode = 0;
        }
        object->entity.render_object->set_animation((u8)animation, 0, 1);
        object->entity.animation_id = animation;
        if (object->state.flags.mode)
            VM_WriteVariable(object->entity.unknown_3d4, enabled == 0, 0, 0);
    }
}
}

extern "C" {

FieldVariableEntity *FieldVariableEntity_CopyState(FieldVariableEntity *entity,
                                                   const FieldVariableEntity *source)
{
    entity->state.flags.mode = source->state.flags.mode;
    entity->state.flags.enabled = source->state.flags.enabled;
    entity->state.flags.bobbing = source->state.flags.bobbing;
    entity->state.flags.unknown_05 = source->state.flags.unknown_05;
    entity->state.flags.bob_frame = source->state.flags.bob_frame;
    entity->state.flags.bounce_frame = source->state.flags.bounce_frame;
    entity->state.unknown_06 = source->state.unknown_06;
    entity->state.unknown_07 = source->state.unknown_07;
    entity->state.unknown_08 = source->state.unknown_08;
    entity->state.variable = source->state.variable;
    entity->state.motion.countdown = source->state.motion.countdown;
    entity->state.motion.remaining_frames = source->state.motion.remaining_frames;
    FieldEntity_CopySpatialState(&entity->entity, &source->entity);
    return entity;
}

}
