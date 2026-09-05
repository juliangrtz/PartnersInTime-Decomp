#include <game/battle_effect.h>
#include <hardware.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

void BattleTransform_BuildBetweenPoints(
    BattleSpriteTransform *transform,
    s16 start_x, s16 start_y, s16 start_z,
    s16 end_x, s16 end_y, s16 end_z,
    u16 longitudinal_scale, u16 lateral_scale) {
    int delta_x = end_x - start_x;
    int delta_y = end_y - start_y;
    int squared_length = delta_x * delta_x + delta_y * delta_y;
    int delta_z = end_z - start_z;
    int length;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_length;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    length = *rSQRT_RESULT;

    transform->matrix[0] =
        _s32_div_f(delta_x << 12, longitudinal_scale);
    transform->matrix[1] =
        _s32_div_f(delta_y << 12, longitudinal_scale);
    transform->matrix[2] = _s32_div_f(delta_z, longitudinal_scale);
    transform->matrix[3] = 0;
    transform->matrix[4] = _s32_div_f(-delta_y * lateral_scale, length);
    transform->matrix[5] = _s32_div_f(delta_x * lateral_scale, length);
    transform->matrix[6] = 0;
    transform->matrix[7] = 0;
    transform->matrix[8] = 0;
    transform->matrix[9] = 0;
    transform->matrix[10] = 4096;
    transform->matrix[11] = 0;
    transform->x = ((end_x + start_x) / 2) << 8;
    transform->y = ((end_y + start_y) / 2) << 8;
    transform->z = ((end_z + start_z) / 2) << 4;
    *(s32 *)transform->unknown_3c = 4096;
}
