#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_damage.h>
#include <game/battle_hit.h>
#include <game/battle_scene.h>

extern void BattleDamage_DispatchHit(BattleHitDescriptor *descriptor);

enum {
    BATTLE_HIT_QUEUE_OFFSET = 0xCAD8,
    BATTLE_HIT_QUEUE_CAPACITY = 8,
    BATTLE_EFFECT_VARIANT_OFFSET = 0xCB7A
};

BattleHitDescriptor *BattleHitDescriptor_Configure(
    u16 source_id, u16 target_id, BattleHitCallback callback,
    u16 linked_actor_id, int hit_kind) {
    BattleHitDescriptor *descriptor =
        BattleHitDescriptor_GetByActorId(source_id);

    descriptor->flags &= ~BATTLE_HIT_STATUS_MASK;
    if (target_id >= 0xFFFF) {
        descriptor->callback = 0;
        return descriptor;
    }

    if (((u32)(descriptor->flags << 25) >> 31) == 0) {
        u8 *volatile *context_slot = &gBattleContext;

        descriptor->flags |= BATTLE_HIT_FLAG_ACTIVE;
        descriptor->next = *(BattleHitDescriptor **)(*context_slot + 0xCAD4);
        *(BattleHitDescriptor **)(*context_slot + 0xCAD4) = descriptor;
    }

    {
        int effective_hit_kind = hit_kind;
        u16 flags;

        descriptor->source_id = source_id;
        descriptor->target_id = target_id;
        if (effective_hit_kind == 0) {
            effective_hit_kind = BATTLE_HIT_KIND_DEFAULT;
        }
        flags = descriptor->flags;
        effective_hit_kind = (u16)effective_hit_kind;
        if (callback == 0) {
            callback = BattleDamage_DispatchHit;
        }
        flags = (flags & ~BATTLE_HIT_KIND_MASK) |
                (effective_hit_kind & BATTLE_HIT_KIND_MASK);
        descriptor->flags = flags;
    }
    descriptor->callback = callback;
    descriptor->flags &= ~BATTLE_HIT_FLAG_UNKNOWN_0080;
    descriptor->flags &= ~BATTLE_HIT_FLAG_UNKNOWN_0100;

    if (linked_actor_id == 0) {
        linked_actor_id = source_id;
    }
    BattleSceneObject_GetById(source_id)->linked_actor_id = linked_actor_id;
    return descriptor;
}

void BattleHitDescriptor_SetStatus(BattleHitDescriptor *descriptor,
                                   int status_id, s8 chance, s8 magnitude) {
    u16 status = (u16)status_id;

    descriptor->flags =
        (descriptor->flags & ~BATTLE_HIT_STATUS_MASK) |
        ((status & (BATTLE_HIT_STATUS_MASK >> BATTLE_HIT_STATUS_SHIFT))
         << BATTLE_HIT_STATUS_SHIFT);
    descriptor->status_chance = chance;
    descriptor->status_magnitude = magnitude;
}

void BattleHitDescriptor_DisableByActor(int actor_id) {
    BattleHitDescriptor_Disable(
        BattleHitDescriptor_GetByActorId((u16)actor_id));
}

void BattleHitDescriptor_Disable(BattleHitDescriptor *descriptor) {
    descriptor->callback = 0;
}

void BattleDamage_ReflectQueuedHits(int target_actor_id) {
    int index = 0;
    BattleHitRecord *record =
        (BattleHitRecord *)(gBattleContext + BATTLE_HIT_QUEUE_OFFSET);

    do {
        int damage;

        if (record->kind == 0) {
            return;
        }
        if (target_actor_id == record->target_id) {
            damage = BattleDamage_CalculateByObject(record->target_id,
                                                    record->source_id);
            if (damage > 0) {
                BattleSceneObject *source =
                    BattleSceneObject_GetById(record->source_id);
                BattleActor_GetById(source->linked_actor_id)->pending_damage =
                    damage;
            }

            {
                u16 target_id = record->target_id;
                BattleHitDescriptor *descriptor;

                record->target_id = record->source_id;
                record->source_id = target_id;
                descriptor = BattleHitDescriptor_GetByActorId(target_id);
                record->status_id =
                    (u32)(descriptor->flags << 16) >>
                    (16 + BATTLE_HIT_STATUS_SHIFT);
                record->status_chance = descriptor->status_chance;
                record->status_magnitude = descriptor->status_magnitude;
            }
        }

        ++index;
        ++record;
    } while (index < BATTLE_HIT_QUEUE_CAPACITY);
}

void BattleEffect_SetVariant(int variant) {
    *(s16 *)(gBattleContext + BATTLE_EFFECT_VARIANT_OFFSET) = variant;
}
