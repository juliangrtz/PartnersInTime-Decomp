#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_scene.h>

enum BattleSceneSlotOffset {
    BATTLE_FIELD_OBJECT_TABLE_OFFSET = 0x4CC8,
    BATTLE_LOW_ACTOR_TABLE_BIAS = 0x4F18,
    BATTLE_HIGH_ACTOR_TABLE_BIAS = 0x6418
};

enum BattleSceneSlotRange {
    BATTLE_FIELD_SLOT_END = 40,
    BATTLE_LOW_ACTOR_SLOT_END = 56,
    BATTLE_PARTY_SLOT_END = 60,
    BATTLE_ENEMY_SLOT_END = 68
};

#define FIELD_OBJECT(slot) \
    (*(BattleSceneObject **)(gBattleContext + \
                             BATTLE_FIELD_OBJECT_TABLE_OFFSET + (slot) * 4))
#define LOW_ACTOR(slot) \
    (*(BattleActor **)(gBattleContext + BATTLE_LOW_ACTOR_TABLE_BIAS + \
                       (slot) * 4))
#define HIGH_ACTOR(slot) \
    (*(BattleActor **)(gBattleContext + BATTLE_HIGH_ACTOR_TABLE_BIAS + \
                       (slot) * 4))

void BattleSceneObject_SwapSlots(u32 first_id, u32 second_id) {
    if (first_id < BATTLE_FIELD_SLOT_END) {
        if (second_id < BATTLE_FIELD_SLOT_END) {
            BattleSceneObject *temporary = FIELD_OBJECT(first_id);
            FIELD_OBJECT(first_id) = FIELD_OBJECT(second_id);
            FIELD_OBJECT(second_id) = temporary;

            FIELD_OBJECT(first_id)->linked_actor_id = first_id;
            FIELD_OBJECT(first_id)->actor_id =
                FIELD_OBJECT(first_id)->linked_actor_id;
            FIELD_OBJECT(second_id)->linked_actor_id = second_id;
            FIELD_OBJECT(second_id)->actor_id =
                FIELD_OBJECT(second_id)->linked_actor_id;
            return;
        }

        if (second_id < BATTLE_LOW_ACTOR_SLOT_END) {
            BattleSceneObject *temporary = FIELD_OBJECT(first_id);
            FIELD_OBJECT(first_id) = LOW_ACTOR(second_id)->scene_object;
            LOW_ACTOR(second_id)->scene_object = temporary;

            FIELD_OBJECT(first_id)->linked_actor_id = first_id;
            FIELD_OBJECT(first_id)->actor_id =
                FIELD_OBJECT(first_id)->linked_actor_id;
            temporary->linked_actor_id = second_id;
            temporary->actor_id = temporary->linked_actor_id;
            return;
        }

        if (second_id >= BATTLE_PARTY_SLOT_END) {
            return;
        }

        {
            BattleSceneObject *temporary = FIELD_OBJECT(first_id);
            FIELD_OBJECT(first_id) = HIGH_ACTOR(second_id)->scene_object;
            HIGH_ACTOR(second_id)->scene_object = temporary;

            FIELD_OBJECT(first_id)->linked_actor_id = first_id;
            FIELD_OBJECT(first_id)->actor_id =
                FIELD_OBJECT(first_id)->linked_actor_id;
            temporary->linked_actor_id = second_id;
            temporary->actor_id = temporary->linked_actor_id;
            return;
        }
    }

    if (first_id < BATTLE_LOW_ACTOR_SLOT_END) {
        return;
    }

    if (first_id < BATTLE_PARTY_SLOT_END) {
        if (second_id < BATTLE_FIELD_SLOT_END) {
            BattleSceneObject *temporary = FIELD_OBJECT(second_id);
            FIELD_OBJECT(second_id) = LOW_ACTOR(first_id)->scene_object;
            LOW_ACTOR(first_id)->scene_object = temporary;

            FIELD_OBJECT(second_id)->linked_actor_id = second_id;
            FIELD_OBJECT(second_id)->actor_id =
                FIELD_OBJECT(second_id)->linked_actor_id;
            temporary->linked_actor_id = first_id;
            temporary->actor_id = temporary->linked_actor_id;
            return;
        }

        if (second_id < BATTLE_LOW_ACTOR_SLOT_END ||
            second_id >= BATTLE_PARTY_SLOT_END) {
            return;
        }

        {
            BattleActor *temporary = LOW_ACTOR(first_id);
            BattleActor *first;
            BattleActor *second;
            u16 first_flags;

            LOW_ACTOR(first_id) = LOW_ACTOR(second_id);
            LOW_ACTOR(second_id) = temporary;
            LOW_ACTOR(first_id)->scene_object->linked_actor_id = first_id;
            LOW_ACTOR(first_id)->scene_object->actor_id =
                LOW_ACTOR(first_id)->scene_object->linked_actor_id;
            LOW_ACTOR(second_id)->scene_object->linked_actor_id = second_id;
            LOW_ACTOR(second_id)->scene_object->actor_id =
                LOW_ACTOR(second_id)->scene_object->linked_actor_id;

            first = LOW_ACTOR(first_id);
            second = LOW_ACTOR(second_id);
            first_flags = first->flags;
            first->flag_bits.unknown_flag_07 =
                second->flag_bits.unknown_flag_07;
            second->flag_bits.unknown_flag_07 =
                (u32)(first_flags << 24) >> 31;
            return;
        }
    }

    if (first_id >= BATTLE_ENEMY_SLOT_END) {
        return;
    }

    if (second_id < BATTLE_FIELD_SLOT_END) {
        BattleSceneObject *temporary = FIELD_OBJECT(second_id);
        FIELD_OBJECT(second_id) = HIGH_ACTOR(first_id)->scene_object;
        HIGH_ACTOR(first_id)->scene_object = temporary;

        FIELD_OBJECT(second_id)->linked_actor_id = second_id;
        FIELD_OBJECT(second_id)->actor_id =
            FIELD_OBJECT(second_id)->linked_actor_id;
        temporary->linked_actor_id = first_id;
        temporary->actor_id = temporary->linked_actor_id;
        return;
    }

    if (second_id < BATTLE_LOW_ACTOR_SLOT_END) {
        return;
    }
    if (second_id < BATTLE_PARTY_SLOT_END) {
        return;
    }
    if (second_id >= BATTLE_ENEMY_SLOT_END) {
        return;
    }

    {
        BattleActor *temporary = HIGH_ACTOR(first_id);
        BattleActor *first;
        BattleActor *second;
        u16 first_flags;

        HIGH_ACTOR(first_id) = HIGH_ACTOR(second_id);
        HIGH_ACTOR(second_id) = temporary;
        HIGH_ACTOR(first_id)->scene_object->linked_actor_id = first_id;
        HIGH_ACTOR(first_id)->scene_object->actor_id =
            HIGH_ACTOR(first_id)->scene_object->linked_actor_id;
        HIGH_ACTOR(second_id)->scene_object->linked_actor_id = second_id;
        HIGH_ACTOR(second_id)->scene_object->actor_id =
            HIGH_ACTOR(second_id)->scene_object->linked_actor_id;

        second = HIGH_ACTOR(second_id);
        first = HIGH_ACTOR(first_id);
        first_flags = first->flags;
        first->flag_bits.unknown_flag_07 =
            second->flag_bits.unknown_flag_07;
        second->flag_bits.unknown_flag_07 =
            (u32)(first_flags << 24) >> 31;
    }
}
