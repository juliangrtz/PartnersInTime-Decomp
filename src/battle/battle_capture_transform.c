/* Capture transform initialization, overlay 2, 0x020B03B8-0x020B0484. */
#include <nitro/fx_mtx.h>
#include <game/battle_capture_surface.h>
#include <game/battle_context.h>
#include <game/battle_task_queue.h>
/* Initialized prefix; this does not establish the complete owner allocation. */
struct BattleCaptureTransform {
    void *buffer;
    s16 x, y;
    u16 enabled, unknown0a;
    MtxFx44 transform;
    MtxFx44 projection;
};
typedef char CaptureTransformPrefixSize[sizeof(BattleCaptureTransform) == 140 ? 1 : -1];
void MTX_Identity44_(MtxFx44 *);
void func_0202cbd4(void *, int, u32);
BattleQueuedTask *BattleCaptureSurface_QueueUpload(u32, int, int, int, int, int);
static inline u8 *CaptureWorkBuffer(void)
{
    return gBattleContext + 226824;
}
void BattleCaptureTransform_Initialize(BattleCaptureTransform *state, void *buffer, int x, int y)
{
    BattleCaptureSurface_QueueUpload(0x38000, 14, x, y, 256, 128);
    if (!buffer) {
        u8 *base = CaptureWorkBuffer();
        /* Keep the native base-buffer and sub-buffer additions separate.
         * Metrowerks folds the C expression into one offset otherwise. */
        asm { add buffer, base, #0xC000 }
    }
    if (!state) state = (BattleCaptureTransform *)(gBattleContext + 52984);
    state->enabled = 1;
    state->buffer = buffer;
    state->x = x;
    state->y = y;
    MTX_Identity44_(&state->transform);
    state->transform._00 = 8192;
    state->transform._11 = 8192;
    state->transform._32 = 4096;
    MTX_Identity44_(&state->projection);
    state->projection._30 = 0x800000;
    func_0202cbd4(buffer, 0, 3496);
}
