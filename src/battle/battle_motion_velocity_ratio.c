/*
 * Velocity ratio motion (overlay 2, 0x020BE478-0x020BE504).
 *
 * Starts a motion from a ratio between two velocities rather than an absolute
 * speed.
 */

#include <game/battle_scene.h>

int BattleMotion_StartWithVelocityRatio(BattleSceneObject *object, int channel, int direction_x,
                                        int direction_y, int direction_z, int distance,
                                        int duration, int velocity_ratio_q8,
                                        int prefer_longer_duration) {
    int distance_scaled = (s32)((u32)distance << 17) / duration;
    int velocity = distance_scaled / (s32)((u32)velocity_ratio_q8 + 256);
    int initial_velocity = (s32)((u32)velocity << 8);
    int acceleration = (s32)((u32)velocity * velocity_ratio_q8 - ((u32)velocity << 8)) / duration;
    return BattleSceneObject_StartAcceleratedMotion(object, channel, direction_x, direction_y,
                                                    direction_z, distance, initial_velocity,
                                                    acceleration, prefer_longer_duration);
}
