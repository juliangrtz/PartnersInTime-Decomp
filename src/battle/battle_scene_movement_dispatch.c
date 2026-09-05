#include <game/battle_scene.h>

extern void OS_Terminate(void);
extern void func_ov002_020a3928(BattleSceneObject *object, int channel_index,
                                int x, int y, int z, int duration,
                                int argument_7, int argument_8,
                                int argument_9);
extern void func_ov002_020a3b2c(BattleSceneObject *object, int channel_index,
                                int x, int y, int z, int duration,
                                BattleSceneObject *target);
enum BattleSceneMovementKind {
    BATTLE_SCENE_MOVEMENT_SET_POSITION,
    BATTLE_SCENE_MOVEMENT_MOVE_BY,
    BATTLE_SCENE_MOVEMENT_MOVE_TO,
    BATTLE_SCENE_MOVEMENT_ARC_A,
    BATTLE_SCENE_MOVEMENT_ARC_B,
    BATTLE_SCENE_MOVEMENT_TOWARD_OBJECT,
    BATTLE_SCENE_MOVEMENT_ACCELERATED
};

enum { BATTLE_SCENE_OBJECT_COUNT = 70 };

void BattleSceneObject_ApplyMovement(u32 object_id, int channel_index,
                                     int movement_kind, int x, int y, int z,
                                     int duration, int target_object_id,
                                     int argument_8, int argument_9) {
    BattleSceneObject *object;

    if (object_id >= BATTLE_SCENE_OBJECT_COUNT) {
        OS_Terminate();
    }
    object = BattleSceneObject_GetById(object_id);

    switch (movement_kind) {
        case BATTLE_SCENE_MOVEMENT_SET_POSITION:
            BattleSceneObject_AdjustPosition(
                object, x - object->x, y - object->y, z - object->z);
            return;
        case BATTLE_SCENE_MOVEMENT_MOVE_BY:
            BattleSceneObject_MoveBy(
                object, channel_index, x, y, z, duration);
            return;
        case BATTLE_SCENE_MOVEMENT_MOVE_TO:
            BattleSceneObject_MoveTo(
                object, channel_index, x, y, z, duration);
            return;
        case BATTLE_SCENE_MOVEMENT_ARC_A:
            BattleSceneObject_StartVerticalArc(
                object, channel_index, duration, target_object_id, z);
            return;
        case BATTLE_SCENE_MOVEMENT_ARC_B:
            BattleSceneObject_StartVerticalMotionTo(
                object, channel_index, duration, target_object_id, z, 1);
            return;
        case BATTLE_SCENE_MOVEMENT_TOWARD_OBJECT:
            func_ov002_020a3b2c(
                object, channel_index, x, y, z, duration,
                BattleSceneObject_GetById((u16)target_object_id));
            return;
        case BATTLE_SCENE_MOVEMENT_ACCELERATED:
            func_ov002_020a3928(
                object, channel_index, x, y, z, duration, target_object_id,
                argument_8, argument_9);
            return;
        default:
            return;
    }
}
