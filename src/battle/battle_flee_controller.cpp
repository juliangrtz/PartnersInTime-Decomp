#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_flee.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>

enum BattleFleeControllerConstant {
    BATTLE_FLEE_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_FLEE_INPUT_OFFSET = 0x104,
    BATTLE_FLEE_ACTIVE_OBJECT_ID_OFFSET = 0x20,
    BATTLE_FLEE_PARTICLE_OBJECT_FIRST = 41,
    BATTLE_FLEE_PARTICLE_OBJECT_COUNT = 3,
    BATTLE_FLEE_HELPER_OBJECT_ID = 40,
    BATTLE_FLEE_INDICATOR_OBJECT_ID = 44,
    BATTLE_FLEE_MARIO_OBJECT_ID = 56,
    BATTLE_FLEE_LUIGI_OBJECT_ID = 57,
    BATTLE_FLEE_MARIO_DEFAULT_RESOURCE_ID = 0,
    BATTLE_FLEE_LUIGI_DEFAULT_RESOURCE_ID = 1,
    BATTLE_FLEE_PARTICLE_RESOURCE_ID = 4,
    BATTLE_FLEE_MARIO_RUN_RESOURCE_ID = 5,
    BATTLE_FLEE_LUIGI_RUN_RESOURCE_ID = 6,
    BATTLE_FLEE_HELPER_RESOURCE_ID = 52,
    BATTLE_FLEE_INDICATOR_RESOURCE_ID = 53,
    BATTLE_FLEE_PARTICLE_ANIMATION = 25,
    BATTLE_FLEE_CANCEL_INPUT = 0x200,
    BATTLE_FLEE_PARTNER_LAUNCH_DELAY = 8,
    BATTLE_FLEE_MATRIX_ONE = 0x1000,
    BATTLE_FLEE_CANCEL_SOUND_ID = 3,
    BATTLE_FLEE_RUNTIME_WAIT_A_OFFSET = 0xDB4,
    BATTLE_FLEE_RUNTIME_WAIT_B_OFFSET = 0xDE4,
    BATTLE_FLEE_RUNTIME_WAIT_C_OFFSET = 0x4E4,
    BATTLE_FLEE_RUNTIME_WAIT_D_OFFSET = 0x514,
    BATTLE_FLEE_HELPER_RESOURCE_INDEX = 0x990,
    BATTLE_FLEE_INDICATOR_RESOURCE_INDEX = 0x980
};

enum BattleFleeResourceId {
    BATTLE_FLEE_MARIO_RESOURCE = (s32)0xC000003D,
    BATTLE_FLEE_LUIGI_RESOURCE = (s32)0xC000003E,
    BATTLE_FLEE_BABY_MARIO_RESOURCE = (s32)0xC000003F,
    BATTLE_FLEE_BABY_LUIGI_RESOURCE = (s32)0xC0000040,
    BATTLE_FLEE_MARIO_CARRYING_RESOURCE = (s32)0xC0000041,
    BATTLE_FLEE_LUIGI_CARRYING_RESOURCE = (s32)0xC0000042,
    BATTLE_FLEE_HELPER_RESOURCE = (s32)0xC200001E,
    BATTLE_FLEE_INDICATOR_RESOURCE = (s32)0xC200002B
};

typedef union BattleFleeRuntimeLoadFlags {
    u32 raw;
    struct {
        u32 unknown_00_29 : 30;
        u32 resource_busy : 1;
        u32 unknown_31 : 1;
    } bits;
} BattleFleeRuntimeLoadFlags;

static inline BattleTaskPool *BattleFlee_GetTaskPool(void) {
    return (BattleTaskPool *)(gBattleContext + BATTLE_FLEE_TASK_POOL_OFFSET);
}

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
extern "C" BattleFleeControllerTask *BattleFlee_Start(void) {
    BattleFleeControllerTask *task =
        (BattleFleeControllerTask *)BattleTaskList_Insert(
            BattleFlee_GetTaskPool(), 0);
    BattleFleeTask **actor_tasks = (BattleFleeTask **)task->actor_tasks;

    actor_tasks[0] = 0;
    actor_tasks[1] = 0;
    task->callback = BattleFlee_LoadResources;
    return task;
}

