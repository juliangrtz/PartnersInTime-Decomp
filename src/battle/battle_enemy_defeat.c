#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_enemy_data.h>
#include <game/battle_enemy_defeat.h>
#include <game/battle_hit.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>
#include <game/item.h>
#include <game/save_data.h>

enum BattleEnemyDefeatConstant {
    BATTLE_ENEMY_DEFEAT_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_ENEMY_HIT_QUEUE_OFFSET = 0xCAD8,
    BATTLE_ACTIVE_ACTOR_ID_OFFSET = 0x20,
    BATTLE_EARNED_COINS_OFFSET = 0x84,
    BATTLE_EARNED_EXPERIENCE_OFFSET = 0x86,
    BATTLE_BONUS_COINS_OFFSET = 0x88,
    BATTLE_BONUS_EXPERIENCE_OFFSET = 0x8A,
    BATTLE_REWARD_ITEM_IDS_OFFSET = 0x8C,
    BATTLE_REWARD_ITEM_COUNTS_OFFSET = 0x9C,
    BATTLE_GUARANTEED_REWARD_ITEM_COUNTS_OFFSET = 0xAC,
    BATTLE_TARGET_MODE_OFFSET = 0x112,
    BATTLE_TARGET_MODE_ACTION_ITEM = 3,
    BATTLE_REWARD_LIMIT = 9999,
    BATTLE_REWARD_ITEM_LIMIT = 99,
    BATTLE_REWARD_ITEM_CAPACITY = 8,
    BATTLE_EFFECT_SCALE = 256,
    SAVE_EQUIPMENT_RECORD_SIZE = 0x24,
    SAVE_EQUIPMENT_EFFECT_OFFSET = 0x418
};

enum BattleEnemyDefeatEffectKind {
    BATTLE_ENEMY_DEFEAT_EFFECT_STANDARD = 0,
    BATTLE_ENEMY_DEFEAT_EFFECT_ALTERNATE = 1,
    BATTLE_ENEMY_DEFEAT_EFFECT_NONE = 3,
    BATTLE_ENEMY_DEFEAT_EFFECT_DELAYED = 4
};

enum BattleEnemyDefeatEquipmentEffect {
    EQUIPMENT_EFFECT_DOUBLE_COINS = 0x3006,
    EQUIPMENT_EFFECT_EXPERIENCE_120_PERCENT = 0x3007,
    EQUIPMENT_EFFECT_TRIPLE_COINS = 0x3017,
    EQUIPMENT_EFFECT_EXPERIENCE_140_PERCENT = 0x3018,
    EQUIPMENT_EFFECT_GUARANTEED_ITEM_DROP = 0x301E
};

typedef struct BattleEnemyDefeatPayload {
    BattleSceneObject *object;
    s32 timer;
} BattleEnemyDefeatPayload;

typedef struct BattleEnemyDefeatTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleEnemyDefeatPayload data;
} BattleEnemyDefeatTask;

typedef struct BattleEnemyRuntimeResource {
    BattleEnemyStatRecord *stats;
} BattleEnemyRuntimeResource;

typedef char BattleEnemyDefeatPayload_SizeCheck
    [sizeof(BattleEnemyDefeatPayload) == 8 ? 1 : -1];
typedef char
    BattleEnemyDefeatTask_SizeCheck[sizeof(BattleEnemyDefeatTask) == 0x14 ? 1
                                                                          : -1];

extern u32 Random_NextModulo(u32 modulus);

#define VOLATILE_BATTLE_CONTEXT (*(u8 *volatile *)&gBattleContext)

void BattleEnemy_UpdateDefeatRemoval(BattleAITask *base_task);
void BattleEnemy_UpdateDelayedDefeatRemoval(BattleAITask *base_task);

