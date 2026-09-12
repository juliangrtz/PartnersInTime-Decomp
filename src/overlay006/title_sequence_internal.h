#ifndef PIT_TITLE_SEQUENCE_INTERNAL_H
#define PIT_TITLE_SEQUENCE_INTERNAL_H
#include <game/title_brightness.h>
#include <game/title_panel_transition.h>
#include <game/title_panel_motion.h>
#include <game/title_localized_sequence.h>
#include <game/title_model.h>
#include <game/title_startup.h>
enum TitleSequencePhase {
    TITLE_SEQUENCE_DELAY_MUSIC = 0,
    TITLE_SEQUENCE_DELAY_ENTRY = 1,
    TITLE_SEQUENCE_WAIT_INITIAL_FADE = 2,
    TITLE_SEQUENCE_WAIT_ENTRY_ORBITS = 3,
    TITLE_SEQUENCE_DELAY_PANELS = 6,
    TITLE_SEQUENCE_WAIT_TRANSITION = 7,
    TITLE_SEQUENCE_WAIT_EXPANSION = 8,
    TITLE_SEQUENCE_WAIT_MODELS = 9,
    TITLE_SEQUENCE_ACTIVE = 10,
    TITLE_SEQUENCE_WAIT_AUTO_EXIT = 11,
    TITLE_SEQUENCE_EXIT_FADE = 12,
    TITLE_SEQUENCE_SKIP_OUT = 13,
    TITLE_SEQUENCE_SKIP_RESET = 14,
    TITLE_SEQUENCE_SKIP_IN = 15
};
typedef struct TitleSequenceModelElement {
    TitleSpriteHeader header;
    BattleModel *sub_model, *main_model;
} TitleSequenceModelElement;
enum TitleSequenceModelState {
    TITLE_SEQUENCE_MODEL_IDLE,
    TITLE_SEQUENCE_MODEL_DELAY,
    TITLE_SEQUENCE_MODEL_MOVING
};
#ifdef __cplusplus
extern "C" {
#endif
void TitleSequenceModel_FinishEntry(TitleSequenceModelElement *work);
void TitleSequenceModel_StartEntry(TitleSequenceModelElement *work);
void TitleSequenceModel_Init(TitleSequenceModelElement *work);
void TitleSequenceModel_Update(void *element);
#ifdef __cplusplus
}
#endif
typedef struct TitleSequenceAuxElement {
    TitleSequenceModelElement model;
    u32 unknown_30;
} TitleSequenceAuxElement;
typedef struct TitleSequencePrompt {
    u8 unknown_00[56];
    s8 selection;
    u8 unknown_39[2], input_disabled;
    u8 unknown_3c[36];
} TitleSequencePrompt;
typedef struct TitleSequenceRumblePrompt {
    TitleSequencePrompt prompt;
    u8 unknown_60[22];
    s8 selection_override;
    u8 unknown_77;
} TitleSequenceRumblePrompt;
/* Prefix of the 59,340-byte sequence allocation, through its cleared state. */
typedef struct TitleAnimationSequence {
    TitleElementList list;
    TitleBrightness brightness;
    TitleSequenceAuxElement auxiliary[25];
    TitlePanelTransition transition;
    TitleSequenceModelElement model;
    u8 unknown_5bc[64];
    TitleSequencePrompt prompt;
    TitleSequenceRumblePrompt rumble_prompt;
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
    u8 exit_kind, participant_mask, unknown_dfc2, rumble_prompt_seen;
    s32 idle_timer;
} TitleAnimationSequence;
typedef char TitleSequenceModelElementSize[sizeof(TitleSequenceModelElement) == 48 ? 1 : -1];
typedef char TitleSequenceAuxElementSize[sizeof(TitleSequenceAuxElement) == 52 ? 1 : -1];
typedef char TitleSequencePrefixSize[sizeof(TitleAnimationSequence) == 57288 ? 1 : -1];
/* The initializer clears the prefix, then copies 512 palette bytes into the tail. */
typedef struct TitleSequenceAllocation {
    TitleAnimationSequence sequence;
    u32 unknown_dfc8;
    u8 color_data[512], unknown_e1cc[1536];
} TitleSequenceAllocation;
typedef char TitleSequenceAllocationSize[sizeof(TitleSequenceAllocation) == 59340 ? 1 : -1];
typedef char TitleSequencePromptSize[sizeof(TitleSequencePrompt) == 96 ? 1 : -1];
typedef char TitleSequenceRumblePromptSize[sizeof(TitleSequenceRumblePrompt) == 120 ? 1 : -1];
/* Save prefix through the rumble preference in byte 0x514. */
typedef struct TitleSequenceSave {
    u8 unknown_000[1300];
    u8 unknown_00_05 : 6, rumble_enabled : 1, unknown_07 : 1;
} TitleSequenceSave;
typedef char TitleSequenceSavePrefixSize[sizeof(TitleSequenceSave) == 1301 ? 1 : -1];

#endif
