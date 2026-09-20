/*
 * Background layer movement (overlay 0, 0x020721EC-0x02072A30).
 * Three independent Q12 motion records feed relative Q8 background scrolling.
 * Motions run for a fixed duration or follow a speed/acceleration profile.
 * Target checks clamp completed axes and enable braking before the next step.
 */

extern "C" {
#include <game/field_geometry.h>
}
#include <game/field_area_motion.h>
#include <game/field_background.h>
#define LAYER(i) (area->layer_motion[i])
extern "C" {
void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);

int FieldArea_MoveLayerWithProfile(FieldAreaContext *area, int layer, fx32 dx, fx32 dy, fx32 speed,
                                   fx32 acceleration, fx32 maximum, fx32 deceleration, u8 stop)
{
    LAYER(layer).flags.bits.x_active = dx != 0;
    LAYER(layer).flags.bits.y_active = dy != 0;
    if (LAYER(layer).flags.axes.active_axes) {
        LAYER(layer).flags.bits.paused = 0;
        LAYER(layer).flags.bits.timed = 0;
        LAYER(layer).flags.bits.braking = 0;
        LAYER(layer).elapsed = 0;
        LAYER(layer).flags.bits.stop = stop;
        LAYER(layer).target_x = dx;
        LAYER(layer).target_y = dy;
        LAYER(layer).x = 0;
        LAYER(layer).y = 0;
        LAYER(layer).previous_x = 0;
        LAYER(layer).previous_y = 0;
        LAYER(layer).direction = FieldArea_GetMotionDirection(area, dx, dy);
        LAYER(layer).speed = speed;
        LAYER(layer).acceleration = acceleration;
        LAYER(layer).maximum_speed = maximum;
        LAYER(layer).deceleration = deceleration;
        func_ov000_020be654(dx, dy, speed, &LAYER(layer).vx, &LAYER(layer).vy);
        if (LAYER(layer).flags.bits.x_active && !LAYER(layer).vx)
            LAYER(layer).vx = dx > 0 ? 1 : -1;
        if (LAYER(layer).flags.bits.y_active && !LAYER(layer).vy)
            LAYER(layer).vy = dy > 0 ? 1 : -1;
        return 1;
    }
    return 0;
}

int FieldArea_MoveLayerForDuration(FieldAreaContext *area, int layer, fx32 dx, fx32 dy,
                                   int duration, u8 stop)
{
    LAYER(layer).flags.bits.x_active = dx != 0;
    LAYER(layer).flags.bits.y_active = dy != 0;
    LAYER(layer).flags.bits.paused = 0;
    LAYER(layer).flags.bits.timed = 1;
    LAYER(layer).flags.bits.braking = 0;
    LAYER(layer).elapsed = 0;
    LAYER(layer).flags.bits.stop = stop;
    LAYER(layer).target_x = dx;
    LAYER(layer).target_y = dy;
    LAYER(layer).x = 0;
    LAYER(layer).y = 0;
    LAYER(layer).previous_x = 0;
    LAYER(layer).previous_y = 0;
    LAYER(layer).direction = FieldArea_GetMotionDirection(area, dx, dy);
    LAYER(layer).speed = duration;
    if (LAYER(layer).flags.bits.x_active)
        LAYER(layer).vx = dx / duration;
    if (LAYER(layer).flags.bits.y_active)
        LAYER(layer).vy = dy / duration;
    return 1;
}

