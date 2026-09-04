#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>
#include <hardware.h>

enum BattlePartyFormationOffset {
    BATTLE_NEXT_STATE_OFFSET = 0x18,
    BATTLE_PARTY_TASK_POOL_OFFSET = 0x8B44
};

enum BattlePartyTransitionConstant {
    BATTLE_PARTY_TRANSITION_OBJECT_DATA_ID = 52,
    BATTLE_PARTY_TRANSITION_RESOURCE_INDEX = 2432,
    BATTLE_PARTY_TRANSITION_TASK_STATE = 0x7000
};

typedef struct BattlePartyFormationTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    u16 next_state;
    u16 actor_id;
    u16 mario_target_x;
    u16 luigi_target_x;
} BattlePartyFormationTask;

typedef char BattlePartyFormationTask_SizeCheck
    [sizeof(BattlePartyFormationTask) == 0x14 ? 1 : -1];

int BattleParty_SpawnLaunchImpact(BattleActor *actor);

int BattleParty_UpdateFormationTransition(BattleAITask *task);

int BattleParty_StartFormationTransition(int next_state, u16 mario_target_x,
                                         u16 luigi_target_x) {
    s32 target_resource_id;
    BattlePartyActor *actor;
    int actor_id;
    BattleSceneObject *scene_object;
    BattleSceneObject *linked_object;
    BattleSceneResource *resource;
    BattlePartyFormationTask *task;

    for (actor_id = BATTLE_ACTOR_PARTY_FIRST;; ++actor_id) {
        if (actor_id > BATTLE_ACTOR_PARTY_FIRST + 1) {
            return next_state;
        }

        actor = (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
        if (!BattleActor_CanReceiveStatus(
                BattleActor_GetById((u16)actor_id))) {
            continue;
        }
        if (actor->actor.transition_state != 0) {
            continue;
        }

        if (BattleContext_GetRuntimeState()->flags.bits.alternate_formation) {
            switch (actor->formation_index) {
            case 0:
                target_resource_id = (s32)0xC0000053;
                break;
            case 1:
                target_resource_id = (s32)0xC0000054;
                break;
            case 2:
                target_resource_id = (s32)0xC0000055;
                break;
            case 3:
                target_resource_id = (s32)0xC0000056;
                break;
            case 4:
                target_resource_id = (s32)0xC0000057;
                break;
            case 5:
                target_resource_id = (s32)0xC0000058;
                break;
            }
        } else {
            switch (actor->formation_index) {
            case 0:
                target_resource_id = (s32)0xC0000000;
                break;
            case 1:
                target_resource_id = (s32)0xC0000001;
                break;
            case 2:
                target_resource_id = (s32)0xC0000002;
                break;
            case 3:
                target_resource_id = (s32)0xC0000003;
                break;
            case 4:
                target_resource_id = (s32)0xC000000B;
                break;
            case 5:
                target_resource_id = (s32)0xC000000D;
                break;
            }
        }

        scene_object = actor->actor.scene_object;
        resource = scene_object->resource;
        linked_object = BattleSceneObject_GetById(actor->linked_object_id);
        if (resource->resource_id == target_resource_id) {
            continue;
        }

        BattleObjectData_AllocateLoadBuffer(
            BATTLE_PARTY_TRANSITION_OBJECT_DATA_ID, resource->allocation_size);
        BattleObjectData_ConfigureLoad(BATTLE_PARTY_TRANSITION_OBJECT_DATA_ID,
                                       BATTLE_PARTY_TRANSITION_RESOURCE_INDEX,
                                       0);
        BattleObjectData_CopyResource(
            resource, BattleObjectData_GetLoadState(
                          BATTLE_PARTY_TRANSITION_OBJECT_DATA_ID));
        BattleEntity_BindResource(scene_object->actor_id,
                                  BATTLE_PARTY_TRANSITION_OBJECT_DATA_ID);
        BattleObjectData_EnsureLoaded(resource->object_data_id,
                                      target_resource_id);
        scene_object->loaded_resource_id = 0;

        if (BattleContext_GetRuntimeState()->flags.bits.alternate_formation) {
            int delta_x = linked_object->x - scene_object->x + 16;
            int delta_y = linked_object->y - scene_object->y;
            int delta_y_squared = delta_y * delta_y;
            int duration;
            int animation_id;

            *rSQRTCNT = SQRTCNT_MODE_32;
            *rSQRT_PARAM_L = delta_x * delta_x / 10 + delta_y * delta_y / 3;
            while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
            }
            duration = *rSQRT_RESULT;

            BattleSceneObject_MoveBy(scene_object, 2, delta_x, delta_y, 0,
                                     duration);
            BattleSceneObject_StartAcceleratedMotionForDuration(
                scene_object, 1, 0, 0, 1, -scene_object->z, duration, 48, 1);
            animation_id = delta_y_squared < 64 ? 2 : 18;
            BattleSceneObject_SetBattleAnimation(scene_object, animation_id, 0);
        } else {
            BattleSceneObject_SetBattleAnimation(scene_object, 37, 0);
        }

        task = (BattlePartyFormationTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext + BATTLE_PARTY_TASK_POOL_OFFSET),
            0);
        {
            u16 *task_data = (u16 *)((u8 *)task + 0x0C);

            task_data[0] = next_state;
            task_data[1] = actor_id;
            task_data[2] = mario_target_x;
            task_data[3] = luigi_target_x;
        }
        task->callback =
            (void (*)(BattleAITask *))BattleParty_UpdateFormationTransition;
        return BATTLE_PARTY_TRANSITION_TASK_STATE;
    }
}

