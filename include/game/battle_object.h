#ifndef PIT_GAME_BATTLE_OBJECT_H
#define PIT_GAME_BATTLE_OBJECT_H

#include <nitro.h>

typedef struct BattleObjectDataLoadState {
    u8 data[0x30];
} BattleObjectDataLoadState;

typedef char BattleObjectDataLoadState_SizeCheck[
    sizeof(BattleObjectDataLoadState) == 0x30 ? 1 : -1];

BattleObjectDataLoadState *BattleObjectData_GetLoadState(int object_data_id);

#endif
