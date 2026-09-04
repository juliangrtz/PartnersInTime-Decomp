#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>

enum BattlePartyCommandMovementOffset {
    BATTLE_STATE_OFFSET = 0x18,
    BATTLE_ACTIVE_ACTOR_ID_OFFSET = 0x20,
    BATTLE_INPUT_PRESSED_OFFSET = 0x104,
    BATTLE_PARTY_LANDING_STATE_OFFSET = 0x128,
    BATTLE_COMMAND_MODE_A_OFFSET = 0x65EE,
    BATTLE_COMMAND_MODE_B_OFFSET = 0x65F0
};

enum BattlePartyCommandInput {
    BATTLE_INPUT_A = 0x001,
    BATTLE_INPUT_B = 0x002,
    BATTLE_INPUT_X = 0x400,
    BATTLE_INPUT_Y = 0x800
};

enum BattlePartyFormationIndex {
    BATTLE_FORMATION_MARIO = 0,
    BATTLE_FORMATION_LUIGI = 1,
    BATTLE_FORMATION_BABY_MARIO = 2,
    BATTLE_FORMATION_BABY_LUIGI = 3,
    BATTLE_FORMATION_MARIO_CARRYING = 4,
    BATTLE_FORMATION_LUIGI_CARRYING = 5
};

enum BattlePartyCommandAnimation {
    BATTLE_ANIMATION_IDLE = 0,
    BATTLE_ANIMATION_ALTERNATE_IDLE = 1,
    BATTLE_ANIMATION_JUMP = 2,
    BATTLE_ANIMATION_PREPARE_BABY = 39,
    BATTLE_ANIMATION_DETACHED_BABY = 40,
    BATTLE_ANIMATION_LAUNCH_BABY = 41,
    BATTLE_ANIMATION_FINISH_BABY = 42
};

enum BattlePartyCarriedBabyPhase {
    BATTLE_CARRIED_BABY_NONE = 0,
    BATTLE_CARRIED_BABY_PREPARING = 1,
    BATTLE_CARRIED_BABY_LAUNCHING = 2,
    BATTLE_CARRIED_BABY_FINISHING = 3
};

enum BattlePartyCommandConstant {
    BATTLE_COMMAND_SELECTION_STATE = 0x3003,
    BATTLE_MARIO_CARRY_RESOURCE_ID = (int)0xC000000B,
    BATTLE_LUIGI_CARRY_RESOURCE_ID = (int)0xC000000D
};

extern void func_ov002_020a404c(BattleSceneObject *object, int channel_index,
                                int argument_2, int argument_3, int argument_4,
                                int initial_z, int peak_z, int speed,
                                int enabled);
extern void func_ov002_020a40d4(BattleSceneObject *object, int channel_index,
                                int argument_2, int argument_3, int argument_4,
                                int initial_z, int target_z, int speed,
                                int enabled);
extern int BattleParty_SpawnLaunchImpact(BattleActor *actor);

static inline int
BattlePartyActor_SelectIdleAnimation(BattlePartyActor *actor) {
    int command_state_available = 1;
    int idle_available = 1;

    if (actor->actor.scene_object->actor_id ==
        *(u16 *)(gBattleContext + BATTLE_ACTIVE_ACTOR_ID_OFFSET)) {
        command_state_available =
            *(u16 *)(gBattleContext + BATTLE_COMMAND_MODE_B_OFFSET) != 0;
    }
    if (!command_state_available &&
        *(u16 *)(gBattleContext + BATTLE_COMMAND_MODE_A_OFFSET) == 0) {
        idle_available = 0;
    }
    if (!idle_available && *(u32 *)(gBattleContext + BATTLE_STATE_OFFSET) !=
                               BATTLE_COMMAND_SELECTION_STATE) {
        return BATTLE_ANIMATION_ALTERNATE_IDLE;
    }
    return BATTLE_ANIMATION_IDLE;
}

static inline s32
BattlePartyActor_GetCarryResourceId(const BattlePartyActor *actor) {
    if (actor->formation_index == BATTLE_FORMATION_MARIO_CARRYING) {
        return BATTLE_MARIO_CARRY_RESOURCE_ID;
    }
    if (actor->formation_index == BATTLE_FORMATION_LUIGI_CARRYING) {
        return BATTLE_LUIGI_CARRY_RESOURCE_ID;
    }
    return -1;
}

