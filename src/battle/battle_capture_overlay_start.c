#include <game/battle_effect_controls.h>
#include <game/battle_context.h>
typedef struct CaptureOverlayView {
    u8 prefix[52168];
    s16 timer, phase;
    u8 unknown[8];
    u16 *palette;
} CaptureOverlayView;
#define CAPTURE ((CaptureOverlayView *)gBattleContext)
void *BattleTransfer_EnqueueAfterMapping(void (*)(void *), int, int, int);
void func_ov002_020b0484(void *);
void BattleCaptureOverlay_Start(void) {
    if (!CAPTURE->timer) {
        BattleTransfer_EnqueueAfterMapping(func_ov002_020b0484, 0, 0, 0);
        CAPTURE->phase = 2;
    }
    CAPTURE->timer = 64;
}
