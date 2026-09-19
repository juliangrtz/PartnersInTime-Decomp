/*
 * Pocket Chomp anchor (overlay 18, 0x020C3CFC-0x020C3EA8).
 *
 * The offset an object is attached at relative to the Chomp, and the attachment
 * itself.
 */

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
    BattleObjectLink_InitializeTrail(object, linked, 2560, 60, 8);
    BattleObjectLink_SetGravity(object, 256);
    BattleObjectLink_SetParameter0A(object, 256);
    BattleObjectLink_SetParameter0C(object, -8);
    BattleObjectLink_SetParameter0E(object, 2);
    BattleObjectLink_SetFollowing(object, 1, 0);
    PocketChomp_GetAnchorOffset(offset, center);
    return (int)BattleObjectLink_SetSourceOffset(object, offset[0], -1, 22 - offset[1]);
}
}
