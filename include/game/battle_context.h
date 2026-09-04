#ifndef PIT_GAME_BATTLE_CONTEXT_H
#define PIT_GAME_BATTLE_CONTEXT_H

#include <game/battle_ai.h>

enum BattleContextOffset {
    BATTLE_CONTEXT_BACKGROUND_ID_OFFSET = 0x3A,
    BATTLE_RUNTIME_STATE_OFFSET = 0xD000,
    BATTLE_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleRuntimeFlag {
    BATTLE_RUNTIME_FLAG_ASSET_LOAD_PENDING = 1 << 8,
    BATTLE_RUNTIME_FLAG_ALTERNATE_FORMATION = 1 << 16,
    BATTLE_RUNTIME_FLAG_BACKGROUND_BUFFER = 1 << 19,
    BATTLE_RUNTIME_FLAG_BACKGROUND_FADED_OUT = 1 << 20
};

typedef union BattleRuntimeFlags {
    u32 raw;
    struct {
        u32 unknown_00_07 : 8;
        u32 asset_load_pending : 1;
        u32 unknown_09_15 : 7;
        u32 alternate_formation : 1;
        u32 unknown_17_18 : 2;
        u32 background_buffer : 1;
        u32 background_faded_out : 1;
        u32 unknown_21_31 : 11;
    } bits;
} BattleRuntimeFlags;

typedef struct BattleRuntimeState {
    u8 unknown_000[0x3A0];
    BattleRuntimeFlags flags;
} BattleRuntimeState;

typedef struct BattleContext {
    u8 unknown_0000[0x3A];
    u16 background_id;
    u8 unknown_003c[0xCFC4];
    BattleRuntimeState runtime;
} BattleContext;

typedef char BattleRuntimeState_SizeCheck[
    sizeof(BattleRuntimeState) == 0x3A4 ? 1 : -1
];
typedef char BattleContext_KnownPrefixSizeCheck[
    sizeof(BattleContext) == 0xD3A4 ? 1 : -1
];

static inline BattleRuntimeState *BattleContext_GetRuntimeState(void) {
    return (BattleRuntimeState *)(gBattleContext + BATTLE_RUNTIME_STATE_OFFSET);
}

#endif
