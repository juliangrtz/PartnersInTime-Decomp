#ifndef PIT_GAME_TITLE_PANEL_MOTION_H
#define PIT_GAME_TITLE_PANEL_MOTION_H
#include <game/title_effects.h>

typedef struct TitleElementLink {
    struct TitleElementLink *next;
} TitleElementLink;
typedef char TitleElementLink_SizeCheck[sizeof(TitleElementLink) == 4 ? 1 : -1];

typedef struct TitlePanelTrail {
    s16 x, y, active, unknown_06;
} TitlePanelTrail;
typedef struct TitlePanel {
    TitleElementLink *next;
    s32 elapsed, duration;
    s32 x, y;
    u32 unknown_14;
    void (*update)(struct TitlePanel *);
    void (*draw)(struct TitlePanel *);
    u32 unknown_20;
    u8 state, unknown_25[3];
    u32 texture, palette;
    s32 start_x, start_y, target_x, target_y;
    void *parent;
    TitlePanelTrail trail[6];
    u8 variant, alpha, index, unknown_77;
} TitlePanel;
typedef char TitlePanel_SizeCheck[sizeof(TitlePanel) == 120 ? 1 : -1];
typedef struct TitleElementList {
    TitleElementLink *first, *last;
} TitleElementList;
typedef char TitleElementList_SizeCheck[sizeof(TitleElementList) == 8 ? 1 : -1];

typedef struct TitlePanelResources {
    void *pixels;
    u32 pixel_bytes;
    void *palette_data;
    u32 palette_bytes;
    GameTextureAllocation texture;
    GameTexturePalette palette;
    u8 trail_alpha[6], unknown_46[2];
} TitlePanelResources;
typedef char TitlePanelResources_SizeCheck[sizeof(TitlePanelResources) == 72 ? 1 : -1];
typedef struct TitlePanelLayoutValue {
    s16 value, unknown[2];
} TitlePanelLayoutValue;
typedef char TitlePanelTrail_SizeCheck[sizeof(TitlePanelTrail) == 8 ? 1 : -1];
typedef char TitlePanelLayoutValue_SizeCheck[sizeof(TitlePanelLayoutValue) == 6 ? 1 : -1];
typedef struct TitleMovingSprite TitleMovingSprite;
struct TitleMovingSprite {
    void *unknown_00;
    s32 elapsed;
    u32 unknown_08;
    s32 x, y, depth;
    void (*update)(TitleMovingSprite *);
    void (*draw)(TitleMovingSprite *);
    u32 unknown_20;
    u8 state, unknown_25, model_mask, unknown_27;
    BattleModel *sub_model, *main_model;
    void *child;
    void *parent;
    s32 unknown_38, velocity;
    struct {
        u8 side : 1, variant : 1, unknown_02_07 : 6;
    } flags;
    u8 unknown_41;
    s16 frame;
};
typedef char TitleMovingSprite_SizeCheck[sizeof(TitleMovingSprite) == 68 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void TitlePanel_Release(TitlePanel *panel);
void TitlePanel_StartFadeIn(TitlePanel *panel);
void TitlePanel_StartPulse(TitlePanel *panel);
void TitlePanel_TryStartTrail(TitlePanel *panel, int chance_denominator);
void TitleElement_Append(TitleElementLink *element);
void TitlePanel_Init(TitlePanel *panel, TitlePanelResources *resources, int index);
void TitlePanel_Update(TitlePanel *panel);
int TitlePanel_DrainTrail(TitlePanel *panel);
void TitlePanel_RecordTrail(TitlePanel *panel);
void TitlePanel_ResetPulse(TitlePanel *panel);
void TitleMovingSprite_Destroy(TitleMovingSprite *work);
void TitleMovingSprite_StartOrbit(TitleMovingSprite *work);
void TitleMovingSprite_StartEntry(TitleMovingSprite *work);
void TitleMovingSprite_Init(TitleMovingSprite *work, void *child, int side, int variant, void *parent);
#ifdef __cplusplus
}
#endif
#endif
