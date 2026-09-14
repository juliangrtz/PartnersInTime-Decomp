#include <game/battle_model_animation.h>
#include <game/model_animation.h>
#include <game/battle_context.h>
#include <game/battle_attack_loader.h>

void func_02010e38(int object, MtxFx44 *matrix, GameMatrixAnimationTrack *track);

enum {
    BATTLE_MODEL_ANIMATION_POOL_OFFSET = 0xD214,
    BATTLE_MODEL_ANIMATION_MODELS_OFFSET = 0x1131C
};

#define BATTLE_ANIMATION_WORKSPACE ((u8 *)data_ov002_020c0660)

BattleModel **BattleModelAnimation_SetModels(BattleModel *first, BattleModel *second,
    BattleModel *third, BattleModel *fourth)
{
    BattleModel **models = (BattleModel **)(BATTLE_ANIMATION_WORKSPACE +
        BATTLE_MODEL_ANIMATION_MODELS_OFFSET);

    if (first == (BattleModel *)-1)
        first = 0;
    models[0] = first;
    if (second != (BattleModel *)-1)
        models[1] = second;
    if (third != (BattleModel *)-1)
        models[2] = third;
    if (fourth != (BattleModel *)-1)
        models[3] = fourth;
    return models;
}

GameMatrixAnimationTrack *BattleModelAnimation_Start(int animation,
    BattleModel *model, int x, int y, s16 z, int speed)
{
    u8 *archive = *(u8 **)(gBattleContext + 0x48);
    const s16 *commands = (const s16 *)(archive + ((u32 *)archive)[animation]);
    GameMatrixAnimationTrack *track;
    GameModelAnimationContext *context;

    /* The shared four-model table retains its other three entries. */
    *(BattleModel **)(BATTLE_ANIMATION_WORKSPACE + BATTLE_MODEL_ANIMATION_MODELS_OFFSET) = model;
    track = GameModelAnimation_Start(
        *(GameModelAnimation **)(BATTLE_ANIMATION_WORKSPACE + BATTLE_MODEL_ANIMATION_POOL_OFFSET),
        commands,
        (FieldRenderObject *const *)(BATTLE_ANIMATION_WORKSPACE + BATTLE_MODEL_ANIMATION_MODELS_OFFSET),
        4, func_02010e38, speed);
    context = (GameModelAnimationContext *)track->context;
    context->offset_x = x;
    context->offset_y = y;
    context->offset_z = z;
    track->speed = speed << 4;
    return track;
}

GameMatrixAnimationTrack *BattleModelAnimation_StartAttached(
    GameMatrixAnimationTrack **owner, int animation, BattleModel *model,
    int x, s16 y, s16 z, int speed)
{
    GameMatrixAnimationTrack *track = BattleModelAnimation_Start(animation, model, x, y, z, speed);
    track->owner = owner;
    *owner = track;
    return track;
}
