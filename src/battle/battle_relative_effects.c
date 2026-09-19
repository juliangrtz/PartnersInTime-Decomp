/*
 * Object-relative effects (overlay 2, 0x02071E40-0x02072004).
 *
 * Spawns a sprite or model effect positioned relative to an object rather than
 * in world coordinates.
 */

#include <game/battle_context.h>
#include <game/battle_effect.h>

enum BattleRelativeEffectContextOffset {
    BATTLE_RELATIVE_EFFECT_VIEW_X_OFFSET = 0xCB9C,
    BATTLE_RELATIVE_EFFECT_VIEW_Y_OFFSET = 0xCB9E
};

/* Metrowerks emits these contiguous functions in reverse source order. */
/* Full-width offsets are narrowed only at the final spawn call. */
BattleEffect *BattleModelEffect_SpawnRelative(
    int effect_id, BattleSceneObject *reference, BattleSceneObject *parent,
    int offset_x, int offset_y, int offset_z, int scale) {
    BattlePosition position;

    if (reference) {
        BattlePosition_StoreViewRelative(
            &position, reference->x, (s16)(reference->y - reference->z),
            (s16)(reference->effect_anchor_z + 16 * (256 - reference->y)),
            reference->flags.bits.use_raw_position,
            reference->flags.bits.use_alternate_model);
        offset_x += position.x;
        offset_y += position.y;
        offset_z += position.z;
    }
    if (!parent) {
        offset_x += *(s16 *)(gBattleContext + BATTLE_RELATIVE_EFFECT_VIEW_X_OFFSET);
        offset_y += *(s16 *)(gBattleContext + BATTLE_RELATIVE_EFFECT_VIEW_Y_OFFSET);
    }
    return BattleModelEffect_Spawn(
        effect_id, parent, (s16)offset_x, (s16)offset_y, (s16)offset_z, scale);
}

BattleEffect *BattleSpriteEffect_SpawnRelative(
    int effect_id, BattleSceneObject *reference,
    int offset_x, int offset_y, int offset_z, int scale) {
    BattlePosition position;

    if (reference) {
        BattlePosition_StoreViewRelative(
            &position, reference->x, (s16)(reference->y - reference->z),
            (s16)(reference->effect_anchor_z + 16 * (256 - reference->y)),
            reference->flags.bits.use_raw_position,
            reference->flags.bits.use_alternate_model);
        offset_x += position.x;
        offset_y += position.y;
        offset_z += position.z;
    }
    return BattleSpriteEffect_Spawn(
        effect_id, (s16)offset_x, (s16)offset_y, (s16)offset_z, scale);
}
