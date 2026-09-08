#include "pocket_chomp_internal.h"

extern "C" {
int PocketChomp_GetAnchorOffset(s16 *offset, PocketChomp *center)
{
    int angle = 30947;
    if (center->bits.direction)
        angle = 1821;
    BattleModel *model = BattleSceneObject_GetActiveModel(center->object);
    if (model)
        angle += model->rotation_z;
    offset[0] = 24 * FX_SinCosTable_[2 * ((angle & 65535) >> 4) + 1] / 4096;
    int y = 24 * FX_SinCosTable_[2 * ((angle & 65535) >> 4)] / 4096;
    offset[1] = y;
    return y;
}

int PocketChomp_AttachObject(PocketChomp *center, int align)
{
    BattleSceneObject *object = center->object, *linked = center->linked_object;
    s16 offset[4];
    if (align)
        BattleSceneObject_AdjustPosition(linked, object->x - linked->x, object->y - linked->y,
                                         object->z - linked->z);
    else
        BattleSceneObject_AdjustPosition(linked, object->x + 22 - linked->x, object->y - linked->y,
                                         object->z - linked->z);
    func_ov002_020b8b14(object, linked, 2560, 60, 8);
    func_ov002_020b8a58(object, 256);
    func_ov002_020b8a3c(object, 256);
    func_ov002_020b8a18(object, -8);
    func_ov002_020b8a08(object, 2);
    func_ov002_020b8ac8(object, 1, 0);
    PocketChomp_GetAnchorOffset(offset, center);
    return func_ov002_020b8ab0(object, offset[0], -1, 22 - offset[1]);
}
}
