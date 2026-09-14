#include "battle_dialogue_control_internal.h"

extern "C" {

int BattleDialogue_IsOpen(int index)
{
    if (index >= 0)
        return ((u8)DialogueController()->slots[(s16)index].window.allocation.bits.closing ^ 1) != 0;
    for (int i = 0; i < 4; ++i)
        if (!(u8)DialogueController()->slots[(s16)i].window.allocation.bits.closing)
            return 1;
    return 0;
}

int BattleDialogue_IsAllocated(int index)
{
    if (index >= 0)
        return DialogueController()->is_allocated((s16)index) != 0;
    for (int i = 0; i < 4; ++i)
        if (DialogueController()->is_allocated((s16)i))
            return 1;
    return 0;
}

void BattleDialogue_Close(int index)
{
    if (index >= 0)
        DialogueController()->close((s16)index);
    else
        for (int i = 0; i < 4; ++i)
            DialogueController()->close((s16)i);
}

int BattleDialogue_GetVisualProperty(int index, int property)
{
    BattleDialogueVisual *visual = DialogueController()->slots[index].battle.visual;
    switch (property) {
    case 0: return visual->phase >= 2;
    case 1: return visual->baseline_z + visual->motion_z;
    default: return 0;
    }
}
}
