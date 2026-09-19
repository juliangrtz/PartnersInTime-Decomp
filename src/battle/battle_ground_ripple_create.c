/*
 * Ground ripple (overlay 2, 0x020AE940-0x020AE9C0).
 *
 * Creates the ripple drawn on the ground under an impact.
 */

#include <game/battle_effect_controls.h>
#include <game/battle_scene.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
typedef struct GroundRippleState {
    s16 x, y;
    u16 object_id;
    s16 radius, growth_frames, hold_frames, timer;
} GroundRippleState;
typedef struct GroundRippleTask {
    void *next;
    void (*update)(struct GroundRippleTask *);
    void *owner;
    GroundRippleState state;
    u8 unused[18];
} GroundRippleTask;
typedef char GroundRippleTaskSize[sizeof(GroundRippleTask) == 44 ? 1 : -1];
void func_ov002_020ae37c(GroundRippleTask *);
BattleAITask *BattleGroundRipple_Create(BattleSceneObject *object, int radius, int growth_frames,
                                        int hold_frames) {
    GroundRippleTask *task =
        (GroundRippleTask *)BattleTaskList_Insert((BattleTaskPool *)(gBattleContext + 37980), 0);
    GroundRippleState *state = &task->state;
    task->state.x = object->x;
    state->y = object->y;
    state->object_id = object->actor_id;
    state->radius = radius;
    state->growth_frames = growth_frames;
    state->hold_frames = hold_frames;
    state->timer = 0;
    task->update = func_ov002_020ae37c;
    return (BattleAITask *)task;
}
