/*
 * Sprite animation tracks (ARM9 resident, 0x020179C0-0x02017EC8).
 *
 * Allocating, starting and stepping a sprite animation track, and the scan that
 * finds the keyframe the current time falls in.
 */

#include <game/sprite_animation.h>
extern void func_0202cbd4(void *, int, u32);
GameSpriteAnimation *GameSpriteAnimation_Initialize(GameSpriteAnimation *animation, void *storage, int tracks, int buffer_size) {
    GameSpriteAnimationTrack *track;
    int i;
    func_0202cbd4(animation, 0, 40);
    func_0202cbd4(storage, 0, tracks * 48 + buffer_size);
    animation->storage = storage;
    animation->buffer = animation->storage + tracks;
    track = animation->storage;
    animation->free_head = track;
    for (i = 0; i < tracks - 1; ++i, ++track) track->next = track + 1;
    track->next = 0;
    animation->free_tail = track;
    animation->active = 0;
    return animation;
}
void GameSpriteAnimation_Finalize(GameSpriteAnimation *animation) {}
GameSpriteAnimationTrack *GameSpriteAnimation_Allocate(GameSpriteAnimation *animation) {
    GameSpriteAnimationTrack *track = animation->free_head;
    if (track == animation->free_tail) return 0;
    animation->free_head = track->next;
    return track;
}
GameSpriteAnimationTrack *GameSpriteAnimation_Start(GameSpriteAnimation *animation, const s16 *commands, int x, int y, int z, int speed) {
    GameSpriteAnimationTrack *track = GameSpriteAnimation_Allocate(animation);
    if (track) {
        track->next = animation->active;
        animation->active = track;
        track->commands = commands;
        track->x = x;
        track->y = y;
        track->z = z;
        track->time = 0;
        track->speed = speed;
        track->flags.bits.duration = 0;
        track->flags.bits.loop = 0;
        track->flags.bits.paused = 0;
        track->parameters[0] = 4096;
        track->parameters[1] = 0;
        track->parameters[2] = 0;
        track->parameters[3] = 4096;
        track->finish = GameSpriteAnimation_ReleaseOwner;
        track->owner = 0;
        GameSpriteAnimation_Scan(animation, track);
        if (speed < 0) track->time = track->flags.bits.duration << 8;
    }
    return track;
}

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

void GameSpriteAnimation_Scan(GameSpriteAnimation *animation, GameSpriteAnimationTrack *track) {
    const s16 *command = track->commands;
    int duration;
    int end;
    if (!command) return;
    duration = 0;
    end = 0;
    for (;;) {
        switch (*command) {
        case 0:
            track->flags.bits.duration = end / 256;
            return;
        case 1:
            duration = command[1] << 8;
            if (duration < 256) duration = 256;
            command += 2;
            animation->commands = command;
            break;
        case 2: {
            int time;
            if (end < duration + (command[1] << 8)) end = duration + (command[1] << 8);
            time = command[1] << 8;
            if (track->flags.bits.duration) {
                int elapsed = track->time - time;
                if (elapsed >= 0 && elapsed <= duration) {
                    animation->time = elapsed;
                    animation->fraction = 4 * elapsed / (duration / 256);
                    func_02016c0c(animation, track, animation->commands);
                }
            }
            command += 2;
            break;
        }
        case 15: command += command[1] * 2 + 2; break;
        case 16: command += 4; break;
        case 17: command += 4; break;
        default: command += 2; break;
        }
    }
}
