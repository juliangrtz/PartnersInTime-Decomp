#ifndef PIT_GAME_BATTLE_HIT_H
#define PIT_GAME_BATTLE_HIT_H

#include <nitro.h>

typedef struct BattleHitDescriptor BattleHitDescriptor;
typedef struct BattleHitRecord BattleHitRecord;
typedef struct BattlePosition BattlePosition;
typedef void (*BattleHitRecordCallback)(BattleHitRecord *record);
typedef BattleHitRecordCallback BattleHitCallback;

enum BattleHitDescriptorFlag {
    BATTLE_HIT_KIND_MASK = 0x003F,
    BATTLE_HIT_FLAG_ACTIVE = 0x0040,
    BATTLE_HIT_FLAG_UNKNOWN_0080 = 0x0080,
    BATTLE_HIT_FLAG_UNKNOWN_0100 = 0x0100,
    BATTLE_HIT_STATUS_SHIFT = 9,
    BATTLE_HIT_STATUS_MASK = 0xFE00
};

enum BattleHitTarget {
    BATTLE_HIT_TARGET_PARTY_PAIR = 56,
    BATTLE_HIT_TARGET_ALL = 60
};

enum BattleHitKind {
    BATTLE_HIT_KIND_DEFAULT = BATTLE_HIT_KIND_MASK
};

struct BattleHitDescriptor {
    BattleHitDescriptor *next;
    BattleHitCallback callback;
    u16 source_id;
    u16 target_id;
    union {
        u16 flags;
        struct {
            u16 hit_kind : 6;
            u16 active : 1;
            u16 skip_mario : 1;
            u16 skip_luigi : 1;
            u16 status_id : 7;
        } flag_bits;
    };
    s8 status_chance;
    s8 status_magnitude;
};

struct BattleHitRecord {
    u16 source_id;
    u16 target_id;
    s16 x;
    s16 y;
    s16 z;
    s16 kind;
    s8 status_id;
    s8 status_chance;
    s8 status_magnitude;
    u8 padding_0f;
    BattleHitRecordCallback callback;
};

typedef struct BattleCollisionBounds {
    s16 axis[6];
} BattleCollisionBounds;

typedef char BattleHitDescriptor_SizeCheck[
    sizeof(BattleHitDescriptor) == 0x10 ? 1 : -1];
typedef char BattleHitRecord_SizeCheck[
    sizeof(BattleHitRecord) == 0x14 ? 1 : -1];
typedef char BattleCollisionBounds_SizeCheck[
    sizeof(BattleCollisionBounds) == 0x0C ? 1 : -1];

void BattleHitDescriptor_Disable(BattleHitDescriptor *descriptor);
void BattleDamage_DispatchHit(BattleHitRecord *record);
void BattleHitQueue_Update(void);
int BattleCollision_GetBounds(BattleCollisionBounds *bounds, u32 actor_id,
                              int animation_index, int frame_index);
int BattleCollision_TestObjects(u32 source_id, u32 target_id,
                                BattlePosition *hit_position);
void BattleHitDescriptor_DisableByActor(int actor_id);
void BattleHitDescriptor_SetStatus(BattleHitDescriptor *descriptor,
                                   int status_id, s8 chance, s8 magnitude);
BattleHitDescriptor *BattleHitDescriptor_GetByActorId(u16 actor_id);
BattleHitDescriptor *BattleHitDescriptor_Configure(
    u16 source_id, u16 target_id, BattleHitCallback callback,
    u16 linked_actor_id, int hit_kind);

#endif
