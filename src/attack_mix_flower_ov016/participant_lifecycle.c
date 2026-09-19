/*
 * Mix Flower participant lifecycle (overlay 16, 0x020C6560-0x020C65C4).
 *
 * Initializes a participant and hides it again.
 */

#include "pair_effects_internal.h"

void Overlay16Participant_Initialize(Overlay16Participant *participant, BattleSceneObject *object,
                                     int variant, int component)
{
    participant->object = object;
    participant->effect = 0;
    participant->unknown12 = 0;
    participant->component = component;
    participant->timer2 = 0;
    participant->phase = 0;
    participant->variant = (u8)variant;
    participant->unknown_flag7 = 0;
}

void Overlay16Participant_Hide(Overlay16Participant *participant)
{
    BattleSceneObject_SetAnimation(participant->object, -1, -1);
}
