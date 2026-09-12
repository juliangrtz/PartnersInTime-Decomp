#ifndef PIT_GAME_TITLE_PANEL_TRANSITION_H
#define PIT_GAME_TITLE_PANEL_TRANSITION_H
#include <nitro.h>

enum TitlePanelTransitionState {
    TITLE_PANEL_STEADY, TITLE_PANEL_SETUP, TITLE_PANEL_SLIDING, TITLE_PANEL_FINISH
};

typedef union TitlePanelDrawFlags {
    u8 value;
    struct { u8 unknown_0 : 1, unknown_1_7 : 7; } bits;
} TitlePanelDrawFlags;
typedef union TitlePanelLoadFlags {
    u8 value;
    struct { u8 hblank_installed : 1, assets_uploaded : 1, unknown_2_7 : 6; } bits;
} TitlePanelLoadFlags;

typedef struct TitlePanelTransition {
    void *next;
    s32 elapsed, duration, x, y, depth;
    void (*update)(struct TitlePanelTransition *);
    void (*draw)(struct TitlePanelTransition *);
    void (*upload)(struct TitlePanelTransition *);
    u8 state;
    TitlePanelDrawFlags draw_flags;
    u8 unknown_26[2];
    const void *characters;
    u32 character_size;
    const void *left_screen, *right_screen, *final_screen, *palette;
    s16 displacement;
    u8 unknown_42;
    TitlePanelLoadFlags loaded;
} TitlePanelTransition;
typedef char TitlePanelTransition_Size[sizeof(TitlePanelTransition) == 68 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void TitlePanelTransition_Release(TitlePanelTransition *work);
void TitlePanelTransition_Skip(TitlePanelTransition *work);
void TitlePanelTransition_Init(TitlePanelTransition *work, const void *characters, u32 size,
                               const void *left, const void *right, const void *final_screen,
                               const void *palette);
void TitlePanelTransition_Upload(TitlePanelTransition *work);
void TitlePanelTransition_SetBlend(int displacement);
void TitlePanelTransition_SetWindows(void);
void TitleScene_SetPanelScroll(int displacement);
void TitlePanelTransition_DisableHBlank(void);
void TitlePanelTransition_EnableHBlank(void);
void TitlePanelTransition_HBlank(void);
void TitlePanelTransition_SetScanlineLayers(int line);
void TitlePanelTransition_Update(TitlePanelTransition *work);
void TitleScene_LoadBackgrounds(void);
#ifdef __cplusplus
}
#endif
#endif
