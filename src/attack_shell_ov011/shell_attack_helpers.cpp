/*
 * Shell attack helpers (overlay 11, 0x020C2AE0-0x020C2E20).
 *
 * The attached effects the shell drags along: spawning them, following the
 * shell each frame, and reporting when they have all finished, which is what the
 * sequence waits on before moving to the next phase.
 */

#include "shell_internal.h"

/* Align the aim point with the target's left collision bound and the
 * selected particle configuration. Only x/y are written; y is also returned. */
extern "C" int Overlay11Attack_GetTargetPosition(BattlePosition *out, BattleSceneObject *object)
{
    BattleCollisionBounds bounds;
    Overlay11ParticleConfig *config = data_ov002_020c0710->particle_config;
    BattleCollision_GetBounds(&bounds, object->actor_id, 0, 0);
    out->x = object->x + bounds.axis[BATTLE_COLLISION_X_MIN] - config->unknown[7];
    int y = object->y;
    out->y = y;
    return y;
}

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

/* Apply the badge bonus once during attack initialization. The native tables
 * are updated in place for the selected Red/Green Shell variant. */
extern "C" {
extern s16 data_ov011_020c60c0[2][32];
extern s16 data_ov011_020c5f94[2][6];
void Overlay11Attack_ApplyBadgeOffsets()
{
    int variant = data_ov002_020c0710->variant;
    s16 *values = data_ov011_020c6040[variant];
    s16 *offsets = data_ov011_020c60c0[variant];
    for (u32 i = 0; i < 32; ++i)
        values[i] += offsets[i];
    Overlay11ControllerConfig *rows = data_ov011_020c6140[variant];
    s16 *row_offsets = data_ov011_020c5f94[variant];
    for (int i = 0; i < 6; ++i)
        rows[i].initial_y += row_offsets[i];
}
}
