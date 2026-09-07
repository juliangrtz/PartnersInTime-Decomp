#include <game/matrix_animation.h>

void GameMatrixAnimation_ReleaseOwner(GameMatrixAnimationTrack *track) {
    if (track->owner && *track->owner == track) *track->owner = 0;
}
