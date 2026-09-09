#ifndef PIT_GAME_BATTLE_ATTACK_LOADER_H
#define PIT_GAME_BATTLE_ATTACK_LOADER_H
#include <game/battle_actor.h>

typedef void (*BattleAttackCallback)(BattlePartyActor *actor);

enum BattleAttackLoadPhase {
    BATTLE_ATTACK_LOAD_BEGIN = 0,
    BATTLE_ATTACK_LOAD_WAIT_REQUEST = 1,
    BATTLE_ATTACK_LOAD_WAIT_OVERLAY = 2,
    BATTLE_ATTACK_LOAD_WAIT_COMMON_REQUEST = 3,
    BATTLE_ATTACK_LOAD_WAIT_COMMON_OVERLAY = 4,
    BATTLE_ATTACK_LOAD_READY = 5
};

/* Prefix of the shared battle work allocation through the overlay loader. */
typedef struct BattleOverlayLoadState {
    u8 unknown_00000[0x11344];
    void *requested_overlay;
    void *active_overlay;
    u8 phase, common_requested;
    u8 unknown_1134e[2];
    BattleAttackCallback callback;
} BattleOverlayLoadState;
typedef char BattleOverlayLoadStateSizeCheck[sizeof(BattleOverlayLoadState) == 0x11354 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
extern BattleOverlayLoadState *data_ov002_020c0660;
void BattleAttack_UpdateOverlayLoad(BattlePartyActor *actor);
void BattleAttack_RequestOverlay(BattlePartyActor *actor, void *overlay, BattleAttackCallback callback);
void BattleAttack_StartItemUse(BattlePartyActor *actor);
void BattleAttack_StartSelectedCommand(BattlePartyActor *actor);
#ifdef __cplusplus
}
#endif
#endif
