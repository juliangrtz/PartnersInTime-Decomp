#ifndef PIT_GAME_BATTLE_HIT_H
#define PIT_GAME_BATTLE_HIT_H

#include <nitro.h>

typedef struct BattleHitDescriptor BattleHitDescriptor;
typedef void (*BattleHitCallback)(BattleHitDescriptor *descriptor);

enum BattleHitDescriptorFlag {
    BATTLE_HIT_KIND_MASK = 0x003F,
    BATTLE_HIT_FLAG_ACTIVE = 0x0040,
    BATTLE_HIT_FLAG_UNKNOWN_0080 = 0x0080,
    BATTLE_HIT_FLAG_UNKNOWN_0100 = 0x0100,
    BATTLE_HIT_STATUS_SHIFT = 9,
    BATTLE_HIT_STATUS_MASK = 0xFE00
};

enum BattleHitKind {
    BATTLE_HIT_KIND_DEFAULT = BATTLE_HIT_KIND_MASK
};

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
