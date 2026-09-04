#include <game/battle_ai.h>

extern void func_0202cbd4(void *destination, int value, u32 size);

void BattleAI_InitStateFromScriptBlock(BattleAIState *state,
                                       const u16 *script_block, int owner_id) {
    func_0202cbd4(state, 0, 0xB8);
    state->script = script_block + ((u32)*script_block >> 1);
    state->owner_id = owner_id;
}
