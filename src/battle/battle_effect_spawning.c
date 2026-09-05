#include <game/battle_context.h>
#include <game/battle_effect.h>

enum BattleEffectContextOffset {
    BATTLE_SPRITE_RESOURCE_TABLE_OFFSET = 0x44,
    BATTLE_MODEL_RESOURCE_TABLE_OFFSET = 0x48,
    BATTLE_SPRITE_EFFECT_MANAGER_OFFSET = 0xCBF0,
    BATTLE_MODEL_EFFECT_MANAGER_OFFSET = 0xCBF4,
    BATTLE_SPRITE_EFFECT_SLOTS_OFFSET = 0xCBF8,
    BATTLE_MODEL_EFFECT_SLOTS_OFFSET = 0xCCF8,
    BATTLE_EFFECT_VIEW_X_OFFSET = 0xCB9C,
    BATTLE_EFFECT_VIEW_Y_OFFSET = 0xCB9E
};

enum BattleEffectConstant {
    BATTLE_TRACKED_EFFECT_COUNT = 64,
    BATTLE_MODEL_EFFECT_INITIAL_SCALE = 4096
};

typedef struct BattleModelEffect {
    u8 unknown_00[0x12];
    s16 scale_q4;
    s16 x;
    s16 y;
    s16 z;
    u8 unknown_1a[0x12];
    BattleSceneObject *parent;
    u8 unknown_30[4];
    BattleEffect **owner_slot;
} BattleModelEffect;

typedef struct BattleEffectRegistry {
    u8 unknown_0000[BATTLE_SPRITE_EFFECT_MANAGER_OFFSET];
    void *sprite_manager;
    void *model_manager;
    BattleEffect *sprite_slots[BATTLE_TRACKED_EFFECT_COUNT];
    BattleEffect *model_slots[BATTLE_TRACKED_EFFECT_COUNT];
} BattleEffectRegistry;

typedef char BattleModelEffect_SizeCheck[
    sizeof(BattleModelEffect) == 0x38 ? 1 : -1];

extern BattleEffect *func_02017d38(
    void *manager, const void *resource,
    int x, int y, int z, int scale);
extern BattleModelEffect *func_02018af4(
    void *manager, const void *resource,
    void (*update_callback)(BattleModelEffect *effect), int initial_scale);
extern void func_ov002_020aaac8(BattleModelEffect *effect);

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
