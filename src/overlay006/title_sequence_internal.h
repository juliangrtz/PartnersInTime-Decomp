#ifndef PIT_TITLE_SEQUENCE_INTERNAL_H
#define PIT_TITLE_SEQUENCE_INTERNAL_H
#include <game/title_brightness.h>
#include <game/title_panel_transition.h>
#include <game/title_panel_motion.h>
#include <game/title_localized_sequence.h>
#include <game/title_model.h>
#include <game/title_startup.h>
typedef struct TitleSequenceModelElement {
    TitleSpriteHeader header;
    BattleModel *sub_model, *main_model;
} TitleSequenceModelElement;
typedef struct TitleSequenceAuxElement {
    TitleSequenceModelElement model;
    u32 unknown_30;
} TitleSequenceAuxElement;
typedef struct TitleSequencePrompt {
    u8 unknown_00[56];
    s8 selection;
    u8 unknown_39[2], unknown_3b;
    u8 unknown_3c[36];
} TitleSequencePrompt;
typedef struct TitleSequenceSecondaryPrompt {
    TitleSequencePrompt prompt;
    u8 unknown_60[22];
    s8 selection_override;
    u8 unknown_77;
} TitleSequenceSecondaryPrompt;
/* Prefix of the 59,340-byte sequence allocation, through its cleared state. */
typedef struct TitleAnimationSequence {
    TitleElementList list;
    TitleBrightness brightness;
    TitleSequenceAuxElement auxiliary[25];
    TitlePanelTransition transition;
    TitleSequenceModelElement model;
    u8 unknown_5bc[64];
    TitleSequencePrompt prompt;
    TitleSequenceSecondaryPrompt secondary_prompt;
    TitleOrbitWork orbit[2][7];
    TitleLocalizedSequence localized_sequence;
    TitleSpriteSequence sprite_sequence;
    TitleMovingSpriteChild children[4];
    TitleMovingSprite moving[4];
    u8 unknown_1b30[52], unknown_1b64[52];
    TitlePanelResources panel_resources;
    TitlePanel panels[6], active_panels[2];
    s32 timer;
    u8 phase, unknown_1fa5[3];
    u8 unknown_1fa8[49152];
    void *characters;
    u32 character_size;
    void *palette, *left_screen, *right_screen, *final_screen;
    u8 exit_kind, participant_mask, unknown_dfc2, unknown_dfc3;
    s32 idle_timer;
} TitleAnimationSequence;
typedef char TitleSequenceModelElementSize[sizeof(TitleSequenceModelElement) == 48 ? 1 : -1];
typedef char TitleSequenceAuxElementSize[sizeof(TitleSequenceAuxElement) == 52 ? 1 : -1];
typedef char TitleSequencePrefixSize[sizeof(TitleAnimationSequence) == 57288 ? 1 : -1];
typedef char TitleSequencePromptSize[sizeof(TitleSequencePrompt) == 96 ? 1 : -1];
typedef char TitleSequenceSecondaryPromptSize[sizeof(TitleSequenceSecondaryPrompt) == 120 ? 1 : -1];
/* Save prefix through byte 0x514; the meaning of bit 6 remains unnamed. */
typedef struct TitleSequenceSave {
    u8 unknown_000[1300];
    u8 unknown_00_05 : 6, unknown_06 : 1, unknown_07 : 1;
} TitleSequenceSave;
typedef char TitleSequenceSavePrefixSize[sizeof(TitleSequenceSave) == 1301 ? 1 : -1];

#endif
