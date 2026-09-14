#ifndef PIT_BATTLE_DIALOGUE_CONTROL_INTERNAL_H
#define PIT_BATTLE_DIALOGUE_CONTROL_INTERNAL_H
#include <game/battle_dialogue.h>
#include <game/battle_attack_loader.h>
#include <game/window.h>

/* The animated-window state is embedded in the manager. Its first fields
   share the motion prefix; later bytes are reused for texture uploads. */
struct BattleDialogueVisual {
    u8 unknown_00[4];
    s16 baseline_z;
    u8 unknown_06[10];
    s16 motion_z;
    u8 unknown_12[190];
    u8 phase;
    u8 unknown_d1[67];
};

union BattleDialogueSlot {
    GameWindow window;
    struct {
        u8 unknown_00[0xC4];
        BattleDialogueVisual *visual;
        u32 unknown_c8;
    } battle;
};

/* View through the shared manager's four-window table. */
struct BattleDialogueController {
    virtual void unknown_00();
    virtual void destroy_and_free();
    virtual void unknown_08();
    virtual void unknown_0c();
    virtual void unknown_10();
    virtual void close(s16 index);
    u8 unknown_004[0xEFC];
    BattleDialogueSlot *slots;

    inline u8 is_allocated(s16 index)
    {
        return slots[index].window.front ? 1 : 0;
    }
};

struct BattleDialogueWork {
    u8 unknown_000[0x208];
    BattleDialogueController *controller;
};

typedef char BattleDialogueVisual_SizeCheck[sizeof(BattleDialogueVisual) == 276 ? 1 : -1];
typedef char BattleDialogueSlot_SizeCheck[sizeof(BattleDialogueSlot) == 204 ? 1 : -1];
typedef char BattleDialogueController_SizeCheck[sizeof(BattleDialogueController) == 0xF04 ? 1 : -1];
typedef char BattleDialogueWork_SizeCheck[sizeof(BattleDialogueWork) == 0x20C ? 1 : -1];

static inline BattleDialogueController *DialogueController()
{
    return ((BattleDialogueWork *)data_ov002_020c0660)->controller;
}
#endif
