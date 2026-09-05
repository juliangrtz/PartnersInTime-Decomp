#ifndef PIT_GAME_BATTLE_FLEE_H
#define PIT_GAME_BATTLE_FLEE_H

#include <game/battle_ai.h>
#include <nitro.h>

typedef union BattleFleeFlags {
    u16 raw;
    struct {
        u16 launch_delay : 14;
        u16 ready_for_partner : 1;
        u16 unknown_15 : 1;
    } bits;
} BattleFleeFlags;

typedef struct BattleFleeTask BattleFleeTask;

typedef struct BattleFleeState {
    BattleFleeTask *partner_task;
    u16 object_id;
    u16 escape_delay;
    u16 input_boost_timer;
    u16 coin_spawn_timer;
    u16 x_fraction;
    BattleFleeFlags flags;
} BattleFleeState;

struct BattleFleeTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleFleeState data;
};

typedef char BattleFleeState_SizeCheck[
    sizeof(BattleFleeState) == 0x10 ? 1 : -1];
typedef char BattleFleeTask_SizeCheck[
    sizeof(BattleFleeTask) == 0x1C ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

void BattleFlee_BeginRun(BattleAITask *task);
void BattleFlee_UpdateRun(BattleAITask *task);
void BattleFlee_WaitForLaunchAnimation(BattleAITask *task);
void BattleFlee_LaunchActor(BattleAITask *task);
void BattleFlee_WaitToLaunchPartner(BattleAITask *task);
void BattleFlee_FinishSceneTransition(BattleAITask *task);
void BattleFlee_UpdateSceneTransition(BattleAITask *task);
void BattleFlee_CommitBattleExit(BattleAITask *task);
void BattleFlee_WaitForExitAnimation(BattleAITask *task);
void BattleFlee_MoveActorOffscreen(BattleAITask *task);
void BattleFlee_WaitForPartnerExit(BattleAITask *task);
void BattleFlee_PreparePartnerExit(BattleAITask *task);

#ifdef __cplusplus
}
#endif

#endif