int BattleParty_UpdateFormationTransition(BattleAITask *task) {
    BattlePartyFormationTask *formation_task;
    BattlePartyActor *actor;
    u16 *task_data;
    u16 actor_id;
    int result;
    BattleSceneObject *scene_object;
    BattleSceneObject *linked_object;
    BattleModel *model;

    formation_task = (BattlePartyFormationTask *)task;
    task_data = &formation_task->next_state;
    actor_id = task_data[1];
    result = BattleObjectData_IsLoadPending(0);
    if (result != 0) {
        return result;
    }
    result = BattleObjectData_IsLoadPending(1);
    if (result != 0) {
        return result;
    }

    actor = (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
    scene_object = actor->actor.scene_object;
    linked_object = BattleSceneObject_GetById(actor->linked_object_id);
    model = scene_object->primary_model;
    result = model->animation_id;

    switch (result) {
    case 2:
    case 18:
        result = BattleSceneObject_IsAnimationActiveById(actor_id, 2);
        if (result != 0) {
            return result;
        }
        BattleParty_SpawnLaunchImpact(&actor->actor);
        BattleEntity_BindResource(actor_id,
                                  actor_id != BATTLE_ACTOR_PARTY_FIRST);
        BattleSceneObject_AdjustPosition(scene_object, -16, 0, 0);
        BattleSceneObject_SetBattleAnimation(scene_object, 36, 0);
        return BattleSceneObject_SetBattleAnimation(linked_object, -1, 0);

    case 19:
        result = BattleSceneObject_IsAnimationActiveById(actor_id, 2);
        if (result != 0) {
            return result;
        }
        BattleParty_SpawnLaunchImpact(&actor->actor);
        BattleSceneObject_SetBattleAnimation(scene_object, 0, 0);
        BattleObjectData_ResetAll();
        result = 0;
        *(u32 *)(gBattleContext + BATTLE_NEXT_STATE_OFFSET) = task_data[0];
        formation_task->callback = 0;
        return result;

    case 36:
        result = (u32)(model->flags << 29) >> 31;
        if (result == 0) {
            return result;
        }
        BattleSceneObject_SetBattleAnimation(scene_object, 0, 0);
        BattleObjectData_ResetAll();
        result = 0;
        *(u32 *)(gBattleContext + BATTLE_NEXT_STATE_OFFSET) = task_data[0];
        formation_task->callback = 0;
        return result;

    case 37:
        result = (u32)(model->flags << 29) >> 31;
        if (result == 0) {
            return result;
        }
        BattleEntity_BindResource(actor_id,
                                  actor_id != BATTLE_ACTOR_PARTY_FIRST);
        BattleSceneObject_SetBattleAnimation(scene_object, 19, 0);
        BattleSceneObject_SetBattleAnimation(linked_object, 14, 0);
        BattleSceneObject_AdjustPosition(
            linked_object, scene_object->x - linked_object->x,
            scene_object->y - linked_object->y, -linked_object->z);
        BattleSceneObject_AdjustPosition(scene_object, 16, 0, 0);

        {
            int target_x;
            int delta_x;
            int target_y;
            int delta_y;
            int duration;

            if (actor_id == BATTLE_ACTOR_PARTY_FIRST) {
                target_x = task_data[2];
            } else {
                target_x = task_data[3];
            }
            delta_x = target_x - scene_object->x;
            if (actor_id == BATTLE_ACTOR_PARTY_FIRST) {
                target_y = 112;
            } else {
                target_y = 152;
            }
            delta_y = target_y - scene_object->y;

            *rSQRTCNT = SQRTCNT_MODE_32;
            *rSQRT_PARAM_L = delta_x * delta_x / 10 + delta_y * delta_y / 3;
            while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
            }
            duration = *rSQRT_RESULT;
            BattleSceneObject_MoveBy(scene_object, 2, delta_x, delta_y, 0,
                                     duration);
            return BattleSceneObject_StartAcceleratedMotionForDuration(
                scene_object, 1, 0, 0, 1, -scene_object->z, duration, 48, 1);
        }
    }

    return result;
}
