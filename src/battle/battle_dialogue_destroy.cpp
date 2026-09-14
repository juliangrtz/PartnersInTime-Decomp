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