void FieldArea_AdvanceLayerMotion(FieldAreaContext *area)
{
    int i;
    for (i = 0; i < 3; ++i) {
        if (LAYER(i).flags.axes.active_axes && !LAYER(i).flags.bits.paused) {
            LAYER(i).previous_x = LAYER(i).x;
            LAYER(i).previous_y = LAYER(i).y;
            if (!LAYER(i).flags.bits.timed && LAYER(i).flags.bits.braking) {
                LAYER(i).speed += LAYER(i).deceleration;
                if (LAYER(i).speed < -LAYER(i).deceleration)
                    LAYER(i).speed = -LAYER(i).deceleration;
                func_ov000_020be654(LAYER(i).target_x - LAYER(i).x, LAYER(i).target_y - LAYER(i).y,
                                    LAYER(i).speed, &LAYER(i).vx, &LAYER(i).vy);
            }
            /* Read both velocities before updating either coordinate. */
            int dx = LAYER(i).flags.bits.x_active ? LAYER(i).vx : 0;
            int dy = LAYER(i).flags.bits.y_active ? LAYER(i).vy : 0;
            LAYER(i).x += dx;
            LAYER(i).y += dy;
            if (!LAYER(i).flags.bits.timed && !LAYER(i).flags.bits.braking) {
                if (LAYER(i).acceleration > 0 && LAYER(i).speed < LAYER(i).maximum_speed) {
                    LAYER(i).speed += LAYER(i).acceleration;
                    if (LAYER(i).speed > LAYER(i).maximum_speed)
                        LAYER(i).speed = LAYER(i).maximum_speed;
                    func_ov000_020be654(LAYER(i).target_x - LAYER(i).x,
                                        LAYER(i).target_y - LAYER(i).y, LAYER(i).speed,
                                        &LAYER(i).vx, &LAYER(i).vy);
                }
            }
            ++LAYER(i).elapsed;
        }
    }
}

int FieldArea_UpdateLayerTargets(FieldAreaContext *area)
{
    int active = 0;
    for (int i = 0; i < 3; ++i) {
        if (LAYER(i).flags.axes.active_axes && !LAYER(i).flags.bits.paused) {
            if (LAYER(i).flags.bits.stop) {
                if (!LAYER(i).flags.bits.timed) {
                    if (LAYER(i).flags.bits.x_active) {
                        if ((LAYER(i).target_x < 0 && LAYER(i).x <= LAYER(i).target_x) ||
                            (LAYER(i).target_x > 0 && LAYER(i).x >= LAYER(i).target_x)) {
                            LAYER(i).x = LAYER(i).target_x;
                            LAYER(i).flags.bits.x_active = 0;
                        }
                    }
                    if (LAYER(i).flags.bits.y_active) {
                        if ((LAYER(i).target_y < 0 && LAYER(i).y <= LAYER(i).target_y) ||
                            (LAYER(i).target_y > 0 && LAYER(i).y >= LAYER(i).target_y)) {
                            LAYER(i).y = LAYER(i).target_y;
                            LAYER(i).flags.bits.y_active = 0;
                        }
                    }
                    if (LAYER(i).flags.axes.active_axes && LAYER(i).deceleration < 0) {
                        int speed = LAYER(i).speed;
                        int distance = 0;
                        /* Include the final nonpositive speed step in the braking distance. */
                        do {
                            speed += LAYER(i).deceleration;
                            distance += speed;
                        } while (speed > 0);
                        if (distance >=
                            FieldGeometry_GetVectorLength(LAYER(i).target_x - LAYER(i).x,
                                                          LAYER(i).target_y - LAYER(i).y, 0))
                            LAYER(i).flags.bits.braking = 1;
                        else
                            LAYER(i).flags.bits.braking = 0;
                    }
                } else if (LAYER(i).elapsed == LAYER(i).speed) {
                    LAYER(i).target_x = LAYER(i).x;
                    LAYER(i).target_y = LAYER(i).y;
                    LAYER(i).flags.axes.active_axes = 0;
                }
            }
            area->background->offset_scroll((u8)i, (LAYER(i).x - LAYER(i).previous_x) / 16,
                                            (LAYER(i).y - LAYER(i).previous_y) / 16);
        }
        /* The native result shifts the two-bit axis mask by the layer index. */
        active |= LAYER(i).flags.axes.active_axes << i;
    }
    return active;
}
}
