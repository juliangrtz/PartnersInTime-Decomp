#ifndef PIT_TITLE_SEQUENCE_INTERNAL_H
#define PIT_TITLE_SEQUENCE_INTERNAL_H
#include <game/title_brightness.h>
#include <game/title_panel_transition.h>
#include <game/title_panel_motion.h>
#include <game/title_localized_sequence.h>
#include <game/title_model.h>
#include <game/title_startup.h>
#include "title_trail_internal.h"
#include "title_menu_internal.h"
#include <game/sprite_output.h>
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
enum TitlePromptState {
    TITLE_PROMPT_INACTIVE, TITLE_PROMPT_DELAY, TITLE_PROMPT_APPEARING,
    TITLE_PROMPT_ACTIVE, TITLE_PROMPT_WAIT_CURSOR, TITLE_PROMPT_WAIT_RUMBLE,
    TITLE_PROMPT_EXITING
};
enum TitlePromptOption {
    TITLE_PROMPT_START_GAME, TITLE_PROMPT_OPTIONS, TITLE_PROMPT_PRESS_START
};
typedef struct TitleSequenceRumblePrompt TitleSequenceRumblePrompt;
typedef struct TitleSequencePrompt {
    TitleMenuBase menu;
    TitleSequenceRumblePrompt *rumble_prompt;
    s16 scale_x, scale_y;
    s32 item_x[3], item_y[3];
    u8 layout, unknown_5d[3];
} TitleSequencePrompt;
typedef struct TitlePromptLayout {
    s16 x, y, layout, selector;
} TitlePromptLayout;
struct TitleSequenceRumblePrompt {
    TitleMenuBase menu;
    GameSpriteAllocation text_tiles;
    GameSpritePalette text_palette;
    void *palette_data;
    s32 return_cursor_x, return_cursor_y;
    u16 first_tile;
    s8 selection_override;
    u8 unknown_77;
};
enum TitleRumbleState {
    TITLE_RUMBLE_INACTIVE, TITLE_RUMBLE_ENTERING, TITLE_RUMBLE_ACTIVE,
    TITLE_RUMBLE_WAIT_CURSOR, TITLE_RUMBLE_EXITING
};
#ifdef __cplusplus
extern "C" {
#endif
void TitleRumblePrompt_Release(TitleSequenceRumblePrompt *work);
void TitleRumblePrompt_Open(TitleSequenceRumblePrompt *work);
void TitleRumblePrompt_Update(void *element);
#ifdef __cplusplus
}
#endif

typedef char TitlePromptLayoutSize[sizeof(TitlePromptLayout) == 8 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void TitlePrompt_Release(TitleSequencePrompt *work);
void TitlePrompt_Start(TitleSequencePrompt *work, int delay);
void TitlePrompt_Init(TitleSequencePrompt *work, void *cursor,
                      TitleSequenceRumblePrompt *rumble_prompt, int language);
void TitlePrompt_Draw(void *element);
void TitlePrompt_Update(void *element);
#ifdef __cplusplus
}
#endif
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
    TitleTrailBuffers trail_buffers;
    TitleTrailStamp trail_stamp;
    TitlePanelResources panel_resources;
    TitlePanel panels[6], active_panels[2];
    s32 timer;
    u8 phase, unknown_1fa5[3];
    u32 trail_pixels[2][6144];
    void *characters;
    u32 character_size;
    void *palette, *left_screen, *right_screen, *final_screen;
    u8 exit_kind, participant_mask, unknown_dfc2, rumble_prompt_seen;
    s32 idle_timer;
} TitleAnimationSequence;
typedef char TitleSequenceModelElementSize[sizeof(TitleSequenceModelElement) == 48 ? 1 : -1];
typedef char TitleSequenceAuxElementSize[sizeof(TitleSequenceAuxElement) == 52 ? 1 : -1];
typedef char TitleSequencePrefixSize[sizeof(TitleAnimationSequence) == 57288 ? 1 : -1];
/* The initializer clears the prefix, then copies the 32x32 4bpp trail stamp. */
typedef struct TitleSequenceAllocation {
    TitleAnimationSequence sequence;
    u32 unknown_dfc8;
    u8 trail_stamp_pixels[512], unknown_e1cc[1536];
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
