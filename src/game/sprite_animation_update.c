#include <game/sprite_animation.h>
void GameSpriteAnimation_Update(GameSpriteAnimation *animation) {
    GameSpriteAnimationTrack *previous = 0;
    GameSpriteAnimationTrack *track;
    *(vu32 *)0x040004a8 = 0;
    track = animation->active;
    animation->depth = 10;
    while (track) {
        if (track->commands) {
            if (track->flags.bits.loop) {
                if (track->flags.bits.duration) {
                    while (track->time < 0) track->time += track->flags.bits.duration << 8;
                    while ((track->flags.bits.duration << 8) < track->time) track->time -= track->flags.bits.duration << 8;
                }
            } else {
                if (track->time < 0) track->time = 0;
                if ((track->flags.bits.duration << 8) < track->time)
                    track->time = track->flags.bits.duration << 8;
            }
            GameSpriteAnimation_Scan(animation, track);
            if (!track->flags.bits.paused) {
                if (track->flags.bits.loop) track->time += track->speed;
                else if (track->speed > 0 && (track->flags.bits.duration << 8) <= track->time) track->commands = 0;
                else if (track->speed < 0 && track->time <= 0) track->commands = 0;
                else track->time += track->speed;
            }
            previous = track;
            track = track->next;
        } else if (track->finish) {
            track->finish(track);
            track->finish = 0;
            previous = track;
            track = track->next;
        } else {
            animation->free_tail->next = track;
            animation->free_tail = track;
            if (previous) previous->next = track->next;
            else animation->active = track->next;
            track = track->next;
        }
    }
}
