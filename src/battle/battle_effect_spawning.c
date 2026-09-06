#include <game/battle_context.h>
#include <game/battle_effect.h>

enum BattleEffectContextOffset {
    BATTLE_MODEL_EFFECT_TASK_POOL_OFFSET = 0x945C,
    BATTLE_SPRITE_RESOURCE_TABLE_OFFSET = 0x44,
    BATTLE_MODEL_RESOURCE_TABLE_OFFSET = 0x48,
    BATTLE_SPRITE_EFFECT_MANAGER_OFFSET = 0xCBF0,
    BATTLE_MODEL_EFFECT_MANAGER_OFFSET = 0xCBF4,
    BATTLE_SPRITE_EFFECT_SLOTS_OFFSET = 0xCBF8,
    BATTLE_MODEL_EFFECT_SLOTS_OFFSET = 0xCCF8,
    BATTLE_EFFECT_VIEW_X_OFFSET = 0xCB9C,
    BATTLE_EFFECT_VIEW_Y_OFFSET = 0xCB9E,
    BATTLE_EFFECT_ANIMATION_FRAME_OFFSET = 0x68D6
};

enum BattleEffectConstant {
    BATTLE_TRACKED_EFFECT_COUNT = 64,
    BATTLE_MODEL_EFFECT_INITIAL_SCALE = 4096,
    BATTLE_RESOURCE_EFFECT_RANDOMIZED_ID = 18,
    BATTLE_RESOURCE_EFFECT_FIRST_FRAME = 18,
    BATTLE_RESOURCE_EFFECT_FRAME_LIMIT = 53,
    BATTLE_RESOURCE_EFFECT_FRAME_MASK = 0xFC0
};

typedef struct BattleModelResourceEffectTask BattleModelResourceEffectTask;

typedef struct BattleModelResourceEffectPayload {
    BattleModel *model;
    u8 unknown_04[0x14];
    s16 x;
    s16 y;
    s16 z;
} BattleModelResourceEffectPayload;

struct BattleModelResourceEffectTask {
    BattleModelResourceEffectTask *next;
    void (*callback)(BattleModelResourceEffectTask *task);
    BattleModelResourceEffectTask **owner_slot;
    BattleModelResourceEffectPayload data;
};

typedef struct BattleEffectRegistry {
    u8 unknown_0000[BATTLE_SPRITE_EFFECT_MANAGER_OFFSET];
    void *sprite_manager;
    void *model_manager;
    BattleEffect *sprite_slots[BATTLE_TRACKED_EFFECT_COUNT];
    BattleEffect *model_slots[BATTLE_TRACKED_EFFECT_COUNT];
} BattleEffectRegistry;

typedef char BattleModelResourceEffectPayload_SizeCheck[
    sizeof(BattleModelResourceEffectPayload) == 0x20 ? 1 : -1];

extern BattleEffect *func_02017d38(
    void *manager, const void *resource,
    int x, int y, int z, int scale);
extern BattleModelEffect *func_02018af4(
    void *manager, const void *resource,
    void (*update_callback)(BattleModelEffect *effect), int initial_scale);
extern void func_ov002_020aaac8(BattleModelEffect *effect);
extern void *func_02029be0(
    u32 size, u32 heap_id, const void *allocator, int argument_3);
extern BattleModel *func_02010634(BattleModel *model);
extern void func_ov002_0206f1f0(
    BattleModel *model, BattleObjectDataLoadState *load_state,
    u32 component_offset);
extern const char data_ov002_020c0318[];

