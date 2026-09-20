/* Reserve a paired flame, choose its kind, and start the one-time visuals.
 * When the pool is full, the participant updater retries using the saved
 * animation flag. Kind -1 selects kind 2 for the active participant, 1 otherwise.
 */
#include "pair_effects_internal.h"
extern "C" {
void Overlay16Participant_PrepareEffect(Overlay16Participant *participant, int kind, int animate)
{
    BattleSceneObject *object = participant->object;
    Overlay16AttackWork *work = data_ov002_020c0710;
    if (!participant->effect) {
        int index = Overlay16PairEffect_FindFree();
        if (index == -1) {
            participant->pending_effect = 1;
            participant->pending_animate = animate;
            return;
        }
        participant->effect = &work->effects[index];
    }
    if (kind == -1)
        kind = work->active == participant ? 2 : 1;
    if (animate)
        Overlay16Participant_ChangeEffectKind(participant, kind);
    else
        Overlay16PairEffect_BeginKind(participant->effect, kind);
    if (!participant->effect_started) {
        const s8 *offset;
        int sprite_effect, model_effect;
        if (kind >= 2) {
            offset = (const s8 *)&gOverlay16AttackTargetOffsets[participant->variant] + 7;
            sprite_effect = 539;
            model_effect = 846;
        } else {
            offset = (const s8 *)&gOverlay16AttackTargetOffsets[participant->variant];
            sprite_effect = 542;
            model_effect = 847;
        }
        BattleSpriteEffect_SpawnRelative(sprite_effect, object, offset[0], -offset[1], 0, 256);
        BattleModelEffect_SpawnAttached(&participant->attached_effect, model_effect,
                                       &participant->effect->primary, 0, 0, 0, 256);
        BattleSound_Play(0, 0, 0, 0);
        participant->effect_started = 1;
    }
}
}
