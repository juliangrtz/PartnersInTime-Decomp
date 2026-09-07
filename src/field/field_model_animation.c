#include <game/field_animation.h>
#include <game/heap.h>

extern void func_0202cbd4(void *destination, int value, u32 size);
extern void func_ov000_020bee20(FieldRenderObject *, FieldRenderObject *, GameModelAnimationContext *, MtxFx44 *);
extern void func_02010e38(int, MtxFx44 *, GameMatrixAnimationTrack *);

FieldModelAnimation *FieldModelAnimation_Init(FieldModelAnimation *state)
{
    GameMatrixAnimation *matrix;
    GameModelAnimation *models;
    func_0202cbd4(state, 0, sizeof(*state));
    matrix = GameHeap_New(sizeof(*matrix), 1, 0, 0);
    if (matrix) matrix = GameMatrixAnimation_Initialize(matrix, state, 32, 32);
    state->matrix_animation = matrix;
    /* The original reserves four bytes beyond the 24-byte pool. */
    models = GameHeap_New(28, 1, 0, 0);
    if (models) models = GameModelAnimation_Initialize(models, state->matrix_animation,
        state->contexts, 32, func_ov000_020bee20);
    state->model_animation = models;
    return state;
}

FieldModelAnimation *FieldModelAnimation_Destroy(FieldModelAnimation *state)
{
    GameModelAnimation *models = state->model_animation;
    if (models) {
        GameModelAnimation_Finalize(models);
        GameHeap_Delete(models);
    }
    {
        GameMatrixAnimation *matrix = state->matrix_animation;
        if (matrix) {
            GameMatrixAnimation_Finalize(matrix);
            GameHeap_Delete(matrix);
        }
    }
    return state;
}

GameMatrixAnimationTrack *FieldModelAnimation_Start(FieldModelAnimation *state, int index,
    const s16 *commands, FieldRenderObject **models, FieldRenderObject *first, FieldRenderObject *second,
    FieldRenderObject *third, FieldRenderObject *fourth, s16 x, s16 y, s16 z, int overlap_priority, int speed)
{
    GameMatrixAnimationTrack *track;
    GameModelAnimationContext *context;
    FieldModelAnimation_SetModels(state, models, first, second, third, fourth);
    track = GameModelAnimation_Start(state->model_animation, commands, models, 4, func_02010e38, speed);
    context = track->context;
    context->offset_x = x;
    context->offset_y = y;
    context->offset_z = z;
    context->overlap_priority = overlap_priority;
    context->flags.overlap_priority = overlap_priority;
    track->speed = speed * 16;
    state->tracks[index] = track;
    return track;
}

void FieldModelAnimation_SetModels(FieldModelAnimation *state, FieldRenderObject **models,
    FieldRenderObject *first, FieldRenderObject *second, FieldRenderObject *third, FieldRenderObject *fourth)
{
    if (first != (FieldRenderObject *)-1) models[0] = first;
    if (second != (FieldRenderObject *)-1) models[1] = second;
    if (third != (FieldRenderObject *)-1) models[2] = third;
    if (fourth != (FieldRenderObject *)-1) models[3] = fourth;
}
