#include "shell_internal.h"

extern "C" {
int Overlay11Attack_GetSpeed(int delta)
{
    int index = data_ov002_020c0710->speed_index + delta;
    if (index < 0)
        index = 0;
    if ((u32)index >= 32)
        index = 31;
    return data_ov011_020c6040[data_ov002_020c0710->variant][index];
}

void Overlay11Effects_SpawnAttached(int effect_id, BattleSceneObject *object)
{
    Overlay11AttachedEffect *entry = data_ov002_020c0710->effects;
    int i;
    for (i = 0; i < 16; ++i, ++entry) {
        if (!entry->effect)
            break;
    }
    if (i < 16) {
        BattlePosition position;
        BattlePosition_StoreViewRelative(&position, object->x, (s16)(object->y - object->z),
                                         (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
                                         object->flags.bits.use_raw_position,
                                         (object->flags.raw << 17) >> 31);
        BattleSpriteEffect_SpawnAttached(&entry->effect, effect_id, position.x, position.y, position.z, 256);
        entry->object = object;
    }
}

void Overlay11Effects_UpdatePositions()
{
    if (data_ov002_020c0710) {
        Overlay11AttachedEffect *entry = data_ov002_020c0710->effects;
        for (int i = 0; i < 16; ++i, ++entry) {
            if (entry->effect) {
                BattleSceneObject *object = entry->object;
                BattlePosition position;
                BattlePosition_StoreViewRelative(&position, object->x, (s16)(object->y - object->z),
                                                 (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
                                                 object->flags.bits.use_raw_position,
                                                 (object->flags.raw << 17) >> 31);
                entry->effect->x = position.x;
                entry->effect->y = position.y;
                entry->effect->z = position.z;
            }
        }
    }
}

int Overlay11Effects_AreFinished()
{
    if (!data_ov002_020c0710)
        return 0;
    Overlay11AttachedEffect *entry = data_ov002_020c0710->effects;
    for (int i = 0; i < 16; ++i, ++entry) {
        if (entry->effect)
            return 0;
    }
    return 1;
}
}
