#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_enemy_data.h>
#include <game/battle_scene.h>

extern void OS_Terminate(void);

enum BattleEntityResourceRange {
    BATTLE_ENTITY_ID_LIMIT = 70,
    BATTLE_OBJECT_DATA_ID_LIMIT = 80,
    BATTLE_ENEMY_OBJECT_DATA_FIRST = 72
};

#define ENEMY_RESOURCE(actor) \
    ((BattleEnemyDataRequest *)(actor)->resource_slot)

void BattleEntity_BindResource(u32 actor_id, u16 object_data_id) {
    BattleSceneObject *object;
    BattleSceneResource *resource;
    BattleEnemyActor *enemy;

    if (actor_id >= BATTLE_ENTITY_ID_LIMIT) {
        OS_Terminate();
    }
    if (object_data_id >= BATTLE_OBJECT_DATA_ID_LIMIT) {
        OS_Terminate();
    }

    if (object_data_id < BATTLE_ENEMY_OBJECT_DATA_FIRST) {
        object = BattleSceneObject_GetById(actor_id);
        resource = &((BattleContext *)gBattleContext)
                        ->runtime.object_data_load_states[object_data_id];

        object->resource = resource;
        object->animation_variant_offset = 0;
        object->flags.bits.script_mode = 0;
        object->unk_0c8 = 0;

        if (resource->texture_set != 0) {
            object->property_0fc = resource->texture_set->scene_property_0fc;
            object->property_0fd = resource->texture_set->scene_property_0fd;
            object->property_0f8 = resource->texture_set->scene_property_0f8;
            object->property_0fe = resource->texture_set->scene_property_0fe;
            object->property_0ff = resource->texture_set->scene_property_0ff;
            object->property_0fa = resource->texture_set->scene_property_0fa;
            object->property_100 = resource->texture_set->scene_property_100;
            object->property_101 = resource->texture_set->scene_property_101;
            object->property_102 = 0;
            object->property_103 = 0;
        }
        return;
    }

    {
        actor_id = (u16)(actor_id - BATTLE_ACTOR_ENEMY_FIRST);

        if (actor_id >= BATTLE_ACTOR_ENEMY_COUNT) {
            OS_Terminate();
        }

        enemy = (BattleEnemyActor *)
            ((BattleContext *)gBattleContext)->enemy_actors[actor_id];
        enemy->actor.resource_slot =
            &((BattleContext *)gBattleContext)
                 ->enemy_data_requests[object_data_id -
                                       BATTLE_ENEMY_OBJECT_DATA_FIRST];

        enemy->actor.max_hp = ENEMY_RESOURCE(&enemy->actor)->stats->max_hp;
        enemy->actor.current_hp = enemy->actor.max_hp;
        enemy->actor.base_speed = ENEMY_RESOURCE(&enemy->actor)->stats->speed;
        enemy->actor.speed = enemy->actor.base_speed;
        enemy->actor.base_power = ENEMY_RESOURCE(&enemy->actor)->stats->power;
        enemy->actor.power = enemy->actor.base_power;
        enemy->actor.base_defense =
            ENEMY_RESOURCE(&enemy->actor)->stats->defense;
        enemy->actor.defense = enemy->actor.base_defense;
        enemy->actor.unk_016 = 0;
        enemy->actor.unk_00e = enemy->actor.unk_016;
        enemy->actor.flags =
            (enemy->actor.flags & ~BATTLE_ACTOR_LEVEL_MASK) |
            (ENEMY_RESOURCE(&enemy->actor)->stats->level &
             BATTLE_ACTOR_LEVEL_MASK);
        enemy->state_flags =
            (enemy->state_flags & ~0xC0) |
            ((ENEMY_RESOURCE(&enemy->actor)->stats->trait_bits.trait_class &
              3) << 6);
        enemy->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        enemy->resource_initialized = 1;

        enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_07;
        enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_KO;
        enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_10;
        enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_12;
        enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_13;
        enemy->actor.flags &= ~BATTLE_ACTOR_FLAG_14;
        enemy->state_flags &= ~1;
        enemy->state_flags &= ~2;
        enemy->state_flags &= ~4;
        enemy->state_flags &= ~8;
        enemy->state_flags &= ~0x10;
        enemy->state_flags &= ~0x20;
        return;
    }
}
