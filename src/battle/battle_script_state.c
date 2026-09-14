#include <game/battle_context.h>

/* Resource handles use the high nibble for the one-based archive slot. */
BattleObjectResourceRequest *BattleObjectData_ResolveSlot(u16 packed_id) {
    int index = (packed_id >> 12) - 1;

    ((BattleContext *)gBattleContext)->ai_archives[index].entry_index =
        packed_id & 0xFFF;
    return &((BattleContext *)gBattleContext)->ai_archives[index];
}

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
