#ifndef PIT_BATTLE_MOTION_EFFECTS_INTERNAL_H
#define PIT_BATTLE_MOTION_EFFECTS_INTERNAL_H
extern "C" {
#include <nitro.h>
#include <nitro/fx_atan.h>
extern s16 FX_SinCosTable_[];
}
#include <game/battle_scene.h>
struct BattleTransformMotion {
    s16 scale_x[2], scale_y[2];
    u16 rotation[2], angle;
    u8 mode : 2, unknown : 6;
    u8 unknown15;
};
struct BattlePursuitMotion {
    BattleSceneObject *target;
    int speed, elapsed;
    int *position;
};
struct BattleFollowMotion {
    BattleSceneObject *object;
    s16 offset[3];
};
struct BattleSineMotion {
    s16 offset[3];
    u16 angle_start, angle_end;
};
struct BattleShrinkMotion {
    s16 delay, duration;
    union {
        u8 raw;
        struct {
            u8 mode : 2, alpha : 5, unknown : 1;
        } bits;
    };
    u8 unknown5, phase, unknown7;
};
extern "C" {
void func_ov002_0206e89c(BattleSceneObject *, BattleMotionChannel *);
void func_ov002_0206ea08(BattleSceneObject *, BattleMotionChannel *);
void func_ov002_0206ecfc(BattleSceneObject *, BattleMotionChannel *);
void func_ov002_0206efa0(BattleSceneObject *, BattleMotionChannel *);
void BattleMotion_SetRangeParameters(s16 *parameters, s16 value, s16 start, s16 end);
void BattleMotion_StartScaleWave(BattleSceneObject *object, int channel, int duration, int amplitude);
void BattleMotion_UpdateScaleWave(BattleSceneObject *object, BattleMotionChannel *channel);
void BattleMotion_StartTransform(BattleSceneObject *object, int channel, int sx0, int sy0, int angle0,
                                 int sx1, int sy1, int angle1, int duration, int mode, int angle);
int BattleMotion_StartPursuit(BattleSceneObject *object, int channel, int storage_channel,
                              BattleSceneObject *target, int speed);
int BattleMotion_StartFollower(BattleSceneObject *object, BattleSceneObject *follower, int channel);
void BattleMotion_UpdateFollower(BattleSceneObject *object, BattleMotionChannel *channel);
int BattleMotion_StartSineMove(BattleSceneObject *object, int channel, int x, int y, int z, int start,
                               int end, int duration, int absolute);
void BattleMotion_StartAppear(BattleSceneObject *object, int channel, int duration);
void BattleMotion_UpdateAppear(BattleSceneObject *object, BattleMotionChannel *channel);
void BattleMotion_StartShrink(BattleSceneObject *object, int channel, int duration, int delay, int mode);
}
typedef char BattleTransformMotionSize[sizeof(BattleTransformMotion) == 16 ? 1 : -1];
typedef char BattlePursuitMotionSize[sizeof(BattlePursuitMotion) == 16 ? 1 : -1];
typedef char BattleFollowMotionSize[sizeof(BattleFollowMotion) == 12 ? 1 : -1];
typedef char BattleSineMotionSize[sizeof(BattleSineMotion) == 10 ? 1 : -1];
typedef char BattleShrinkMotionSize[sizeof(BattleShrinkMotion) == 8 ? 1 : -1];
#endif
