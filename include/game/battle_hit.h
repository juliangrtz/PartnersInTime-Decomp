#ifndef PIT_GAME_BATTLE_HIT_H
#define PIT_GAME_BATTLE_HIT_H

#include <nitro.h>

typedef struct BattleHitDescriptor BattleHitDescriptor;
typedef void (*BattleHitCallback)(BattleHitDescriptor *descriptor);

struct BattleHitDescriptor {
    BattleHitDescriptor *next;
    BattleHitCallback callback;
    u16 source_id;
    u16 target_id;
    u16 flags;
    s8 status_chance;
    s8 status_magnitude;
};

typedef char BattleHitDescriptor_SizeCheck[
    sizeof(BattleHitDescriptor) == 0x10 ? 1 : -1];

void BattleHitDescriptor_Disable(BattleHitDescriptor *descriptor);
void BattleHitDescriptor_DisableByActor(int actor_id);
void BattleHitDescriptor_SetStatus(BattleHitDescriptor *descriptor,
                                   int status_id, s8 chance, s8 magnitude);
BattleHitDescriptor *BattleHitDescriptor_GetByActorId(u16 actor_id);
BattleHitDescriptor *BattleHitDescriptor_Configure(
    u16 source_id, u16 target_id, BattleHitCallback callback,
    u16 linked_actor_id, int hit_kind);

#endif
