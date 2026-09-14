#ifndef PIT_GAME_BATTLE_CURVE_MOTION_H
#define PIT_GAME_BATTLE_CURVE_MOTION_H
#include <game/battle_scene.h>
#ifdef __cplusplus
extern "C" {
#endif
void BattleMotion_StartAxisRotation(BattleSceneObject *object, int channel, s16 axis_start_x,
                                    s16 axis_start_y, s16 axis_start_z, s16 axis_end_x,
                                    s16 axis_end_y, s16 axis_end_z, s16 angle_step,
                                    s16 total_angle);
void BattleMotion_StartSineDisplacement(BattleSceneObject *object, int channel, int x, int y, int z,
                                        int phase, int step, int cycles, int final_radius);
void BattleMotion_UpdatePath(BattleSceneObject *object, BattleMotionChannel *channel);
#ifdef __cplusplus
}
#endif
#endif