BattleAITask *BattleEnemy_StartDefeat(BattleSceneObject *object) {
    BattleEnemyDefeatTask *task;
    BattleEnemyDefeatPayload *payload;
    BattleEnemyActor *enemy;
    BattleHitRecord *hit;
    BattlePosition position;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |= 1 << 2;
    task = (BattleEnemyDefeatTask *)BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext +
                           BATTLE_ENEMY_DEFEAT_TASK_POOL_OFFSET),
        0);
    payload = &task->data;
    task->callback = BattleEnemy_UpdateDefeatRemoval;
    payload->object = object;
    enemy = (BattleEnemyActor *)BattleActor_GetEnemySlot(object->actor_id);

    BattleHitDescriptor_DisableByActor(object->actor_id);
    hit = (BattleHitRecord *)(gBattleContext + BATTLE_ENEMY_HIT_QUEUE_OFFSET);
    if (hit->kind != 0) {
        do {
            if (hit->source_id == object->actor_id) {
                hit->source_id = 0;
            }
            hit++;
        } while (hit->kind != 0);
    }
    hit = (BattleHitRecord *)(gBattleContext + BATTLE_ENEMY_HIT_QUEUE_OFFSET);
    if (hit->kind != 0) {
        do {
            if (hit->target_id == object->actor_id) {
                hit->target_id = 0;
            }
            hit++;
        } while (hit->kind != 0);
    }

    BattleStatus_ClearAll(&enemy->actor);
    BattlePosition_StoreViewRelative(
        &position, object->x, (s16)(object->y - object->z),
        (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
        object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model);

    if (enemy->actor.resource_slot != 0 &&
        enemy->defeat_effect_kind != BATTLE_ENEMY_DEFEAT_EFFECT_NONE &&
        enemy->defeat_effect_kind != BATTLE_ENEMY_DEFEAT_EFFECT_DELAYED) {
        BattleEnemyStatRecord *stats =
            ((BattleEnemyRuntimeResource *)enemy->actor.resource_slot)->stats;
        int guaranteed_item_drop = 0;
        u16 active_actor_id =
            *(u16 *)(gBattleContext + BATTLE_ACTIVE_ACTOR_ID_OFFSET);

        if (active_actor_id >= BATTLE_ACTOR_PARTY_FIRST &&
            active_actor_id <
                BATTLE_ACTOR_PARTY_FIRST + BATTLE_ACTOR_PARTY_COUNT &&
            *(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                BATTLE_TARGET_MODE_ACTION_ITEM) {
            BattlePartyActor *party_actor =
                (BattlePartyActor *)BattleActor_GetPartySlot(active_actor_id);
            u8 *equipment_record =
                gSaveData + SAVE_EQUIPMENT_RECORD_SIZE * (u8) *
                                (u16 *)party_actor->actor.resource_slot;
            int equipment_effect =
                equipment_record[SAVE_EQUIPMENT_EFFECT_OFFSET] |
                ITEM_ID_BADGE_TAG;
            int coin_bonus = -1;
            int experience_bonus = -1;

            switch (equipment_effect) {
            case EQUIPMENT_EFFECT_TRIPLE_COINS:
                coin_bonus = (200 * stats->coins + 99) / 100;
                break;
            case EQUIPMENT_EFFECT_DOUBLE_COINS:
                coin_bonus = (100 * stats->coins + 99) / 100;
                break;
            case EQUIPMENT_EFFECT_EXPERIENCE_140_PERCENT:
                experience_bonus = (40 * stats->experience + 99) / 100;
                break;
            case EQUIPMENT_EFFECT_EXPERIENCE_120_PERCENT:
                experience_bonus = (20 * stats->experience + 99) / 100;
                break;
            case EQUIPMENT_EFFECT_GUARANTEED_ITEM_DROP:
                guaranteed_item_drop = 1;
                break;
            }

            if (coin_bonus >= 0) {
                int total = coin_bonus + *(s16 *)(gBattleContext +
                                                  BATTLE_BONUS_COINS_OFFSET);

                if (total >= BATTLE_REWARD_LIMIT) {
                    total = BATTLE_REWARD_LIMIT;
                }
                *(u16 *)(gBattleContext + BATTLE_BONUS_COINS_OFFSET) = total;
            }
            if (experience_bonus >= 0) {
                int total =
                    experience_bonus +
                    *(s16 *)(gBattleContext + BATTLE_BONUS_EXPERIENCE_OFFSET);

                if (total >= BATTLE_REWARD_LIMIT) {
                    total = BATTLE_REWARD_LIMIT;
                }
                *(u16 *)(gBattleContext + BATTLE_BONUS_EXPERIENCE_OFFSET) =
                    total;
            }
        }

        {
            BattleContext *context = (BattleContext *)gBattleContext;
            int experience = stats->experience;
            int earned_experience = context->earned_experience;
            int earned_coins = context->earned_coins + stats->coins;
            int total_experience;

            if (earned_coins >= BATTLE_REWARD_LIMIT) {
                earned_coins = BATTLE_REWARD_LIMIT;
            }
            ((BattleContext *)gBattleContext)->earned_coins = earned_coins;
            total_experience = earned_experience + experience;
            if (total_experience >= BATTLE_REWARD_LIMIT) {
                total_experience = BATTLE_REWARD_LIMIT;
            }
            ((BattleContext *)gBattleContext)->earned_experience =
                total_experience;
        }

        if (!enemy->state_flag_bits.item_drop_processed &&
            (guaranteed_item_drop ||
             Random_NextModulo(100) <
                 stats->item_drop_1.fields.chance_percent)) {
            u16 item_id;
            int slot;

            enemy->state_flag_bits.item_drop_processed = 1;
            item_id = stats->item_drop_2.fields.chance_percent <=
                              Random_NextModulo(100)
                          ? stats->item_drop_1.fields.item_id
                          : stats->item_drop_2.fields.item_id;

            if (item_id != 0) {
                u8 *reward_context;

                slot = 0;
                while (1) {
                    u8 *reward_record;
                    int item_offset;
                    u16 stored_item;

                    reward_context = gBattleContext;
                    reward_record = reward_context + sizeof(u16) * slot;
                    item_offset = sizeof(u16) * slot;
                    stored_item =
                        *(u16 *)(reward_record + BATTLE_REWARD_ITEM_IDS_OFFSET);

                    if (item_id == stored_item) {
                        u8 *count_record =
                            VOLATILE_BATTLE_CONTEXT + item_offset;

                        if (*(u16 *)(count_record +
                                     BATTLE_REWARD_ITEM_COUNTS_OFFSET) +
                                *(u16 *)(count_record +
                                         BATTLE_GUARANTEED_REWARD_ITEM_COUNTS_OFFSET) <
                            BATTLE_REWARD_ITEM_LIMIT) {
                            if (guaranteed_item_drop) {
                                u16 *guaranteed_counts =
                                    (u16 *)(reward_context +
                                            BATTLE_GUARANTEED_REWARD_ITEM_COUNTS_OFFSET);

                                guaranteed_counts[slot]++;
                            } else {
                                u16 *counts =
                                    (u16 *)(reward_context +
                                            BATTLE_REWARD_ITEM_COUNTS_OFFSET);

                                counts[slot]++;
                            }
                        }
                        goto reward_collection_complete;
                    }
                    if (stored_item == 0) {
                        *(u16 *)(reward_record +
                                 BATTLE_REWARD_ITEM_IDS_OFFSET) = item_id;
                        if (guaranteed_item_drop) {
                            *(u16 *)(gBattleContext + item_offset +
                                     BATTLE_GUARANTEED_REWARD_ITEM_COUNTS_OFFSET) =
                                1;
                        } else {
                            *(u16 *)(gBattleContext + item_offset +
                                     BATTLE_REWARD_ITEM_COUNTS_OFFSET) = 1;
                        }
                        goto reward_collection_complete;
                    }
                    slot++;
                    if (slot >= BATTLE_REWARD_ITEM_CAPACITY) {
                        goto reward_collection_complete;
                    }
                }
            }
        }
    }

reward_collection_complete:
    switch (enemy->defeat_effect_kind) {
    case BATTLE_ENEMY_DEFEAT_EFFECT_STANDARD:
        BattleModelEffect_Spawn(41, object, 0, 0, 0, BATTLE_EFFECT_SCALE);
        BattleSpriteEffect_Spawn(4, position.x, position.y, position.z,
                                 BATTLE_EFFECT_SCALE);
        BattleModelEffect_SpawnFromResource(19, 25, position.x, position.y,
                                            (s16)(position.z - 4),
                                            BATTLE_EFFECT_SCALE);
        BattleModelEffect_SpawnFromResource(18, 10, position.x, position.y,
                                            (s16)(position.z + 4),
                                            BATTLE_EFFECT_SCALE);
        BattleSound_Play(4, 0, 0, 0);
        payload->timer = 60;
        break;
    case BATTLE_ENEMY_DEFEAT_EFFECT_ALTERNATE:
        BattleModelEffect_Spawn(42, object, 0, 0, 0, BATTLE_EFFECT_SCALE);
        BattleSpriteEffect_Spawn(18, position.x, position.y, position.z,
                                 BATTLE_EFFECT_SCALE);
        BattleSound_Play(5, 0, 0, 0);
        payload->timer = 300;
        object->removal_state_102 = 0;
        object->removal_state_103 = 0;
        BattleSceneObject_SetAnimation(object, enemy->defeat_animation_id, -1);
        break;
    case BATTLE_ENEMY_DEFEAT_EFFECT_NONE:
        payload->timer = 0;
        break;
    case BATTLE_ENEMY_DEFEAT_EFFECT_DELAYED:
        payload->timer = 30;
        task->callback = BattleEnemy_UpdateDelayedDefeatRemoval;
        break;
    default:
        payload->timer = 0;
        break;
    }

    enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
    return (BattleAITask *)task;
}

void BattleEnemy_UpdateDefeatRemoval(BattleAITask *base_task) {
    BattleEnemyDefeatTask *task = (BattleEnemyDefeatTask *)base_task;
    BattleEnemyDefeatPayload *payload = &task->data;
    BattleSceneObject *object = payload->object;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |= 1 << 2;
    if (payload->timer > 0) {
        payload->timer--;
    }
    if (payload->timer > 0) {
        return;
    }

    BattleActor_GetById(object->actor_id)->current_hp = 0;
    BattleSceneObject_SetAnimation(object, -1, -1);
    {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(object->actor_id));

        if (model != 0) {
            model->flags &= ~BATTLE_MODEL_FLAG_10;
            model->flags &= ~BATTLE_MODEL_FLAG_11;
            model->scale_x = BATTLE_EFFECT_SCALE;
            model->scale_y = BATTLE_EFFECT_SCALE;
            model->rotation_z = 0;
            model->animation_state_bits.state = 31;
        }
    }
    task->callback = 0;
}

void BattleEnemy_UpdateDelayedDefeatRemoval(BattleAITask *base_task) {
    BattleEnemyDefeatTask *task = (BattleEnemyDefeatTask *)base_task;
    BattleEnemyDefeatPayload *payload = &task->data;
    BattleSceneObject *object = payload->object;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |= 1 << 2;
    if (payload->timer > 0) {
        payload->timer--;
    }
    if (payload->timer > 0) {
        return;
    }

    BattleActor_GetById(object->actor_id)->current_hp = 0;
    object->primary_model = 0;
    task->callback = 0;
}

#undef VOLATILE_BATTLE_CONTEXT
