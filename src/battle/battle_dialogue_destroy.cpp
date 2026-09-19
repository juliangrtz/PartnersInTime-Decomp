/*
 * Battle dialogue teardown (overlay 2, 0x02066B6C-0x02066BC4).
 *
 * Destroys the dialogue controller and releases what it owns.
 */

#include "battle_dialogue_control_internal.h"

static inline void DeleteDialogueController(BattleDialogueController *controller)
{
    if (controller)
        controller->destroy_and_free();
}

extern "C" void BattleDialogue_Destroy(void)
{
    if (!DialogueController())
        return;
    DeleteDialogueController(DialogueController());
    ((BattleDialogueWork *)data_ov002_020c0660)->controller = 0;
}
