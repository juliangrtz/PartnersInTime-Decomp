#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_damage_number.h>
#include <game/battle_effect.h>

enum BattleDamageNumberConstant {
    BATTLE_DAMAGE_NUMBER_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_DAMAGE_NUMBER_POPUP_X_OFFSET = 0xCB9C,
    BATTLE_DAMAGE_NUMBER_POPUP_Y_OFFSET = 0xCB9E,
    BATTLE_DAMAGE_NUMBER_ATTACHED_INITIAL = 0x369,
    BATTLE_DAMAGE_NUMBER_ATTACHED_FIRST = 0x36A,
    BATTLE_DAMAGE_NUMBER_ATTACHED_MERGED = 0x36B,
    BATTLE_DAMAGE_NUMBER_ATTACHED_FINAL = 0x36C,
    BATTLE_DAMAGE_NUMBER_ATTACHED_ALTERNATE = 0x36D,
    BATTLE_DAMAGE_NUMBER_EFFECT_ACTIVE = 1 << 3,
    BATTLE_DAMAGE_NUMBER_EFFECT_COMPLETE = 1 << 4,
    BATTLE_DAMAGE_NUMBER_Z = 16,
    BATTLE_DAMAGE_NUMBER_SCALE = 256
};

BattleEffect *BattleDamage_SpawnNumber(int value, int x, int y, int effect_id,
                                       u16 attached_actor_id) {
    s16 popup_x =
        *(s16 *)(gBattleContext + BATTLE_DAMAGE_NUMBER_POPUP_X_OFFSET);
    s16 popup_y =
        *(s16 *)(gBattleContext + BATTLE_DAMAGE_NUMBER_POPUP_Y_OFFSET);
    BattleDamageNumberEffect *effect;

    x += popup_x;
    y += popup_y;

    if (attached_actor_id == 0) {
        effect = (BattleDamageNumberEffect *)BattleModelEffect_Spawn(
            effect_id, 0, (s16)x, (s16)y, BATTLE_DAMAGE_NUMBER_Z,
            BATTLE_DAMAGE_NUMBER_SCALE);
        effect->value = value;
    } else {
        BattleDamageNumberTask *task;

        effect = (BattleDamageNumberEffect *)BattleModelEffect_Spawn(
            BATTLE_DAMAGE_NUMBER_ATTACHED_INITIAL, 0, (s16)x, (s16)y,
            BATTLE_DAMAGE_NUMBER_Z, BATTLE_DAMAGE_NUMBER_SCALE);
        effect->value = value;
        effect->flags |= BATTLE_DAMAGE_NUMBER_EFFECT_ACTIVE;
        task = (BattleDamageNumberTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_DAMAGE_NUMBER_TASK_POOL_OFFSET),
            0);
        {
            BattleDamageNumberPayload *payload = &task->data;
            payload->effect = effect;
            payload->actor_id = attached_actor_id;
        }
        task->callback = BattleDamageNumber_MergeAttached;
    }

    return (BattleEffect *)effect;
}

void BattleDamageNumber_MergeAttached(BattleAITask *base_task) {
    BattleDamageNumberTask *task = (BattleDamageNumberTask *)base_task;
    BattleDamageNumberPayload *payload = &task->data;
    BattleActor *actor;
    BattleDamageNumberEffect *effect;
    int value;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |= 0x2000;
    actor = BattleActor_GetById(payload->actor_id);
    effect = task->effect;
    actor->flags |= BATTLE_ACTOR_FLAG_DAMAGE_NUMBER_PENDING;

    if (!effect->flag_bits.complete) {
        return;
    }

    effect->update_callback = 0;
    task->callback = 0;
    value = effect->value;

    if (actor->damage_number_task != 0) {
        BattleDamageNumberTask *existing_task = actor->damage_number_task;
        BattleDamageNumberEffect *existing_effect = existing_task->effect;
        int existing_value = existing_effect->value;

        existing_effect->update_callback = 0;
        task = existing_task;
        payload = &task->data;
        value += existing_value;
        existing_task->effect =
            (BattleDamageNumberEffect *)BattleModelEffect_Spawn(
                BATTLE_DAMAGE_NUMBER_ATTACHED_MERGED, 0, effect->x, effect->y,
                BATTLE_DAMAGE_NUMBER_Z, BATTLE_DAMAGE_NUMBER_SCALE);
    } else {
        actor->damage_number_task = task;
        payload->effect = (BattleDamageNumberEffect *)BattleModelEffect_Spawn(
            BATTLE_DAMAGE_NUMBER_ATTACHED_FIRST, 0, effect->x, effect->y,
            BATTLE_DAMAGE_NUMBER_Z, BATTLE_DAMAGE_NUMBER_SCALE);
    }

    payload->effect->flags |= BATTLE_DAMAGE_NUMBER_EFFECT_ACTIVE;
    payload->effect->value = value;
    task->callback = BattleDamageNumber_UpdateAttached;
}

void BattleDamageNumber_UpdateAttached(BattleAITask *base_task) {
    BattleDamageNumberTask *task = (BattleDamageNumberTask *)base_task;
    BattleDamageNumberPayload *payload = &task->data;
    BattleActor *actor = BattleActor_GetById(payload->actor_id);
    BattleDamageNumberEffect *effect = task->effect;
    int value;

    if (actor->damage_number_flag_bits.damage_number_pending) {
        actor->flags &= ~BATTLE_ACTOR_FLAG_DAMAGE_NUMBER_PENDING;
        return;
    }

    if (!actor->damage_number_flag_bits.damage_number_alternate) {
        effect->update_callback = 0;
        task->callback = BattleDamageNumber_CleanupAttached;
        value = effect->value;
        BattleModelEffect_SpawnAttached(
            (BattleEffect **)payload, BATTLE_DAMAGE_NUMBER_ATTACHED_FINAL, 0,
            effect->x, effect->y, BATTLE_DAMAGE_NUMBER_Z,
            BATTLE_DAMAGE_NUMBER_SCALE);
        payload->effect->value = value;
        return;
    }

    if (BattleContext_GetRuntimeState()
            ->flags.bits.damage_number_merge_pending) {
        return;
    }
    effect->update_callback = 0;
    task->callback = BattleDamageNumber_CleanupAttached;
    value = effect->value;
    BattleModelEffect_SpawnAttached(
        (BattleEffect **)payload, BATTLE_DAMAGE_NUMBER_ATTACHED_ALTERNATE, 0,
        effect->x, effect->y, BATTLE_DAMAGE_NUMBER_Z,
        BATTLE_DAMAGE_NUMBER_SCALE);
    payload->effect->value = value;
}

void BattleDamageNumber_CleanupAttached(BattleAITask *base_task) {
    BattleDamageNumberTask *task = (BattleDamageNumberTask *)base_task;
    BattleDamageNumberPayload *payload = &task->data;
    BattleActor *actor = BattleActor_GetById(payload->actor_id);

    if (task->effect == 0) {
        task->callback = 0;
        actor->damage_number_task = 0;
    }
}
