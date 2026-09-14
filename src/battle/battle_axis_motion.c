#include <game/battle_scene.h>
void func_ov002_020a33a4(s16 *position, int angle, int ax, int ay, int az, int bx, int by, int bz);
void func_ov002_020a39d0(BattleSceneObject *, BattleMotionChannel *);
void BattleMotion_UpdateAxisRotation(BattleSceneObject *, BattleMotionChannel *);
void BattleMotion_StartSineDisplacement(BattleSceneObject *object, int index, int x, int y, int z,
                                        int phase, int speed, int cycles, int radius) {
    int duration;
    s16 *parameters;
    if (speed < 0) {
        speed = -speed;
        phase = -phase;
    }
    if (!cycles)
        duration = 0;
    else {
        duration = (speed - 1 + ((cycles << 16) - phase)) / speed;
        if (duration <= 0)
            return;
    }
    parameters = BattleSceneObject_BeginMotionChannel(object, index, duration, func_ov002_020a39d0);
    parameters[0] = x;
    parameters[1] = y;
    parameters[2] = z;
    parameters[3] = phase;
    parameters[4] = speed;
    parameters[5] = radius;
}

void BattleMotion_UpdateAxisRotation(BattleSceneObject *object, BattleMotionChannel *channel) {
    s16 position[3];
    s16 *parameters = channel->parameters;
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    func_ov002_020a33a4(position, parameters[6] * channel->elapsed_q8 / 256, parameters[0],
                        parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
    object->x = position[0];
    object->y = position[1];
    object->z = position[2];
}

void BattleMotion_StartAxisRotation(BattleSceneObject *object, int index, s16 ax, s16 ay, s16 az,
                                    s16 bx, s16 by, s16 bz, s16 speed, s16 angle) {
    int duration = (angle << 8) / speed;
    s16 *parameters = BattleSceneObject_BeginMotionChannel(object, index, duration,
                                                           BattleMotion_UpdateAxisRotation);
    parameters[0] = ax;
    parameters[1] = ay;
    parameters[2] = az;
    parameters[3] = bx;
    parameters[4] = by;
    parameters[5] = bz;
    parameters[6] = speed;
    parameters[7] = angle;
}
