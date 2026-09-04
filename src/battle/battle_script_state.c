#include <game/battle_ai.h>

BattleAIState *BattleScriptState_GetByObjectId(u16 object_id) {
    if (object_id < 0x28) {
        object_id -= 0x1C;
    } else if (object_id < 0x38) {
        object_id -= 0x1C;
    } else if (object_id < 0x3C) {
        object_id -= 0x1C;
    } else if (object_id < 0x44) {
        object_id -= 0x1C;
    }

    return (BattleAIState *)(gBattleContext + 0x6D44) + object_id;
}
