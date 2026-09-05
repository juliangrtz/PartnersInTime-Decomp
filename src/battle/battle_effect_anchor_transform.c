#include <game/battle_ai.h>
#include <game/battle_effect.h>

#include <hardware.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

enum {
    BATTLE_VIEW_ORIGIN_X_OFFSET = 0xCB9C,
    BATTLE_VIEW_ORIGIN_Y_OFFSET = 0xCB9E
};

BattleSceneObject *BattleSceneObject_StretchBetweenAnchors(
    int object_id,
    int first_object_id,
    s16 first_offset_x, s16 first_offset_y, int first_offset_z,
    int first_anchor_z,
    u16 second_object_id,
    s16 second_offset_x, int second_offset_y, int second_offset_z,
    int second_anchor_z,
    u16 longitudinal_scale, u16 lateral_scale,
    u16 flatten) {
    BattleSceneObject *object = BattleSceneObject_GetById((u16)object_id);
    BattleModel *model = object->primary_model;
    BattleSceneObject *anchor;
    BattleSpriteTransform *transform;
    int first_x;
    int first_y;
    int first_z;
    int second_x;
    int second_y;
    int second_z;
    s16 projected_first_z;
    s16 projected_second_z;
    int delta_x;
    int delta_y;
    int delta_z;
    int squared_length;
    int length;

    if (model == 0) {
        return (BattleSceneObject *)model;
    }

    anchor = BattleSceneObject_GetById((u16)first_object_id);
    first_x = (s16)(first_offset_x + anchor->x);
    first_y = (s16)(first_offset_y + anchor->y);
    first_z = (s16)(first_offset_z + anchor->z);

    anchor = BattleSceneObject_GetById(second_object_id);
    second_x = (s16)(second_offset_x + anchor->x);
    second_y = (s16)(second_offset_y + anchor->y);
    second_z = (s16)(second_offset_z + anchor->z);

    if (flatten != 0) {
        first_z = 0;
        second_z = 0;
        first_anchor_z = (s16)(first_anchor_z + 64);
        second_anchor_z = (s16)(second_anchor_z + 64);
    }

    projected_first_z = first_anchor_z + 16 * (256 - first_y);
    if (projected_first_z < 0) {
        projected_first_z = 0;
    }
    projected_second_z = second_anchor_z + 16 * (256 - second_y);
    if (projected_second_z < 0) {
        projected_second_z = 0;
    }

    delta_x =
        (s16)(second_x -
              *(s16 *)(gBattleContext + BATTLE_VIEW_ORIGIN_X_OFFSET)) -
        (s16)(first_x -
              *(s16 *)(gBattleContext + BATTLE_VIEW_ORIGIN_X_OFFSET));
    delta_y =
        (s16)(second_y - second_z -
              *(s16 *)(gBattleContext + BATTLE_VIEW_ORIGIN_Y_OFFSET)) -
        (s16)(first_y - first_z -
              *(s16 *)(gBattleContext + BATTLE_VIEW_ORIGIN_Y_OFFSET));
    delta_z = projected_second_z - projected_first_z;
    squared_length = delta_x * delta_x + delta_y * delta_y;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_length;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    length = *rSQRT_RESULT;

    transform = model->vtable->get_sprite_transform(model);
    transform->matrix[0] =
        _s32_div_f(delta_x << 12, longitudinal_scale);
    transform->matrix[1] =
        _s32_div_f(delta_y << 12, longitudinal_scale);
    transform->matrix[2] = _s32_div_f(delta_z, longitudinal_scale);
    transform->matrix[4] = _s32_div_f(-delta_y * lateral_scale, length);
    transform->matrix[5] = _s32_div_f(delta_x * lateral_scale, length);
    transform->matrix[6] = 0;
    transform->matrix[8] = 0;
    transform->matrix[9] = 0;
    transform->matrix[10] = 4096;

    BattleSceneObject_AdjustPosition(
        object,
        (second_x + first_x) / 2 - object->x,
        (second_y + first_y) / 2 - object->y,
        (second_z + first_z) / 2 - object->z);
    object->effect_anchor_z = (second_anchor_z + first_anchor_z) / 2;
    return object;
}
