#include <game/model_animation.h>

extern GameModelAnimation *data_0205a8c8;
extern void func_0202cbd4(void *, int, u32);
GameModelAnimationContext *GameModelAnimation_AllocateContext(GameModelAnimation *pool);

void GameModelAnimation_ReleaseContext(GameMatrixAnimationTrack *track)
{
    GameModelAnimationContext *context;
    GameModelAnimationContext *cursor;
    GameModelAnimationContext *previous;
    if (track->owner && *track->owner == track)
        *track->owner = 0;
    context = track->context;
    if (context) {
        data_0205a8c8->free_tail->next = context;
        data_0205a8c8->free_tail = context;
        cursor = data_0205a8c8->active;
        if (cursor != context) {
            previous = 0;
            while (cursor && cursor != context) {
                previous = cursor;
                cursor = cursor->next;
            }
            previous->next = context->next;
        } else {
            data_0205a8c8->active = context->next;
        }
    }
}

GameModelAnimation *GameModelAnimation_Initialize(GameModelAnimation *pool,
    GameMatrixAnimation *animation, GameModelAnimationContext *storage, int count,
    void (*prepare)(BattleModel *, BattleModel *, GameModelAnimationContext *, MtxFx44 *))
{
    GameModelAnimationContext *context;
    int i;
    func_0202cbd4(pool, 0, (u8 *)(pool + 1) - (u8 *)pool);
    func_0202cbd4(storage, 0, sizeof(*storage) * count);
    pool->animation = animation;
    pool->prepare = prepare;
    pool->storage = storage;
    context = pool->storage;
    pool->free_head = context;
    for (i = 0; i < count - 1; ++i, ++context)
        context->next = context + 1;
    context->next = 0;
    pool->free_tail = context;
    pool->active = 0;
    data_0205a8c8 = pool;
    return pool;
}

GameModelAnimation *GameModelAnimation_Finalize(GameModelAnimation *pool)
{
    data_0205a8c8 = 0;
    return pool;
}

GameMatrixAnimationTrack *GameModelAnimation_Start(GameModelAnimation *pool, const s16 *commands,
    BattleModel *const *models, int count,
    void (*draw)(int, MtxFx44 *, GameMatrixAnimationTrack *), int speed)
{
    GameMatrixAnimationTrack *track;
    GameModelAnimationContext *context;
    int i;
    track = GameMatrixAnimation_Start(pool->animation, commands, draw, speed);
    context = GameModelAnimation_AllocateContext(pool);
    func_0202cbd4(&context->unknown_04, 0, 36);
    for (i = 0; i < count; ++i)
        context->models[i] = models[i];
    context->next = pool->active;
    pool->active = context;
    track->context = context;
    track->draw = draw;
    track->finish = GameModelAnimation_ReleaseContext;
    track->owner = 0;
    return track;
}

GameModelAnimationContext *GameModelAnimation_AllocateContext(GameModelAnimation *pool)
{
    GameModelAnimationContext *context = pool->free_head;
    if (context == pool->free_tail)
        return 0;
    pool->free_head = context->next;
    return context;
}
