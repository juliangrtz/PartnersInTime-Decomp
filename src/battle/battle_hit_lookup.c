#include <game/battle_ai.h>
#include <game/battle_hit.h>

BattleHitDescriptor *BattleHitDescriptor_GetByActorId(u16 actor_id) {
    if (actor_id < 0x0A) {
        actor_id = (u16)(actor_id + 0x14);
    } else if (actor_id < 0x38) {
        actor_id = (u16)(actor_id - 0x28);
    } else {
        actor_id -= 0x28;
    }

    return (BattleHitDescriptor *)(gBattleContext + 0xC8F4) + actor_id;
}
