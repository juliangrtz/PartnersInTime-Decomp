/*
 * Draw successive copies of a model along relative trail samples.
 * Save the matrix and animation fields around rendering. The reset option
 * clears the first three rows only; the existing translation row survives.
 * Each sample's depth uses signed division toward zero, not an arithmetic shift.
 */
#include <game/battle_scene.h>
extern "C" {
extern MtxFx44 data_ov002_020c0f2c;
void func_0202cc58(const void *, void *, u32);
void func_0202cd2c(const void *, void *, u32);
void MTX_TransApply44(const MtxFx44 *, MtxFx44 *, s32, s32, s32);

/* Each 12-byte sample stores a relative translation followed by unused fields. */
struct BattleTrailPoint {
    s16 x, y, z;
    s16 unknown06[3];
};
typedef char BattleTrailPointSize[sizeof(BattleTrailPoint) == 12 ? 1 : -1];

void BattleTrail_DrawModelCopies(const BattleTrailPoint *points, int count, BattleModel *model,
                       s16 animation, int x, int y, int z, int reset)
{
    MtxFx44 *transform;
    s16 saved_animation, saved_property;
    int i;
    if (model->animation_state_bits.state < 1) return;
    transform = (MtxFx44 *)model->get_sprite_transform();
    if ((u32)transform < (u32)&data_ov002_020c0f2c)
        func_0202cd2c(transform, &data_ov002_020c0f2c, 64);
    else
        func_0202cc58(transform, &data_ov002_020c0f2c, 64);
    saved_animation = model->animation_id;
    saved_property = model->property_056;
    model->animation_id = animation;
    model->property_056 = 0;
    if (reset) {
        transform->_00 = 4096; transform->_01 = 0; transform->_02 = 0; transform->_03 = 0;
        transform->_10 = 0; transform->_11 = 4096; transform->_12 = 0; transform->_13 = 0;
        transform->_20 = 0; transform->_21 = 0; transform->_22 = 4096; transform->_23 = 0;
    }
    MTX_TransApply44(transform, transform, (s32)((u32)x << 8), (s32)((u32)y << 8), z);
    for (i = 0; i < count; ++i) {
        MTX_TransApply44(transform, transform, points->x, points->y - points->z, -points->y / 16);
        model->draw(0, 0, 0);
        ++points;
    }
    if ((u32)&data_ov002_020c0f2c < (u32)transform)
        func_0202cd2c(&data_ov002_020c0f2c, transform, 64);
    else
        func_0202cc58(&data_ov002_020c0f2c, transform, 64);
    model->animation_id = saved_animation;
    model->property_056 = saved_property;
}
}