extern "C" void BattleFlee_LoadResources(BattleAITask *base_task) {
    BattleFleeControllerTask *task =
        (BattleFleeControllerTask *)base_task;
    int formation;

    BattleObjectData_ResetAll();
    BattleObjectData_GetLoadState(BATTLE_FLEE_MARIO_RUN_RESOURCE_ID)
        ->resource_id = 0;
    BattleObjectData_GetLoadState(BATTLE_FLEE_LUIGI_RUN_RESOURCE_ID)
        ->resource_id = 0;

    formation =
        ((BattlePartyActor *)BattleActor_GetPartySlot(
             BATTLE_FLEE_MARIO_OBJECT_ID))->formation_index;
    switch (formation) {
        case BATTLE_FORMATION_MARIO:
            BattleObjectData_EnsureLoaded(
                BATTLE_FLEE_MARIO_RUN_RESOURCE_ID,
                BATTLE_FLEE_MARIO_RESOURCE);
            break;
        case BATTLE_FORMATION_BABY_MARIO:
            BattleObjectData_EnsureLoaded(
                BATTLE_FLEE_MARIO_RUN_RESOURCE_ID,
                BATTLE_FLEE_BABY_MARIO_RESOURCE);
            break;
        case BATTLE_FORMATION_MARIO_CARRYING:
            BattleObjectData_EnsureLoaded(
                BATTLE_FLEE_MARIO_RUN_RESOURCE_ID,
                BATTLE_FLEE_MARIO_CARRYING_RESOURCE);
            break;
    }

    formation =
        ((BattlePartyActor *)BattleActor_GetPartySlot(
             BATTLE_FLEE_LUIGI_OBJECT_ID))->formation_index;
    switch (formation) {
        case BATTLE_FORMATION_LUIGI:
            BattleObjectData_EnsureLoaded(
                BATTLE_FLEE_LUIGI_RUN_RESOURCE_ID,
                BATTLE_FLEE_LUIGI_RESOURCE);
            break;
        case BATTLE_FORMATION_BABY_LUIGI:
            BattleObjectData_EnsureLoaded(
                BATTLE_FLEE_LUIGI_RUN_RESOURCE_ID,
                BATTLE_FLEE_BABY_LUIGI_RESOURCE);
            break;
        case BATTLE_FORMATION_LUIGI_CARRYING:
            BattleObjectData_EnsureLoaded(
                BATTLE_FLEE_LUIGI_RUN_RESOURCE_ID,
                BATTLE_FLEE_LUIGI_CARRYING_RESOURCE);
            break;
    }

    BattleObjectData_AllocateLoadBuffer(BATTLE_FLEE_HELPER_RESOURCE_ID,
                                        0x3000);
    BattleObjectData_ConfigureLoad(BATTLE_FLEE_HELPER_RESOURCE_ID,
                                   BATTLE_FLEE_HELPER_RESOURCE_INDEX, 0);
    BattleObjectData_EnsureLoaded(BATTLE_FLEE_HELPER_RESOURCE_ID,
                                  BATTLE_FLEE_HELPER_RESOURCE);
    BattleObjectData_AllocateLoadBuffer(BATTLE_FLEE_INDICATOR_RESOURCE_ID,
                                        0x1000);
    BattleObjectData_ConfigureLoad(BATTLE_FLEE_INDICATOR_RESOURCE_ID,
                                   BATTLE_FLEE_INDICATOR_RESOURCE_INDEX, 0);
    BattleObjectData_EnsureLoaded(BATTLE_FLEE_INDICATOR_RESOURCE_ID,
                                  BATTLE_FLEE_INDICATOR_RESOURCE);
    task->callback = BattleFlee_InitializeScene;
}

