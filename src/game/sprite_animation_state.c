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
