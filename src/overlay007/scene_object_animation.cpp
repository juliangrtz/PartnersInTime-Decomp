#include "scene_object_internal.h"

extern "C" {

int SceneObject_IsMotionActiveById(u32 id, int channel)
{
    return SceneObject_IsMotionActive((SceneObject *)SceneObject_GetById(id), channel);
}

int SceneObject_IsMotionActive(SceneObject *object, int channel)
{
    return object->motion[channel].callback != 0;
}

void SceneObject_SetSecondaryAnimation(SceneObject *object, int animation, int mode)
{
    int state;
    if (!mode)
        return;
    if (mode == 1 && !(object->flags.bits.use_secondary_model)) {
        object->flags.bits.use_secondary_model = 1;
        if (PRIMARY(object))
            SceneModel_Stop(PRIMARY(object));
    }
    if ((object->flags.bits.use_secondary_model) == 1) {
        if (animation < 0) {
            if (SECONDARY(object))
                SceneModel_Stop(SECONDARY(object));
        } else {
            state = 1;
            if (SECONDARY(object))
                state = SECONDARY(object)->flag_bits.unknown_00_01;
            if (object->resource != META(object->resource_id)) {
                object->resource = META(object->resource_id);
                if (SECONDARY(object))
                    SceneModel_Stop(SECONDARY(object));
            }
            if (!SECONDARY(object)) {
                object->secondary_renderable = func_ov005_02068c80();
                SECONDARY(object)->flag_bits.animation_active = 0;
            }
            SECONDARY(object)->flag_bits.unknown_09 = 0;
            SECONDARY(object)->flag_bits.panel_animation_trigger = 0;
            if (!SECONDARY(object)->flag_bits.animation_active) {
                SceneModel_Initialize(SECONDARY(object), 1, object->resource_id);
                SECONDARY(object)->flag_bits.animation_active = 1;
            }
            SECONDARY(object)->set_primary_animation((u8)animation, 0, 1);
            SECONDARY(object)->flag_bits.unknown_00_01 = state;
        }
    }
}

void SceneObject_SetAnimationById(u32 id, int animation, int secondary)
{
    SceneObject_SetAnimation((SceneObject *)SceneObject_GetById(id), animation, secondary);
}

void SceneObject_SetAnimation(SceneObject *object, int animation, int secondary)
{
    object->property_0d6 = 0;
    if (animation >= 0) {
        int base = object->animation_id;
        animation += base;
        object->property_0d2 = object->property_0d0 - base;
        object->property_0d0 = animation;
    }
    if (!secondary) {
        if (object->flags.bits.use_secondary_model) {
            object->flags.bits.use_secondary_model = 0;
            if (SECONDARY(object))
                SceneModel_Stop(SECONDARY(object));
        }
    } else
        SceneObject_SetSecondaryAnimation(object, animation, secondary);
    if (!(object->flags.bits.use_secondary_model)) {
        if (animation < 0) {
            object->resource = META(object->resource_id);
            if (PRIMARY(object))
                SceneModel_Stop(PRIMARY(object));
        } else {
            if (object->resource != META(object->resource_id)) {
                object->resource = META(object->resource_id);
                if (PRIMARY(object))
                    SceneModel_Stop(PRIMARY(object));
            }
            if (!PRIMARY(object)) {
                object->primary_renderable = func_ov005_02068c80();
                PRIMARY(object)->flag_bits.animation_active = 0;
            }
            PRIMARY(object)->flag_bits.unknown_09 = 0;
            PRIMARY(object)->flag_bits.panel_animation_trigger = 0;
            if (!PRIMARY(object)->flag_bits.animation_active) {
                SceneModel_Initialize(PRIMARY(object), 0, object->resource_id);
                PRIMARY(object)->animation_offset_x = 0;
                PRIMARY(object)->animation_offset_y = 0;
                PRIMARY(object)->flag_bits.animation_active = 1;
            }
            PRIMARY(object)->set_primary_animation((u8)animation, 0, 1);
        }
    }
}

void SceneObjects_SwapSlots(u32 first, u32 second)
{
    SceneObject *saved;
    if (first < 40 && second < 40) {
        saved = SLOTS(first);
        SLOTS(first) = SLOTS(second);
        SLOTS(second) = saved;
        SLOTS(first)->linked_object_id = first;
        SLOTS(first)->object_id = SLOTS(first)->linked_object_id;
        SLOTS(second)->linked_object_id = second;
        SLOTS(second)->object_id = SLOTS(second)->linked_object_id;
    }
}

void SceneObject_SetProperty(u32 id, int property, int value)
{
    BattleModel *model;
    SceneObject *object;
    /* The native switch keeps explicit no-op properties at their original
       fallthrough positions. */
    switch (property) {
    case 0:
        SceneObject_GetModelById(id)->flag_bits.animation_active = value;
    case 1:
        return;
    case 2:
        OBJECT(id)->render_height = value;
        return;
    case 3:
        SceneObject_GetModelById(id)->flag_bits.unknown_00_01 = value;
        return;
    case 4:
        OBJECT(id)->default_motion_speed = value;
        return;
    case 5:
        SceneObject_GetModelById(id)->effect_scale = (value << 8) / 4096;
    case 6:
    case 7:
    case 8:
        return;
    case 9:
        SceneObject_GetModelById(id)->flag_bits.unknown_03 = value;
        return;
    case 10:
        OBJECT(id)->render_offset_x = value;
        return;
    case 11:
        OBJECT(id)->render_offset_y = value;
        return;
    case 12:
        object = OBJECT(id);
        object->drawn_z = value;
        object->base_y = object->drawn_z;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
        return;
    case 23:
        SceneObject_GetModelById(id)->animation_id = value;
        return;
    case 24:
        model = SceneObject_GetModelById(id);
        model->set_animation((u8)model->get_animation_id(), (s16)value);
        return;
    case 25:
        SceneObject_SetFacingLeft(id, value);
        return;
    case 26:
        SceneObject_SetFlipY(id, value);
        return;
    case 27:
        SceneObject_GetModelById(id)->flag_bits.unknown_09 = value;
    case 28:
        return;
    case 29:
        SceneObject_SetAnimationLimit(OBJECT(id), value, 0);
        return;
    case 30:
        OBJECT(id)->property_0d2 = value;
    case 31:
    case 32:
        return;
    case 33:
        SceneObject_GetModelById(id)->rotation_z = value;
        return;
    case 35:
        SceneObject_GetModelById(id)->scale_x = value / 16;
        return;
    case 36:
        SceneObject_GetModelById(id)->scale_y = value / 16;
    case 37:
    case 38:
        return;
    case 39:
        OBJECT(id)->animation_id = value;
    case 40:
    case 41:
    case 42:
    case 43:
        return;
    }
}
}
