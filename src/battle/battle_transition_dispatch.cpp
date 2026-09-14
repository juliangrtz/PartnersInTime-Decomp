extern "C" {
#include <game/battle_context.h>
#include <game/battle_entry.h>
#include <game/battle_frame.h>
#include <game/save_data.h>

extern void (*data_ov002_020c03c4[4])(BattleAITask *task);
}

#define FRAME ((BattleFrameContextView *)gBattleContext)
#define ENTRY ((BattleEntrySaveView *)(gSaveData + 0x558))

extern "C" void BattleTransition_UpdateExitWait(BattleAITask *task) {
    if (FRAME->sub_brightness_level > 0) {
        FRAME->sub_brightness_level -= 2;
        if (FRAME->sub_brightness_level < 0) {
            FRAME->sub_brightness_level = 0;
        } else {
            /* Reaching exactly zero waits one more update; crossing it does not. */
            return;
        }
    }
    if (BattleContext_GetRuntimeState()->flags.bits.unknown_05) {
        return;
    }
    data_ov002_020c03c4[ENTRY->flags.transition_kind](task);
}
