#include <game/battle_ai.h>
#include <game/battle_hit.h>
#include <game/battle_scene.h>

extern void BattleDamage_DispatchHit(BattleHitDescriptor *descriptor);

BattleHitDescriptor *BattleHitDescriptor_Configure(
    u16 source_id, u16 target_id, BattleHitCallback callback,
    u16 linked_actor_id, int hit_kind) {
    BattleHitDescriptor *descriptor =
        BattleHitDescriptor_GetByActorId(source_id);

    descriptor->flags &= ~0xFE00;
    if (target_id >= 0xFFFF) {
        descriptor->callback = 0;
        return descriptor;
    }

    if (((u32)descriptor->flags << 25) >> 31 == 0) {
        u8 *volatile *context_slot = &gBattleContext;

        descriptor->flags |= 0x40;
        descriptor->next = *(BattleHitDescriptor **)(*context_slot + 0xCAD4);
        *(BattleHitDescriptor **)(*context_slot + 0xCAD4) = descriptor;
    }

    {
        int effective_hit_kind = hit_kind;
        u16 flags;

        descriptor->source_id = source_id;
        descriptor->target_id = target_id;
        if (effective_hit_kind == 0) {
            effective_hit_kind = 0x3F;
        }
        flags = descriptor->flags;
        effective_hit_kind = (u16)effective_hit_kind;
        if (callback == 0) {
            callback = BattleDamage_DispatchHit;
        }
        flags = (flags & ~0x3F) | (effective_hit_kind & 0x3F);
        descriptor->flags = flags;
    }
    descriptor->callback = callback;
    descriptor->flags &= ~0x80;
    descriptor->flags &= ~0x100;

    if (linked_actor_id == 0) {
        linked_actor_id = source_id;
    }
    BattleSceneObject_GetById(source_id)->linked_actor_id = linked_actor_id;
    return descriptor;
}
