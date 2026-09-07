#include <game/sprite_animation.h>
u32 GameSpriteAnimation_PackColor(const s32 *rgb) {
    int red = rgb[0];
    int green = rgb[1];
    int blue = rgb[2];
    if (red > 31) red = 31; else if (red < 0) red = 0;
    if (green > 31) green = 31; else if (green < 0) green = 0;
    if (blue > 31) blue = 31; else if (blue < 0) blue = 0;
    return red | (green << 5) | (blue << 10);
}
void GameSpriteAnimation_ReleaseOwner(GameSpriteAnimationTrack *track) {
    if (track->owner && *track->owner == track) *track->owner = 0;
}
