#include <game/battle_transition.h>
#include <nitro.h>
#include <game/battle_context.h>
#include <game/battle_capture_surface.h>

#include <game/battle_task_queue.h>
extern BattleQueuedTask *BattleCaptureSurface_QueueUpload(u32, int, int, int, int, int);

/* Capture alpha, origin and current camera coordinates in the battle context. */
#define REG32(address) (*(volatile u32 *)(address))
#define FADE (*(s16 *)(gBattleContext + 54162))
#define CAPTURE_X (*(s16 *)(gBattleContext + 54164))
#define CAPTURE_Y (*(s16 *)(gBattleContext + 54166))
#define VIEW_X (*(s16 *)(gBattleContext + 52124))
#define VIEW_Y (*(s16 *)(gBattleContext + 52126))

void BattleCaptureSurface_UpdateFade(void) {
    s16 x, y;
    if ((BattleObjectData_GetLoadState(14)->flags.raw << 1) >> 31)
        return;
    if (!((BattleRuntimeFlags *)(gBattleContext + 54176))->bits.global_property_23) {
        s16 *level = (s16 *)(gBattleContext + 54162);
        if (*level > 0)
            --*level;
        if (!FADE)
            return;
    } else {
        s16 *level = (s16 *)(gBattleContext + 54162);
        if (!*level) {
            *level = 1;
            CAPTURE_X = VIEW_X;
            CAPTURE_Y = VIEW_Y + 80;
            BattleCaptureSurface_QueueUpload(0x38000, 14, CAPTURE_X, CAPTURE_Y, 256, 128);
            return;
        }
        if (*level < 31)
            ++*level;
    }
    /* Keep the native signed halfword truncation before Q8 translation. */
    x = CAPTURE_X - VIEW_X;
    y = CAPTURE_Y - VIEW_Y;
    REG32(0x040004a4) = (FADE << 16) | 0x80;
    REG32(0x040004a8) = 0x52507000;
    REG32(0x040004ac) = *(u32 *)(gBattleContext + 52100) >> 4;
    REG32(0x04000480) = 0x7fff;
    /* Position and texture matrices both scale the captured quad by four. */
    REG32(0x04000440) = 2;
    REG32(0x04000454) = 0;
    REG32(0x04000470) = x * 256;
    REG32(0x04000470) = y * 256;
    REG32(0x04000470) = 0x1fff;
    REG32(0x0400046c) = 0x4000;
    REG32(0x0400046c) = 0x4000;
    REG32(0x0400046c) = 0x1000;
    REG32(0x04000440) = 3;
    REG32(0x04000454) = 0;
    REG32(0x0400046c) = 0x4000;
    REG32(0x0400046c) = 0x4000;
    REG32(0x0400046c) = 0x1000;
    REG32(0x04000500) = 1;
    REG32(0x04000488) = 0;
    REG32(0x0400048c) = 0;
    REG32(0x0400048c) = 0;
    REG32(0x04000488) = 0x02000000;
    REG32(0x0400048c) = 0x1fc00000;
    REG32(0x0400048c) = 0;
    REG32(0x04000488) = 0x02000400;
    REG32(0x0400048c) = 0x1fc04000;
    REG32(0x0400048c) = 0;
    REG32(0x04000488) = 0x400;
    REG32(0x0400048c) = 0x4000;
    REG32(0x0400048c) = 0;
    REG32(0x04000504) = 0;
    REG32(0x04000454) = 0;
}
