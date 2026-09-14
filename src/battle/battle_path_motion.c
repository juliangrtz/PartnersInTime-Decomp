#include <game/battle_scene.h>
extern int func_ov002_020722d8(s16 *, int *, int *);
void BattleMotion_UpdatePath(BattleSceneObject *object, BattleMotionChannel *channel) {
    int x, y;
    int active = func_ov002_020722d8(channel->parameters, &x, &y);
    object->x = x;
    object->y = y;
    object->motion_target_x = object->x;
    object->motion_target_y = object->y;
    if (!active)
        channel->callback = 0;
}
