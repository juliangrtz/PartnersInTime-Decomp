#include <game/battle_effect.h>
#include <game/overlay013_attack.h>

enum {
    OVERLAY13_DIRECTION_ANIMATION_LOW = 7,
    OVERLAY13_DIRECTION_ANIMATION_MIDDLE = 8,
    OVERLAY13_DIRECTION_ANIMATION_HIGH = 9,
};

static inline int Overlay13Attack_GetModelAnimation(BattleModel *model) {
#ifdef __cplusplus
    return model->get_animation_id();
#else
    return ((int (*)(BattleModel *))((void **)model->vtable)[15])(model);
#endif
}

static inline void Overlay13Attack_SetModelAnimation(BattleModel *model,
                                                     int animation_id) {
#ifdef __cplusplus
    model->set_primary_animation(animation_id, 0, 1);
#else
    model->vtable->set_primary_animation(model, animation_id, 0, 1);
#endif
}

/* Metrowerks emits these C-linkage functions in reverse source order. */
int Overlay13Attack_StartVerticalEntry(BattleSceneObject *object,
                                       int animation_id, int variant) {
    BattleModel *model;
    int duration;

    BattleSceneObject_SetAnimation(object, animation_id, 0);
    model = object->primary_model;
    model->flags &= ~BATTLE_MODEL_FLAG_10;
    model->flags &= ~BATTLE_MODEL_FLAG_11;
    duration = BattleSceneObject_StartVerticalMotionTo(
        object, 3, 0xA00, -144, 80, 1);
    if (variant & 1) {
        BattleSceneObject_MoveTo(object, 2, 16, 152, 0, duration);
    } else {
        BattleSceneObject_MoveTo(object, 2, 16, 112, 0, duration);
    }
    return 1;
}

int Overlay13Attack_UpdateAnimation(BattleSceneObject *object,
                                    int animation_id) {
    if (BattleSceneObject_IsAnimationChannelActive(object, 3)) {
        if (animation_id !=
                Overlay13Attack_GetModelAnimation(object->primary_model) &&
            object->z < object->motion_origin_z) {
            BattleModel *model = object->primary_model;

            Overlay13Attack_SetModelAnimation(model, animation_id);
        }
        return 0;
    }
    return 1;
}

int Overlay13Attack_ClassifyVerticalAngle(BattlePosition *position,
                                          s16 *angle, u8 *animation_id) {
    int difference = position->y - position->z;
    int result = 0;
    int adjusted_angle;

    do {
        if (difference >= 0) {
            *angle = difference;
            adjusted_angle = *angle + 192;
            break;
        }
        if (difference < 0) {
            if (difference > -42) {
                *angle = difference;
                result = 1;
                adjusted_angle = 192;
                break;
            }
        }
        if (difference <= -42) {
            result = 2;
            *angle = difference + 266;
            adjusted_angle = *angle;
        }
    } while (0);

    if (adjusted_angle <= 96) {
        *animation_id = OVERLAY13_DIRECTION_ANIMATION_LOW;
        return result;
    }
    if (adjusted_angle <= 288) {
        *animation_id = OVERLAY13_DIRECTION_ANIMATION_MIDDLE;
    } else {
        *animation_id = OVERLAY13_DIRECTION_ANIMATION_HIGH;
    }
    return result;
}