extern void BattleModelEffectTask_Update(
    BattleModelResourceEffectTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleEffect *BattleSpriteEffect_Spawn(
    int effect_id, int x, int y, int z, int scale) {
    u8 *context = gBattleContext;
    const u8 *resource_table =
        *(const u8 **)(context + BATTLE_SPRITE_RESOURCE_TABLE_OFFSET);

    return func_02017d38(
        *(void **)(context + BATTLE_SPRITE_EFFECT_MANAGER_OFFSET),
        resource_table + ((const u32 *)resource_table)[effect_id],
        x + *(s16 *)(context + BATTLE_EFFECT_VIEW_X_OFFSET),
        y + *(s16 *)(context + BATTLE_EFFECT_VIEW_Y_OFFSET),
        z, scale);
}

BattleEffect *BattleSpriteEffect_SpawnAttached(
    BattleEffect **owner_slot, int effect_id,
    int x, int y, s16 z, int scale) {
    BattleEffect *effect =
        BattleSpriteEffect_Spawn(effect_id, x, y, z, scale);

    effect->owner_slot = owner_slot;
    *owner_slot = effect;
    return effect;
}

int BattleSpriteEffect_SpawnInFreeSlot(
    int effect_id, int x, int y, int z, int scale) {
    int index;

    for (index = 0;; index++) {
        if (index == BATTLE_TRACKED_EFFECT_COUNT) {
            return -1;
        }
        if (((BattleEffectRegistry *)gBattleContext)->sprite_slots[index] ==
            0) {
            break;
        }
    }
    /*
     * The original caller forwards z as a full ABI word; the callee consumes
     * its low signed halfword. An unprototyped call preserves that boundary.
     */
    ((BattleEffect *(*)())BattleSpriteEffect_SpawnAttached)(
        &((BattleEffectRegistry *)gBattleContext)->sprite_slots[index],
        effect_id, x, y, z, scale);
    return index;
}

BattleEffect *BattleModelEffect_Spawn(
    int effect_id, BattleSceneObject *parent,
    s16 x, s16 y, s16 z, int scale) {
    u8 *context = gBattleContext;
    const u8 *resource_table =
        *(const u8 **)(context + BATTLE_MODEL_RESOURCE_TABLE_OFFSET);
    BattleModelEffect *effect = func_02018af4(
        *(void **)(context + BATTLE_MODEL_EFFECT_MANAGER_OFFSET),
        resource_table + ((const u32 *)resource_table)[effect_id],
        func_ov002_020aaac8, BATTLE_MODEL_EFFECT_INITIAL_SCALE);

    effect->x = x;
    effect->y = y;
    effect->z = z;
    effect->parent = parent;
    effect->scale_q4 = 16 * scale;
    return (BattleEffect *)effect;
}

BattleEffect *BattleModelEffect_SpawnAttached(
    BattleEffect **owner_slot, int effect_id, BattleSceneObject *parent,
    s16 x, s16 y, s16 z, int scale) {
    BattleEffect *effect =
        BattleModelEffect_Spawn(effect_id, parent, x, y, z, scale);
    BattleModelEffect *model_effect = (BattleModelEffect *)effect;

    model_effect->owner_slot = owner_slot;
    *owner_slot = effect;
    return effect;
}

int BattleModelEffect_SpawnInFreeSlot(
    int effect_id, BattleSceneObject *parent,
    s16 x, s16 y, s16 z, int scale) {
    int index;

    for (index = 0;; index++) {
        if (index == BATTLE_TRACKED_EFFECT_COUNT) {
            return -1;
        }
        if (((BattleEffectRegistry *)gBattleContext)->model_slots[index] ==
            0) {
            break;
        }
    }
    BattleModelEffect_SpawnAttached(
        &((BattleEffectRegistry *)gBattleContext)->model_slots[index],
        effect_id, parent, x, y, z, scale);
    return index;
}

BattleAITask *BattleModelEffect_SpawnFromResource(
    int resource_id, int animation_id,
    int x, int y, s16 z, int scale) {
    BattleModelResourceEffectTask *task =
        (BattleModelResourceEffectTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_MODEL_EFFECT_TASK_POOL_OFFSET),
            0);
    BattleModelResourceEffectPayload *payload = &task->data;
    BattleObjectDataLoadState *load_state;
    BattleModel *model = (BattleModel *)func_02029be0(
        sizeof(BattleModel),
        BattleContext_GetRuntimeState()->resource_heap_id,
        data_ov002_020c0318, 0);

    if (model != 0) {
        model = func_02010634(model);
    }
    payload->model = model;

    load_state = BattleObjectData_GetLoadState(resource_id);
    func_ov002_0206f1f0(
        payload->model, load_state,
        (load_state->flags.bits.resource_index - 1) << 6);
    payload->model->vtable->stop(payload->model);
    payload->model->vtable->set_primary_animation(
        payload->model, animation_id & 0xFF, 0, 1);

    if (resource_id == BATTLE_RESOURCE_EFFECT_RANDOMIZED_ID) {
        ++*(u16 *)(gBattleContext + BATTLE_EFFECT_ANIMATION_FRAME_OFFSET);
        if (*(u16 *)(gBattleContext +
                     BATTLE_EFFECT_ANIMATION_FRAME_OFFSET) >=
            BATTLE_RESOURCE_EFFECT_FRAME_LIMIT) {
            *(u16 *)(gBattleContext + BATTLE_EFFECT_ANIMATION_FRAME_OFFSET) =
                BATTLE_RESOURCE_EFFECT_FIRST_FRAME;
        }
        payload->model->animation_state =
            (payload->model->animation_state &
             ~BATTLE_RESOURCE_EFFECT_FRAME_MASK) |
            ((*(u16 *)(gBattleContext +
                       BATTLE_EFFECT_ANIMATION_FRAME_OFFSET) & 0x3F) << 6);
    } else {
        payload->model->animation_state &=
            ~BATTLE_RESOURCE_EFFECT_FRAME_MASK;
    }

    payload->x = x + *(s16 *)(gBattleContext + BATTLE_EFFECT_VIEW_X_OFFSET);
    payload->y = y + *(s16 *)(gBattleContext + BATTLE_EFFECT_VIEW_Y_OFFSET);
    payload->z = z;
    payload->model->effect_scale = scale;
    task->callback = BattleModelEffectTask_Update;
    return (BattleAITask *)task;
}
