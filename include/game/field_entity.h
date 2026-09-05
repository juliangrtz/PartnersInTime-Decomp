#ifndef PARTNERS_IN_TIME_GAME_FIELD_ENTITY_H
#define PARTNERS_IN_TIME_GAME_FIELD_ENTITY_H

#include <nitro.h>

enum FieldEntityStateFlag {
    FIELD_ENTITY_STATE_ACTIVE = 1 << 1,
    FIELD_ENTITY_STATE_FLAG_02 = 1 << 2
};

typedef struct FieldEntity {
    void *vtable;
    u8 unknown_004[6];
    union {
        u16 property_00a;
        struct {
            u16 property_00a_flag_00 : 1;
            u16 property_00a_unknown_01_15 : 15;
        } property_00a_bits;
    };
    u8 unknown_00c[0xC4];
    union {
        u32 state_flags;
        struct {
            u32 unknown_flag_00 : 1;
            u32 active : 1;
            u32 flag_02 : 1;
            u32 unknown_flags_03_14 : 12;
            u32 interaction_state : 2;
            u32 unknown_flags_17_31 : 15;
        } state_flag_bits;
    };
    u8 unknown_0d4[0x0C];
    u32 action_timer;
    u8 unknown_0e4[8];
} FieldEntity;

typedef char FieldEntity_SizeCheck[sizeof(FieldEntity) == 0xEC ? 1 : -1];

#endif