u32 BattlePartyActor_UpdateCommandMovement(BattlePartyActor *actor) {
    BattleSceneObject *object;
    BattleModel *model;
    u16 input_mask;

    if (actor->actor.current_hp <= 0) {
        actor->state_flags.command_bits.airborne = 0;
        actor->state_flags.command_bits.jump_input_enabled = 0;
    }

    object = actor->actor.scene_object;
    model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(object->actor_id));

    if (actor->state_flags.command_bits.airborne) {
        if (object->z != 0) {
            if (object->actor_id ==
                    *(u16 *)(gBattleContext + BATTLE_ACTIVE_ACTOR_ID_OFFSET) &&
                object->z > 16) {
                BattleSceneObject_AdjustPosition(object, 0, 0, 16 - object->z);
                func_ov002_020a40d4(object, 3, 0, 0, 1, -object->z, 0, -256, 1);
                *(u16 *)(gBattleContext + BATTLE_PARTY_LANDING_STATE_OFFSET) =
                    1;
            }
        } else if (actor->state_flags.command_bits.carried_baby_phase ==
                   BATTLE_CARRIED_BABY_LAUNCHING) {
            int baby_actor_id =
                actor->formation_index == BATTLE_FORMATION_MARIO_CARRYING
                    ? BATTLE_ACTOR_BABY_MARIO
                    : BATTLE_ACTOR_BABY_LUIGI;
            BattleSceneObject *baby = BattleSceneObject_GetById(baby_actor_id);

            BattleSceneObject_GetActiveModel(baby);
            BattleSceneObject_SetAnimation(baby, -1, 0);
            BattleSceneObject_SetAnimation(object, BATTLE_ANIMATION_FINISH_BABY,
                                           0);
            BattleSceneObject_GetById(baby_actor_id)
                ->previous_base_animation_id = 0;
            actor->state_flags.command_bits.airborne = 0;
            actor->state_flags.command_bits.carried_baby_phase =
                BATTLE_CARRIED_BABY_FINISHING;
        } else if (actor->state_flags.command_bits.carried_baby_phase !=
                   BATTLE_CARRIED_BABY_PREPARING) {
            BattleSceneObject_SetAnimation(
                object, BattlePartyActor_SelectIdleAnimation(actor), 0);
            BattleParty_SpawnLaunchImpact(&actor->actor);
            actor->state_flags.command_bits.airborne = 0;
        }
    } else if (actor->state_flags.command_bits.jump_input_enabled &&
               actor->actor.transition_state == 0) {
        switch (actor->formation_index) {
        case BATTLE_FORMATION_BABY_MARIO:
            input_mask = BATTLE_INPUT_X;
            break;
        case BATTLE_FORMATION_BABY_LUIGI:
            input_mask = BATTLE_INPUT_Y;
            break;
        case BATTLE_FORMATION_MARIO_CARRYING:
            input_mask = BATTLE_INPUT_A;
            break;
        case BATTLE_FORMATION_LUIGI:
        case BATTLE_FORMATION_LUIGI_CARRYING:
            input_mask = BATTLE_INPUT_B;
            break;
        case BATTLE_FORMATION_MARIO:
        default:
            input_mask = BATTLE_INPUT_A;
            break;
        }

        if (*(u16 *)(gBattleContext + BATTLE_INPUT_PRESSED_OFFSET) &
            input_mask) {
            BattleSceneObject_SetAnimation(object, BATTLE_ANIMATION_JUMP, 0);
            BattleSceneObject_SetStateFlags(object, 1, 0);
            func_ov002_020a404c(object, 3, 0, 0, 1, 0, 24, 1536, 1);
            actor->state_flags.command_bits.airborne = 1;

            switch (actor->formation_index) {
            case BATTLE_FORMATION_MARIO:
            case BATTLE_FORMATION_LUIGI:
            case BATTLE_FORMATION_MARIO_CARRYING:
            case BATTLE_FORMATION_LUIGI_CARRYING:
                BattleSound_Play(38, 0, 0, 0);
                break;
            case BATTLE_FORMATION_BABY_MARIO:
            case BATTLE_FORMATION_BABY_LUIGI:
                BattleSound_Play(41, 0, 0, 0);
                break;
            }
        } else {
            u16 input = *(u16 *)(gBattleContext + BATTLE_INPUT_PRESSED_OFFSET);
            int prepare_carried_baby =
                ((input & BATTLE_INPUT_X) &&
                 actor->formation_index == BATTLE_FORMATION_MARIO_CARRYING) ||
                ((input & BATTLE_INPUT_Y) &&
                 actor->formation_index == BATTLE_FORMATION_LUIGI_CARRYING);

            if (prepare_carried_baby) {
                BattleSceneObject_SetAnimation(
                    object, BATTLE_ANIMATION_PREPARE_BABY, 0);
                BattleSceneObject_SetStateFlags(object, 1, 0);
                actor->state_flags.command_bits.carried_baby_phase =
                    BATTLE_CARRIED_BABY_PREPARING;
                actor->state_flags.command_bits.airborne = 1;
            }
        }
    }

    if (actor->state_flags.command_bits.carried_baby_phase ==
        BATTLE_CARRIED_BABY_PREPARING) {
        s32 expected_resource_id = BattlePartyActor_GetCarryResourceId(actor);

        if (model != 0 &&
            object->resource->resource_id == expected_resource_id &&
            model->animation_id == BATTLE_ANIMATION_PREPARE_BABY) {
            int baby_actor_id;
            int object_data_id;
            int animation_duration;
            BattleSceneObject *baby;

            if (actor->formation_index == BATTLE_FORMATION_MARIO_CARRYING) {
                baby_actor_id = BATTLE_ACTOR_BABY_MARIO;
                object_data_id = 0;
                animation_duration = 256;
            } else {
                baby_actor_id = BATTLE_ACTOR_BABY_LUIGI;
                object_data_id = 1;
                animation_duration = 384;
            }

            baby = BattleSceneObject_GetById(baby_actor_id);
            BattleObjectData_GetLoadState(object_data_id);
            BattleSceneObject_AdjustPosition(baby, object->x - baby->x,
                                             object->y - baby->y,
                                             object->z - baby->z);
            BattleEntity_BindResource(baby_actor_id, object_data_id);
            BattleSceneObject_SetAnimationFromComponent(
                baby, BATTLE_ANIMATION_DETACHED_BABY, 0, animation_duration);
            BattleSceneObject_SetStateFlags(baby, 1, 0);
            baby->previous_base_animation_id = 8;
            BattleSceneObject_SetAnimation(object, BATTLE_ANIMATION_LAUNCH_BABY,
                                           0);
            BattleSceneObject_SetStateFlags(object, 1, 0);
            func_ov002_020a404c(object, 3, 0, 0, 1, 0, 24, 1536, 1);
            actor->state_flags.command_bits.carried_baby_phase =
                BATTLE_CARRIED_BABY_LAUNCHING;
            BattleSound_Play(41, 0, 0, 0);
        } else {
            actor->state_flags.command_bits.carried_baby_phase =
                BATTLE_CARRIED_BABY_NONE;
        }
    }

    if (actor->state_flags.command_bits.carried_baby_phase ==
        BATTLE_CARRIED_BABY_FINISHING) {
        s32 expected_resource_id = BattlePartyActor_GetCarryResourceId(actor);

        if (model != 0 &&
            object->resource->resource_id == expected_resource_id &&
            model->animation_id == BATTLE_ANIMATION_FINISH_BABY) {
            if (model->flags & BATTLE_MODEL_FLAG_02) {
                BattleSceneObject_SetAnimation(
                    object, BattlePartyActor_SelectIdleAnimation(actor), 0);
                actor->state_flags.command_bits.carried_baby_phase =
                    BATTLE_CARRIED_BABY_NONE;
            }
        } else {
            actor->state_flags.command_bits.carried_baby_phase =
                BATTLE_CARRIED_BABY_NONE;
        }
    } else if (actor->state_flags.command_bits.carried_baby_phase !=
               BATTLE_CARRIED_BABY_PREPARING) {
        actor->state_flags.command_bits.carried_baby_phase =
            BATTLE_CARRIED_BABY_NONE;
    }

    return actor->state_flags.raw;
}