extern "C" void BattleFlee_InitializeScene(BattleAITask *base_task) {
    BattleFleeControllerTask *task =
        (BattleFleeControllerTask *)base_task;
    BattleAITask **owner_slots;
    BattleSceneObject *indicator;
    int index;

    owner_slots = task->actor_tasks;

    if (((BattleFleeRuntimeLoadFlags *)(
             gBattleContext + BATTLE_RUNTIME_STATE_OFFSET +
             BATTLE_FLEE_RUNTIME_WAIT_A_OFFSET))->bits.resource_busy ||
        ((BattleFleeRuntimeLoadFlags *)(
             gBattleContext + BATTLE_RUNTIME_STATE_OFFSET +
             BATTLE_FLEE_RUNTIME_WAIT_B_OFFSET))->bits.resource_busy ||
        ((BattleFleeRuntimeLoadFlags *)(
             gBattleContext + BATTLE_RUNTIME_STATE_OFFSET +
             BATTLE_FLEE_RUNTIME_WAIT_C_OFFSET))->bits.resource_busy ||
        ((BattleFleeRuntimeLoadFlags *)(
             gBattleContext + BATTLE_RUNTIME_STATE_OFFSET +
             BATTLE_FLEE_RUNTIME_WAIT_D_OFFSET))->bits.resource_busy) {
        return;
    }

    BattleEntity_BindResource(BATTLE_FLEE_HELPER_OBJECT_ID,
                              BATTLE_FLEE_HELPER_RESOURCE_ID);
    BattleSceneObject_SetAnimation(
        BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID), -1, -1);
    BattleEntity_BindResource(BATTLE_FLEE_INDICATOR_OBJECT_ID,
                              BATTLE_FLEE_INDICATOR_RESOURCE_ID);
    BattleSceneObject_SetAnimation(
        BattleSceneObject_GetById(BATTLE_FLEE_INDICATOR_OBJECT_ID), -1, -1);
    indicator = BattleSceneObject_GetById(BATTLE_FLEE_INDICATOR_OBJECT_ID);
    BattleSceneObject_AdjustPosition(
        indicator, 32 - indicator->x, 300 - indicator->y,
        276 - indicator->z);
    indicator = BattleSceneObject_GetById(BATTLE_FLEE_INDICATOR_OBJECT_ID);
    indicator->flags.bits.use_raw_position = 1;

    for (index = 0; index < BATTLE_FLEE_PARTICLE_OBJECT_COUNT; index++) {
        BattleModel *model;
        int frame_span;

        BattleEntity_BindResource(
            (u16)(index + BATTLE_FLEE_PARTICLE_OBJECT_FIRST),
            BATTLE_FLEE_PARTICLE_RESOURCE_ID);
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(
                (u16)((unsigned int)index +
                      BATTLE_FLEE_PARTICLE_OBJECT_FIRST)),
            BATTLE_FLEE_PARTICLE_ANIMATION, -1);
        /* The enum-typed conversion keeps MWCC from merging the three
           object-ID calculations and reproduces the original register use. */
        model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(
                (u16)((BattleFleeControllerConstant)index +
                      BATTLE_FLEE_PARTICLE_OBJECT_FIRST)));
        model->stop();
        frame_span = model->animation_data->end_frame -
                     model->animation_data->start_frame;
        model->set_animation(
            BATTLE_FLEE_PARTICLE_ANIMATION,
            (s16)(index * frame_span / BATTLE_FLEE_PARTICLE_OBJECT_COUNT));
    }

    BattleFleeTask *first_task = (BattleFleeTask *)BattleTaskList_Insert(
        BattleFlee_GetTaskPool(), 0);
    BattleFleeState *first_state = &first_task->data;
    u16 first_object_id =
        *(u16 *)(gBattleContext + BATTLE_FLEE_ACTIVE_OBJECT_ID_OFFSET);
    u16 partner_object_id =
        first_object_id == BATTLE_FLEE_MARIO_OBJECT_ID
            ? BATTLE_FLEE_LUIGI_OBJECT_ID
            : BATTLE_FLEE_MARIO_OBJECT_ID;

    if (BattleActor_CanReceiveStatus(
            BattleActor_GetById(partner_object_id))) {
        BattleFleeTask *partner_task =
            (BattleFleeTask *)BattleTaskList_Insert(
            BattleFlee_GetTaskPool(), 0);
        BattleFleeState *partner_state = &partner_task->data;
        first_state->partner_task = partner_task;
        first_state->object_id = first_object_id;
        first_state->flags.bits.launch_delay =
            BATTLE_FLEE_PARTNER_LAUNCH_DELAY;
        first_state->flags.bits.ready_for_partner = 1;
        partner_state->partner_task = first_task;
        partner_state->object_id = partner_object_id;
        partner_state->flags.bits.launch_delay = 0;
        partner_state->flags.bits.ready_for_partner = 1;
        first_task->callback = BattleFlee_LaunchActor;
        partner_task->callback = BattleFlee_WaitToLaunchPartner;
        BattleTask_BindOwnerSlot(
            (BattleAITask *)first_task,
            owner_slots);
        BattleTask_BindOwnerSlot(
            (BattleAITask *)partner_task,
            owner_slots + 1);
    } else {
        first_state->partner_task = 0;
        first_state->object_id = first_object_id;
        first_state->flags.bits.launch_delay =
            BATTLE_FLEE_PARTNER_LAUNCH_DELAY;
        first_state->flags.bits.ready_for_partner = 1;
        first_task->callback = BattleFlee_LaunchActor;
        BattleTask_BindOwnerSlot(
            (BattleAITask *)first_task,
            owner_slots);
    }
    task->callback = BattleFlee_UpdateController;
}

