extern "C" {
#include <game/script_vm.h>
#include <game/heap.h>
}
#include <game/field_variable_entity.h>
extern "C" {
extern const s8 data_ov000_020c0c68[4], data_ov000_020c0c6c[17];
extern FieldEntityVTable data_ov000_020c1310;
void func_ov000_020b4990(FieldRuntimeEntity *, FieldRuntimeEntity **);
void func_020093b4(FieldRenderObject *, int);
void func_0202cbd4(void *, int, u32);
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
    func_ov000_020b4990(&object->entity, entities);
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
