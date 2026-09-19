/*
 * Camera motion stop (overlay 0, 0x02072A30-0x02072A9C).
 *
 * Stops the camera's current move and reports the direction it was going.
 */

#include <game/field_area_motion.h>
#include <nitro/fx_atan.h>
void FieldArea_StopCameraMotion(FieldAreaContext *area, FieldCameraMotion *motion, int snap) {
    if (!motion)
        motion = (FieldCameraMotion *)area->unknown_2b90;
    if (motion->flags.bits.active_axes) {
        motion->flags.raw &= ~3;
        if (snap) {
            area->camera_x = motion->end_x;
            area->camera_y = motion->end_y;
        }
    }
}

u16 FieldArea_GetMotionDirection(FieldAreaContext *unused, fx32 dx, fx32 dy) {
    return FX_Atan2Idx(dy, dx) + 0x4000;
}