extern "C" void BattleFlee_UpdateController(BattleAITask *base_task) {
    BattleFleeControllerTask *task =
        (BattleFleeControllerTask *)base_task;
    BattleFleeTask **actor_tasks = (BattleFleeTask **)task->actor_tasks;
    BattleSceneObject *indicator;
    int particle_index;

    for (particle_index = 0;
         particle_index < BATTLE_FLEE_PARTICLE_OBJECT_COUNT;
         particle_index++) {
        BattleModel *model = BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(
                (u16)(particle_index + BATTLE_FLEE_PARTICLE_OBJECT_FIRST)));
        BattleSpriteTransform *transform = model->get_sprite_transform();

        transform->matrix[0] = BATTLE_FLEE_MATRIX_ONE;
        transform->matrix[1] = 0;
        transform->matrix[2] = 0;
        transform->matrix[4] = 0;
        transform->matrix[5] = BATTLE_FLEE_MATRIX_ONE;
        transform->matrix[6] = 0;
        transform->matrix[8] = 0;
        transform->matrix[9] = 0;
        transform->matrix[10] = BATTLE_FLEE_MATRIX_ONE;
        model->prepare_render();
    }

    indicator = BattleSceneObject_GetById(BATTLE_FLEE_INDICATOR_OBJECT_ID);
    if (!(BattleActor_CanReceiveStatus(
              BattleActor_GetById(BATTLE_FLEE_MARIO_OBJECT_ID)) &&
          BattleActor_CanReceiveStatus(
              BattleActor_GetById(BATTLE_FLEE_LUIGI_OBJECT_ID)) &&
          (actor_tasks[0] == 0 || actor_tasks[1] == 0))) {
        if (indicator->primary_model == 0 ||
            !indicator->primary_model->flag_bits.animation_active) {
            BattleSceneObject_SetAnimation(indicator, 0, -1);
        }
        if ((*(u16 *)(gBattleContext + BATTLE_FLEE_INPUT_OFFSET) &
             BATTLE_FLEE_CANCEL_INPUT) != 0) {
            int actor_index;

            for (actor_index = 0; actor_index < 2; actor_index++) {
                BattleFleeTask *actor_task = actor_tasks[actor_index];

                if (actor_task != 0) {
                    int object_id = actor_task->data.object_id;

                    if (object_id == BATTLE_FLEE_MARIO_OBJECT_ID) {
                        BattleEntity_BindResource(
                            BATTLE_FLEE_MARIO_OBJECT_ID,
                            BATTLE_FLEE_MARIO_DEFAULT_RESOURCE_ID);
                        BattleSceneObject_SetAnimation(
                            BattleSceneObject_GetById(
                                BATTLE_FLEE_MARIO_OBJECT_ID),
                            0, -1);
                        BattleSceneObject_StopMotionChannel(
                            BattleSceneObject_GetById(
                                BATTLE_FLEE_MARIO_OBJECT_ID),
                            1, 0);
                    } else {
                        BattleEntity_BindResource(
                            BATTLE_FLEE_LUIGI_OBJECT_ID,
                            BATTLE_FLEE_LUIGI_DEFAULT_RESOURCE_ID);
                        BattleSceneObject_SetAnimation(
                            BattleSceneObject_GetById(
                                BATTLE_FLEE_LUIGI_OBJECT_ID),
                            0, -1);
                        BattleSceneObject_StopMotionChannel(
                            BattleSceneObject_GetById(
                                BATTLE_FLEE_LUIGI_OBJECT_ID),
                            1, 0);
                    }
                    actor_task->callback = 0;
                }
            }
            *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |=
                1 << 22;
            BattleSceneObject_SetAnimation(
                BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID),
                -1, -1);
            BattleSceneObject_SetAnimation(indicator, -1, -1);
            task->callback = BattleFlee_UpdateSceneTransition;
            BattleSound_Play(BATTLE_FLEE_CANCEL_SOUND_ID, 0, 0, 0);
            return;
        }
    } else if (indicator->primary_model != 0 &&
               indicator->primary_model->flag_bits.animation_active) {
        BattleSceneObject_SetAnimation(indicator, -1, -1);
    }

    if (actor_tasks[0] == 0 && actor_tasks[1] == 0) {
        BattleSceneObject_SetAnimation(indicator, -1, -1);
        task->callback = BattleFlee_CommitBattleExit;
    }
}
