#ifndef PIT_GAME_BATTLE_ACTION_H
#define PIT_GAME_BATTLE_ACTION_H

#include <game/battle_scene.h>
#include <game/battle_task_queue.h>

enum BattleActionFlag {
    BATTLE_ACTION_FLAG_SCRIPT_LOAD_PENDING = 1 << 2
};

typedef struct BattleActionResourceId {
    u16 unknown_00;
    u16 packed_object_data_id;
} BattleActionResourceId;

typedef struct BattleActionDefinition {
    BattleActionResourceId *resource_id;
} BattleActionDefinition;

typedef struct BattleActionState {
    BattleSceneObject *scene_object;
    u8 unknown_004[0x68];
    BattleActionDefinition *definition;
    const u16 *script;
    u8 unknown_074[0x228];
    u32 flags;
} BattleActionState;

typedef char BattleActionState_SizeCheck[
    sizeof(BattleActionState) == 0x2A0 ? 1 : -1];

BattleQueuedTask *BattleActionScript_RequestLoad(BattleActionState *action);
BattleQueuedTask *BattlePartyScript_RequestLoad(u16 packed_object_data_id);

#endif
