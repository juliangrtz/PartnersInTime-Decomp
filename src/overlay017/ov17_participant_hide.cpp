#include "participant_internal.h"

extern "C" {
void Overlay17Participant_Initialize(Overlay17Participant *participant, int index)
{
    Overlay17ParticipantState *state = &participant->state;
    participant->state.animation_component = 48 * index + 2444;
    participant->state.tuning = 0;
    participant->state.counter = 0;
    participant->state.bounce = 0;
    participant->state.duration = 0;
    state->target = 0;
    state->bits.phase = 0;
    state->bits.active = 0;
    state->bits.formation = 0;
    state->bits.input = 0;
    state->bits.index = index;
    participant->object.render_state = 32767;
    participant->object.effect_anchor_z = -8;
}

void Overlay17Participant_ReleaseModel(Overlay17Participant *participant)
{
    if (participant->object.primary_model) {
        func_ov002_0206f1b8(participant->object.primary_model);
        if (participant->object.primary_model)
            participant->object.primary_model->unknown_14();
        participant->object.primary_model = 0;
    }
}

Overlay17ParticipantState *Overlay17Participant_Hide(Overlay17Participant *participant)
{
    if (BattleSceneObject_GetActiveModel(&participant->object))
        BattleSceneObject_SetAnimation(&participant->object, -1, -1);
    Overlay10Motion_StopAll(&participant->object);
    participant->state.bits.phase = 0;
    return &participant->state;
}
}
