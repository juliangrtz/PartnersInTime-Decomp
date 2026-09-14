#include <game/battle_context.h>
#include <game/battle_sprite_grid_capture.h>
#include "battle_scheduler_internal.h"
#include <game/random.h>
#include <nitro/gx_init.h>

typedef union GridCaptureFlags {
    u8 raw;
    struct { u8 active : 1; u8 distortion_only : 1; u8 unknown : 6; } bits;
} GridCaptureFlags;

typedef struct GridCaptureState {
    s16 phase;
    s16 cell_delay_range;
    s16 quarter_delay_range;
    u16 selected_cells;
    u16 visited_cells;
    u8 started_cells;
    u8 finished_cells;
    s8 displacement;
    u8 origin_line;
    u8 first_line;
    u8 band;
    s8 timer;
    GridCaptureFlags flags;
    u8 unknown_012[1028 - 18];
} GridCaptureState;

typedef struct GridCaptureOverride {
    int (*render)(BattleSceneObject *, int);
    GridCaptureState *state;
} GridCaptureOverride;

extern GridCaptureOverride *data_ov002_020c0dc8;
extern u32 SDK_AUTOLOAD_DTCM_START[];
extern u32 OS_DisableIrqMask(u32 mask);
extern u32 OS_EnableIrqMask(u32 mask);
extern void OS_SetIrqFunction(u32 mask, void (*callback)(void));
extern BattleTransferTask *BattleTransfer_EnqueueAfterMapping(BattleTransferCallback, int, int, int);
extern GridCaptureOverride *func_ov002_020bb1c4(BattleSceneObject *, int);
extern int func_ov002_020b8ca4(BattleSceneObject *, int);
extern void func_0202cbd4(void *, int, int);
void BattleSpriteGridCapture_OnVCountInterrupt(void);
void BattleSpriteGridCapture_UpdateScanlines(BattleTransferTask *unused);

/* The native effect alternates four 16-line bands on sub-screen BG1. */
static inline void set_sub_bg1_offset(u16 x, u16 y) {
    *(vu32 *)0x04001014 = (x & 0x1FF) | ((y << 16) & 0x1FF0000);
}

typedef char GridCaptureState_SizeCheck[sizeof(GridCaptureState) == 1028 ? 1 : -1];
typedef char GridCaptureOverride_SizeCheck[sizeof(GridCaptureOverride) == 8 ? 1 : -1];

/* Metrowerks emits functions in reverse source order. */
void BattleSpriteGridCapture_Initialize(BattleSceneObject *object) {
    GridCaptureOverride *capture = func_ov002_020bb1c4(object, 1028);
    func_0202cbd4(capture->state, 0, 1028);
    data_ov002_020c0dc8 = capture;
    capture->render = func_ov002_020b8ca4;
}

void BattleSpriteGridCapture_Start(BattleSceneObject *object, int cell_delay_range,
                         int quarter_delay_range, int count, int distortion_only) {
    GridCaptureState *state = ((GridCaptureOverride *)object->render_override)->state;
    int i;
    if (!state->flags.bits.active && state->phase != 0) return;
    if (state->flags.bits.active && state->phase != 5) return;
    state->selected_cells = 0;
    for (i = 0; i < 16; ++i) {
        if (Random_NextModulo(16 - i) < count) {
            --count;
            state->selected_cells |= 1 << i;
        }
    }
    state->cell_delay_range = cell_delay_range;
    state->quarter_delay_range = quarter_delay_range;
    state->visited_cells = 0;
    state->started_cells = 0;
    state->finished_cells = 0;
    state->flags.bits.distortion_only = distortion_only;
    if (distortion_only) state->flags.bits.active = 0;
    if (state->flags.bits.active) {
        state->phase = 4;
        return;
    }
    state->phase = 1;
    state->flags.bits.active = 1;
    BattleTransfer_EnqueueAfterMapping(BattleSpriteGridCapture_UpdateScanlines, 0, 0, 0);
}

