#include "flower_internal.h"

extern "C" {
void Overlay14Participant_StartFade(Overlay14Participant *participant)
{
    Overlay14Fade *fade = (Overlay14Fade *)((u8 *)participant + 28);
    participant->actor->actor.scene_object->render_override = (BattleSceneRenderOverride *)fade;
    fade->render = func_ov014_020c2bb4;
    ((Overlay14ParticipantFade *)participant)->frame = 0;
}

void Overlay14Participant_EndFade(Overlay14Participant *participant)
{
    ((Overlay14ParticipantFade *)participant)->stop = 1;
}
}
