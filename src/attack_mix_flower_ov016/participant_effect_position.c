/*
 * Mix Flower effect positioning (overlay 16, 0x020C5914-0x020C5988).
 *
 * Places a participant's effect relative to the participant.
 */

#include "pair_effects_internal.h"

void Overlay16Participant_PositionEffect(Overlay16Participant *participant)
{
    BattleSceneObject *object = participant->object;
    Overlay16AttackTargetParameters *offset = &gOverlay16AttackTargetOffsets[participant->variant].parameters;
    BattleSceneObject *effect = &participant->effect->primary;
    Overlay10Motion_StopAll(effect);
    effect = &participant->effect->primary;
    BattleSceneObject_AdjustPosition(effect, object->x + offset->x - effect->x, object->y + 2 - effect->y,
                                     offset->z - effect->z);
}