void BattleSpriteGridCapture_Advance(BattleSceneObject *object, int cell_delay_range, int quarter_delay_range) {
    GridCaptureState *state = ((GridCaptureOverride *)object->render_override)->state;
    state->cell_delay_range = cell_delay_range;
    state->quarter_delay_range = quarter_delay_range;
    state->visited_cells = 0;
    state->started_cells = 0;
    state->finished_cells = 0;
    state->phase = state->flags.bits.distortion_only ? 8 : 6;
}

void BattleSpriteGridCapture_Finish(BattleSceneObject *object, int cell_delay_range, int quarter_delay_range) {
    GridCaptureState *state = ((GridCaptureOverride *)object->render_override)->state;
    state->cell_delay_range = cell_delay_range;
    state->quarter_delay_range = quarter_delay_range;
    state->visited_cells = 0;
    state->started_cells = 0;
    state->finished_cells = 0;
    state->flags.bits.active = 0;
    state->phase = state->flags.bits.distortion_only ? 8 : 6;
}

int BattleSpriteGridCapture_IsActive(BattleSceneObject *object) {
    GridCaptureState *state = ((GridCaptureOverride *)object->render_override)->state;
    if (state->phase == 0) return 0;
    return state->phase != 5;
}

void BattleSpriteGridCapture_UpdateScanlines(BattleTransferTask *unused) {
    GridCaptureState *state = data_ov002_020c0dc8->state;
    if (state->phase == 1) {
        if (state->timer > 0) --state->timer;
        if (state->timer < 2) ++state->displacement;
        if (state->timer == 0) state->timer = 16;
        if (state->displacement == 8) {
            state->timer = 0;
            state->band = 0;
            state->first_line = 0;
            state->phase = state->flags.bits.distortion_only ? 5 : 2;
        }
    }
    if (state->phase == 8) {
        if (state->timer > 0) --state->timer;
        if (state->timer < 2) --state->displacement;
        if (state->timer == 0) state->timer = 16;
        if (state->displacement == 0) {
            state->timer = 0;
            state->band = 0;
            state->first_line = 0;
            state->phase = 0;
            return;
        }
    }
    state->first_line = state->origin_line;
    state->band = 0;
    *(vu16 *)0x04000004 &= ~0x20;
    OS_DisableIrqMask(4);
    OS_SetIrqFunction(4, BattleSpriteGridCapture_OnVCountInterrupt);
    GX_SetVCountEqVal(state->first_line + 16 * (state->band + 1));
    OS_EnableIrqMask(4);
    *(vu16 *)0x04000004 |= 0x20;
    BattleTransfer_EnqueueAfterMapping(BattleSpriteGridCapture_UpdateScanlines, 0, 0, 0);
}

void BattleSpriteGridCapture_OnVCountInterrupt(void) {
    GridCaptureState *state = data_ov002_020c0dc8->state;
    switch (state->band) {
    case 0:
        set_sub_bg1_offset(*(s16 *)(gBattleContext + 0xCBA0) + 128 + state->displacement,
                           *(s16 *)(gBattleContext + 0xCBA2) + 32);
        ++state->band;
        GX_SetVCountEqVal(state->first_line + 16 * (state->band + 1));
        break;
    case 1:
        set_sub_bg1_offset(*(s16 *)(gBattleContext + 0xCBA0) + 128,
                           *(s16 *)(gBattleContext + 0xCBA2) + 32);
        ++state->band;
        GX_SetVCountEqVal(state->first_line + 16 * (state->band + 1));
        break;
    case 2:
        set_sub_bg1_offset(*(s16 *)(gBattleContext + 0xCBA0) + 128 + state->displacement,
                           *(s16 *)(gBattleContext + 0xCBA2) + 32);
        ++state->band;
        GX_SetVCountEqVal(state->first_line + 16 * (state->band + 1));
        break;
    case 3:
        set_sub_bg1_offset(*(s16 *)(gBattleContext + 0xCBA0) + 128,
                           *(s16 *)(gBattleContext + 0xCBA2) + 32);
        *(vu16 *)0x04000004 &= ~0x20;
        OS_DisableIrqMask(4);
        break;
    }
    *(u32 *)((u32)SDK_AUTOLOAD_DTCM_START + 0x3FF8) |= 4;
}
