#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_scene.h>

enum BattleTargetCursorMovementConstant {
    BATTLE_ACTIVE_ACTOR_ID_OFFSET = 0x20,
    BATTLE_TARGET_CURSOR_OBJECT_OFFSET = 0x6734,
    BATTLE_TARGET_CURSOR_ANIMATION_OFFSET = 0x6740
};

extern void func_ov002_020a3b2c(BattleSceneObject *object, int channel_index,
                                int x, int y, int z, int duration,
                                BattleSceneObject *target);

void BattleTargetCursor_MoveToGroup(int duration, int x, int y,
                                    int animation_id) {
    if (animation_id >= 0) {
        BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetPartySlot(
            *(u16 *)(gBattleContext + BATTLE_ACTIVE_ACTOR_ID_OFFSET));

        *(u16 *)(gBattleContext + BATTLE_TARGET_CURSOR_ANIMATION_OFFSET) =
            animation_id + *(u16 *)actor->actor.resource_slot;
    }
    BattleSceneObject_UnlinkMotion(
        *(BattleSceneObject **)(gBattleContext +
                                BATTLE_TARGET_CURSOR_OBJECT_OFFSET));
    BattleSceneObject_MoveTo(
        *(BattleSceneObject **)(gBattleContext +
                                BATTLE_TARGET_CURSOR_OBJECT_OFFSET),
        3, x, 256, 256 - y, duration);
}

void BattleTargetCursor_MoveToActor(int duration, int actor_id,
                                    int animation_id) {
    BattleSceneObject *target;

    if (animation_id >= 0) {
        BattlePartyActor *actor = (BattlePartyActor *)BattleActor_GetPartySlot(
            *(u16 *)(gBattleContext + BATTLE_ACTIVE_ACTOR_ID_OFFSET));

        *(u16 *)(gBattleContext + BATTLE_TARGET_CURSOR_ANIMATION_OFFSET) =
            animation_id + *(u16 *)actor->actor.resource_slot;
    }
    target = BattleSceneObject_GetById(actor_id);
    func_ov002_020a3b2c(
        *(BattleSceneObject **)(gBattleContext +
                                BATTLE_TARGET_CURSOR_OBJECT_OFFSET),
        3, target->property_0fc, target->property_0fd, target->property_0f8,
        duration, target);
}
