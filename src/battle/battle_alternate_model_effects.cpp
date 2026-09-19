extern "C" {
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/sprite_output.h>
#include <game/heap.h>
extern char data_ov002_020be74c[];
void func_ov002_0206f3cc(BattleModel *model, BattleObjectDataLoadState *resource,
    GameSpritePalette *palette, int resource_id);
}

enum {
    BATTLE_ALTERNATE_MODEL_BYTES = 304,
    BATTLE_EFFECT_TASK_POOL_OFFSET = 0x945C,
    BATTLE_ALTERNATE_MODEL_HEAP_OFFSET = 0xE15C,
    BATTLE_EFFECT_CAMERA_X_OFFSET = 0xCB9C,
    BATTLE_EFFECT_CAMERA_Y_OFFSET = 0xCB9E,
    BATTLE_EFFECT_ORIGIN_X_OFFSET = 0xCBA0,
    BATTLE_EFFECT_ORIGIN_Y_OFFSET = 0xCBA2
};
struct BattleAlternateModelEffectPayload {
    BattleModel *model;
    GameSpritePalette palette;
    s16 x, y, z;
};
struct BattleAlternateModelEffectTask {
    BattleAlternateModelEffectTask *next;
    void (*callback)(BattleAlternateModelEffectTask *);
    BattleAlternateModelEffectTask **owner;
    BattleAlternateModelEffectPayload data;
};
typedef char BattleAlternateModelEffectPayload_SizeCheck[sizeof(BattleAlternateModelEffectPayload) == 32 ? 1 : -1];
typedef char BattleAlternateModelEffectTask_SizeCheck[sizeof(BattleAlternateModelEffectTask) == 44 ? 1 : -1];

/* The outer owner check and nullable deleting call are separate native guards. */
static inline void DeleteModel(BattleModel *model)
{
    if (model)
        model->unknown_14();
}

static inline void SetPosition(BattleModel *model, s16 x, s16 y)
{
    model->animation_offset_x = x;
    model->animation_offset_y = y;
}

extern "C" {
void BattleAlternateModelEffect_Update(BattleAlternateModelEffectTask *task);

BattleAITask *BattleAlternateModelEffect_Spawn(int resource_id, int animation_id,
    int x, int y, s16 z, int scale)
{
    BattleAlternateModelEffectTask *task = (BattleAlternateModelEffectTask *)BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext + BATTLE_EFFECT_TASK_POOL_OFFSET), 0);
    BattleAlternateModelEffectPayload *payload = &task->data;
    BattleModel *model = (BattleModel *)GameHeap_New(BATTLE_ALTERNATE_MODEL_BYTES,
        *(int *)(gBattleContext + BATTLE_ALTERNATE_MODEL_HEAP_OFFSET), data_ov002_020be74c, 0);
    BattleObjectDataLoadState *resource;

    if (model)
        model = BattleModelController_InitBase(model);
    payload->model = model;
    resource = BattleObjectData_GetLoadState((u16)resource_id);
    func_ov002_0206f3cc(payload->model, resource, &payload->palette, resource->resource_id);
    payload->model->set_primary_animation(animation_id & 0xFF, 0, 1);
    SetPosition(payload->model, x, y);
    payload->x = x + *(s16 *)(gBattleContext + BATTLE_EFFECT_ORIGIN_X_OFFSET);
    payload->y = y + *(s16 *)(gBattleContext + BATTLE_EFFECT_ORIGIN_Y_OFFSET);
    payload->z = z;
    payload->model->effect_scale = scale;
    payload->model->render_anchor_z = z;
    task->callback = BattleAlternateModelEffect_Update;
    return (BattleAITask *)task;
}

void BattleAlternateModelEffect_Update(BattleAlternateModelEffectTask *task)
{
    BattleAlternateModelEffectPayload *payload = &task->data;
    s16 x = payload->x - *(s16 *)(gBattleContext + BATTLE_EFFECT_CAMERA_X_OFFSET);
    s16 y = payload->y - *(s16 *)(gBattleContext + BATTLE_EFFECT_CAMERA_Y_OFFSET);
    s16 z = payload->z < 0 ? 0 : payload->z;
    {
        BattleModel *model = payload->model;
        model->animation_offset_x = x;
        model->animation_offset_y = y;
    }
    payload->model->render_anchor_z = z;
    if (!payload->model->flag_bits.panel_animation_trigger)
        return;
    GameSpritePalette_Unlink(&payload->palette);
    payload->model->stop();
    if (payload->model) {
        DeleteModel(payload->model);
        payload->model = 0;
    }
    task->callback = 0;
}

BattleAITask *BattleAlternateModelEffect_SpawnAttached(BattleAITask **owner,
    int resource_id, int animation_id, int x, s16 y, s16 z, int scale)
{
    BattleAITask *task = BattleAlternateModelEffect_Spawn(resource_id, animation_id, x, y, z, scale);
    BattleTask_BindOwnerSlot(task, owner);
    return task;
}
}

extern "C" {

#include "battle_model_slots_internal.h"
int BattleAlternateModelEffect_SpawnInFreeSlot(int resource, int animation, int x, s16 y, s16 z,
                                               int scale) {
    int slot;
    for (slot = 0;; ++slot) {
        if (slot == 64)
            return -1;
        if (!SLOTS->effects[slot])
            break;
    }
    BattleAlternateModelEffect_SpawnAttached(&SLOTS->effects[slot], resource, animation, x, y, z,
                                             scale);
    return slot;
}

}
