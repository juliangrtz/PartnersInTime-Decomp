#include <game/battle_hit.h>

void BattleHitDescriptor_Disable(BattleHitDescriptor *descriptor) {
    descriptor->callback = 0;
}

void BattleHitDescriptor_DisableByActor(int actor_id) {
    BattleHitDescriptor_Disable(
        BattleHitDescriptor_GetByActorId((u16)actor_id));
}

void BattleHitDescriptor_SetStatus(BattleHitDescriptor *descriptor,
                                   int status_id, s8 chance, s8 magnitude) {
    descriptor->flags =
        (descriptor->flags & ~0xFE00) | (((u16)status_id & 0x7F) << 9);
    descriptor->status_chance = chance;
    descriptor->status_magnitude = magnitude;
}
