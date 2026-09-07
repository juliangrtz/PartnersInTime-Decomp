#include <game/field_animation.h>
#include <nitro/gx_init.h>

void FieldSpriteAnimation_CancelTrack(FieldSpriteAnimation *state, int index)
{
    GameSpriteAnimationTrack *track = state->tracks[index];
    if (track) track->commands = 0;
    state->tracks[index] = 0;
}

void FieldSpriteAnimation_Stop(FieldSpriteAnimation *state)
{
    int index;
    if (!state->flags.active) return;
    for (index = 0; index < 2; ++index) FieldSpriteAnimation_CancelTrack(state, index);
    if (!state->flags.owns_blend) *(vu16 *)0x04000050 &= ~1;
    else *(vu16 *)0x04000050 = 0;
    *(vu32 *)0x04000540 = 1;
    GX_SetGraphicsMode(1, 0, 0);
    state->field_system->render_descriptor->flags &= ~0x02000000;
    state->flags.active = 0;
}
