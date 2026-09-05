#include <game/battle_effect.h>
#include <game/battle_hit.h>
#include <game/battle_scene.h>

int BattleCollision_TestObjects(u32 source_id, u32 target_id,
                                BattlePosition *hit_position) {
    BattleSceneObject *source_object;
    BattleSceneObject *target_object;
    BattleModel *target_model;
    int source_animation;
    int previous_frame;
    int source_current_frame;
    int target_animation;
    int target_current_frame;
    BattleCollisionBounds source_previous;
    BattleCollisionBounds source_current;
    BattleCollisionBounds target_previous;
    BattleCollisionBounds target_current;

    source_object = BattleSceneObject_GetById((u16)source_id);
    target_object = BattleSceneObject_GetById((u16)target_id);
    BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById((u16)source_id));
    target_model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById((u16)target_id));
    if (target_model == 0) {
        return 0;
    }
    if (!target_model->flag_bits.animation_active) {
        return 0;
    }

    source_animation = 0;
    target_current_frame = 0;
    source_current_frame = 0;
    target_animation = 0;
    previous_frame = -1;

    for (;;) {
        int target_result;

        if (!BattleCollision_GetBounds(&source_previous, source_id,
                                       source_animation, previous_frame)) {
            return 0;
        }
        if (!BattleCollision_GetBounds(&source_current, source_id,
                                       source_animation,
                                       source_current_frame)) {
            return 0;
        }

        source_previous.axis[0] += source_object->motion_origin_x;
        source_previous.axis[1] += source_object->motion_origin_x;
        source_previous.axis[2] =
            -(source_previous.axis[2] + source_object->motion_origin_z);
        source_previous.axis[3] =
            -(source_previous.axis[3] + source_object->motion_origin_z);
        source_previous.axis[4] =
            -(source_previous.axis[4] + source_object->motion_origin_y);
        source_previous.axis[5] =
            -(source_previous.axis[5] + source_object->motion_origin_y);

        source_current.axis[0] += source_object->x;
        source_current.axis[1] += source_object->x;
        source_current.axis[2] =
            -(source_current.axis[2] + source_object->z);
        source_current.axis[3] =
            -(source_current.axis[3] + source_object->z);
        source_current.axis[4] =
            -(source_current.axis[4] + source_object->y);
        source_current.axis[5] =
            -(source_current.axis[5] + source_object->y);

        target_animation = 0;
        while (BattleCollision_GetBounds(&target_previous, target_id,
                                         target_animation, previous_frame) &&
               BattleCollision_GetBounds(&target_current, target_id,
                                         target_animation,
                                         target_current_frame)) {
            target_previous.axis[0] += target_object->motion_origin_x;
            target_previous.axis[1] += target_object->motion_origin_x;
            target_previous.axis[2] =
                -(target_previous.axis[2] + target_object->motion_origin_z);
            target_previous.axis[3] =
                -(target_previous.axis[3] + target_object->motion_origin_z);
            target_previous.axis[4] =
                -(target_previous.axis[4] + target_object->motion_origin_y);
            target_previous.axis[5] =
                -(target_previous.axis[5] + target_object->motion_origin_y);

            target_current.axis[0] += target_object->x;
            target_current.axis[1] += target_object->x;
            target_current.axis[2] =
                -(target_current.axis[2] + target_object->z);
            target_current.axis[3] =
                -(target_current.axis[3] + target_object->z);
            target_current.axis[4] =
                -(target_current.axis[4] + target_object->y);
            target_current.axis[5] =
                -(target_current.axis[5] + target_object->y);

            target_result = BattleCollision_TestVolumes(
                &source_previous, &source_current, &target_previous,
                &target_current, hit_position);
            if (target_result != 0) {
                return target_result;
            }
            ++target_animation;
        }
        ++source_animation;
    }
}
