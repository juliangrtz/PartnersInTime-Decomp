/*
 * Field sprite animation start (overlay 0, 0x020BF2E0-0x020BF33C).
 *
 * Starts a field sprite animation.
 */

#include <game/field_animation.h>

extern void func_ov000_020bf33c(FieldSpriteAnimation *state);

GameSpriteAnimationTrack *FieldSpriteAnimation_Start(FieldSpriteAnimation *state, int index,
    const s16 *commands, int x, s16 y, s16 speed)
{
    GameSpriteAnimationTrack *track;
    func_ov000_020bf33c(state);
    track = GameSpriteAnimation_Start(state->animation, commands, x, y, 0, speed);
    state->tracks[index] = track;
    return track;
}
