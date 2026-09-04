#include <game/battle_scene.h>

extern void OS_Terminate(void);

enum {
    BATTLE_SCENE_OBJECT_ID_LIMIT = 70
};

int BattleSceneObject_IsAnimationActiveById(u32 object_id,
                                            int channel_index) {
    if (object_id >= BATTLE_SCENE_OBJECT_ID_LIMIT) {
        OS_Terminate();
    }
    return BattleSceneObject_IsAnimationChannelActive(
        BattleSceneObject_GetById(object_id), channel_index);
}

int BattleSceneObject_IsAnimationChannelActive(BattleSceneObject *object,
                                               int channel_index) {
    if (channel_index >= BATTLE_MOTION_CHANNEL_COUNT) {
        OS_Terminate();
    }
    if (object->motion_channels[channel_index].callback == 0) {
        return 0;
    }
    return 1;
}
