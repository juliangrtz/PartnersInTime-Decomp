#include <game/matrix_animation.h>
extern const MtxFx44 data_02054f7c;
extern void func_0202cbd4(void *, int, u32);
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);

GameMatrixAnimation *GameMatrixAnimation_Initialize(GameMatrixAnimation *animation, void *storage, int tracks, int matrices) {
    GameMatrixAnimationTrack *track;
    int i;
    func_0202cbd4(animation, 0, 100);
    func_0202cbd4(storage, 0, tracks * 56 + (matrices << 6));
    animation->storage = storage;
    animation->matrices = (MtxFx44 *)(animation->storage + tracks);
    if (&data_02054f7c < animation->matrices)
        func_0202cd2c(&data_02054f7c, animation->matrices, 64);
    else
        func_0202cc58(&data_02054f7c, animation->matrices, 64);
    track = animation->storage;
    animation->free_head = animation->storage;
    for (i = 0; i < tracks - 1; ++i, ++track) track->next = track + 1;
    track->next = 0;
    animation->free_tail = track;
    animation->active = 0;
    return animation;
}
void GameMatrixAnimation_Finalize(void) {}
GameMatrixAnimationTrack *GameMatrixAnimation_Allocate(GameMatrixAnimation *animation) {
    GameMatrixAnimationTrack *track = animation->free_head;
    if (track == animation->free_tail) return 0;
    animation->free_head = track->next;
    return track;
}
GameMatrixAnimationTrack *GameMatrixAnimation_Start(GameMatrixAnimation *animation, const s16 *commands,
    void (*draw)(int, MtxFx44 *, GameMatrixAnimationTrack *), int speed) {
    GameMatrixAnimationTrack *track;
    if (!commands) return 0;
    if (!draw) return 0;
    track = GameMatrixAnimation_Allocate(animation);
    if (track) {
        track->next = animation->active;
        animation->active = track;
        track->commands = commands;
        track->duration = commands[commands[1]];
        track->current_time = speed < 0 ? track->duration : 0;
        track->previous_time = track->current_time - speed;
        track->speed = speed;
        track->flags.bits.loop = 0;
        track->flags.bits.paused = 0;
        track->flags.bits.separate_sequences = 0;
        track->flags.bits.retain = 0;
        track->flags.bits.finished = 0;
        track->draw = draw;
        track->context = 0;
        track->parameters[0] = 0;
        track->parameters[1] = 0;
        track->parameters[2] = 0;
        track->finish = GameMatrixAnimation_ReleaseOwner;
        track->owner = 0;
    }
    return track;
}
