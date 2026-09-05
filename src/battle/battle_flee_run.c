#include <game/battle_actor.h>
#include <game/battle_coin_loss.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_flee.h>
#include <game/battle_scene.h>
#include <game/save_data.h>

enum BattleFleeRunConstant {
    BATTLE_FLEE_INPUT_OFFSET = 0x104,
    BATTLE_FLEE_HELPER_OBJECT_ID = 40,
    BATTLE_FLEE_SAVE_COIN_COUNT_OFFSET = 0x488,
    BATTLE_FLEE_SAVE_ESCAPE_MODIFIER_OFFSET = 0x418,
    BATTLE_FLEE_INPUT_MARIO = 0x0001,
    BATTLE_FLEE_INPUT_LUIGI = 0x0002,
    BATTLE_FLEE_INPUT_BABY_MARIO = 0x0400,
    BATTLE_FLEE_INPUT_BABY_LUIGI = 0x0800,
    BATTLE_FLEE_INPUT_BOOST = 5,
    BATTLE_FLEE_INPUT_BOOST_MAX = 255,
    BATTLE_FLEE_SPEED_NORMAL_Q8 = 16,
    BATTLE_FLEE_SPEED_BOOSTED_Q8 = 320,
    BATTLE_FLEE_RUN_DELAY = 100,
    BATTLE_FLEE_COIN_INTERVAL_NORMAL = 34,
    BATTLE_FLEE_COIN_INTERVAL_REDUCED = 60,
    BATTLE_FLEE_REDUCED_COIN_LOSS_BADGE_ID = 0x3005,
    BATTLE_FLEE_RUN_INPUT_SOUND_ID = 61,
    BATTLE_FLEE_START_SOUND_ID = 151,
    BATTLE_FLEE_IMPACT_SOUND_ID = 62,
    BATTLE_FLEE_MARIO_VOICE_ID = 16388,
    BATTLE_FLEE_LUIGI_VOICE_ID = 16450,
    BATTLE_FLEE_BABY_MARIO_VOICE_ID = 16478,
    BATTLE_FLEE_BABY_LUIGI_VOICE_ID = 16499
};

extern int Random_NextModulo(int modulus);
extern void func_ov002_020a2728(int effect_id, int duration,
                                BattleSceneObject *object);

static inline int BattleFlee_HasReducedCoinLoss(int actor_id) {
    BattleActor *actor = BattleActor_GetById(actor_id);
    int member_id = *(u16 *)actor->resource_slot;

    return ((*(u8 *)(gSaveData + BATTLE_FLEE_SAVE_ESCAPE_MODIFIER_OFFSET +
                     sizeof(SavePartyMember) * (u8)member_id) |
             0x3000) == BATTLE_FLEE_REDUCED_COIN_LOSS_BADGE_ID);
}

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
void BattleFlee_BeginRun(BattleAITask *base_task) {
    BattleFleeTask *task = (BattleFleeTask *)base_task;
    BattleFleeState *state = &task->data;
    BattleSceneObject *object = BattleSceneObject_GetById(state->object_id);
    BattleSceneObject *helper;
    int object_y;
    int object_x;
    int helper_z;
    int helper_x;
    int helper_y;

    if (!object->primary_model->flag_bits.panel_animation_trigger) {
        return;
    }

    BattleSceneObject_SetAnimation(object, 0, -1);
    object->primary_model->flags &= ~BATTLE_MODEL_ANIMATION_MODE_MASK;
    object->primary_model->flags &= ~BATTLE_MODEL_FLAG_09;
    state->escape_delay =
        state->partner_task != 0 ? BATTLE_FLEE_RUN_DELAY : 0;
    state->coin_spawn_timer = 0;
    state->input_boost_timer = 0;
    state->x_fraction = 0;
    task->callback = BattleFlee_UpdateRun;

    switch (((BattlePartyActor *)BattleActor_GetPartySlot(object->actor_id))
                ->formation_index) {
        case BATTLE_FORMATION_MARIO:
        case BATTLE_FORMATION_MARIO_CARRYING:
            BattleSound_Play(BATTLE_FLEE_START_SOUND_ID, 0, 0, 0);
            break;
        case BATTLE_FORMATION_LUIGI:
        case BATTLE_FORMATION_LUIGI_CARRYING:
            BattleSound_Play(BATTLE_FLEE_START_SOUND_ID, 0, 0, 0);
            break;
        case BATTLE_FORMATION_BABY_MARIO:
            BattleSound_Play(BATTLE_FLEE_START_SOUND_ID, 0, 0, 0);
            break;
        case BATTLE_FORMATION_BABY_LUIGI:
            BattleSound_Play(BATTLE_FLEE_START_SOUND_ID, 0, 0, 0);
            break;
        default:
            break;
    }

    switch (((BattlePartyActor *)BattleActor_GetPartySlot(state->object_id))
                ->formation_index) {
        case BATTLE_FORMATION_MARIO:
            BattleSceneObject_SetAnimation(
                BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID),
                5, -1);
            break;
        case BATTLE_FORMATION_LUIGI:
            BattleSceneObject_SetAnimation(
                BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID),
                12, -1);
            break;
        case BATTLE_FORMATION_BABY_MARIO:
            BattleSceneObject_SetAnimation(
                BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID),
                19, -1);
            break;
        case BATTLE_FORMATION_BABY_LUIGI:
            BattleSceneObject_SetAnimation(
                BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID),
                26, -1);
            break;
        case BATTLE_FORMATION_MARIO_CARRYING:
            BattleSceneObject_SetAnimation(
                BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID),
                5, -1);
            break;
        case BATTLE_FORMATION_LUIGI_CARRYING:
            BattleSceneObject_SetAnimation(
                BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID),
                12, -1);
            break;
        default:
            break;
    }

    object_y = object->y;
    object_x = object->x;
    helper_z = object->z - 16;
    helper_y = object_y - 24;
    helper_x = object_x + 28;
    helper = BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID);
    BattleSceneObject_AdjustPosition(
        helper, helper_x - helper->x, helper_y - helper->y,
        helper_z - helper->z);
}

void BattleFlee_UpdateRun(BattleAITask *base_task) {
    BattleFleeTask *task = (BattleFleeTask *)base_task;
    BattleFleeState *state = &task->data;
    BattleSceneObject *object = BattleSceneObject_GetById(state->object_id);
    BattlePartyActor *actor =
        (BattlePartyActor *)BattleActor_GetPartySlot(state->object_id);
    int input_active = 0;
    int position_q8;
    int movement_q8;
    int object_y;
    int object_x;
    int helper_x;
    int helper_y;
    int helper_z;
    BattleSceneObject *helper;

    switch (actor->formation_index) {
        case BATTLE_FORMATION_MARIO:
            input_active =
                *(u16 *)(gBattleContext + BATTLE_FLEE_INPUT_OFFSET) &
                BATTLE_FLEE_INPUT_MARIO;
            break;
        case BATTLE_FORMATION_LUIGI:
            input_active =
                *(u16 *)(gBattleContext + BATTLE_FLEE_INPUT_OFFSET) &
                BATTLE_FLEE_INPUT_LUIGI;
            break;
        case BATTLE_FORMATION_BABY_MARIO:
            input_active =
                *(u16 *)(gBattleContext + BATTLE_FLEE_INPUT_OFFSET) &
                BATTLE_FLEE_INPUT_BABY_MARIO;
            break;
        case BATTLE_FORMATION_BABY_LUIGI:
            input_active =
                *(u16 *)(gBattleContext + BATTLE_FLEE_INPUT_OFFSET) &
                BATTLE_FLEE_INPUT_BABY_LUIGI;
            break;
        case BATTLE_FORMATION_MARIO_CARRYING:
            input_active =
                *(u16 *)(gBattleContext + BATTLE_FLEE_INPUT_OFFSET) &
                BATTLE_FLEE_INPUT_MARIO;
            break;
        case BATTLE_FORMATION_LUIGI_CARRYING:
            input_active =
                *(u16 *)(gBattleContext + BATTLE_FLEE_INPUT_OFFSET) &
                BATTLE_FLEE_INPUT_LUIGI;
            break;
        default:
            break;
    }
    if (input_active != 0) {
        int boost_timer;

        BattleSound_Play(BATTLE_FLEE_RUN_INPUT_SOUND_ID, 0, 0, 0);
        boost_timer = state->input_boost_timer + BATTLE_FLEE_INPUT_BOOST;
        if (boost_timer >= BATTLE_FLEE_INPUT_BOOST_MAX) {
            boost_timer = BATTLE_FLEE_INPUT_BOOST_MAX;
        }
        state->input_boost_timer = boost_timer;
    }

    object_y = object->y;
    object_x = object->x;
    helper_z = object->z - 16;
    helper_y = object_y - 24;
    helper_x = object_x + 28;
    helper = BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID);
    BattleSceneObject_AdjustPosition(
        helper, helper_x - helper->x, helper_y - helper->y,
        helper_z - helper->z);

    if (*(u32 *)(gSaveData + BATTLE_FLEE_SAVE_COIN_COUNT_OFFSET) != 0) {
        if (state->coin_spawn_timer != 0) {
            --state->coin_spawn_timer;
        } else {
            int interval = BATTLE_FLEE_COIN_INTERVAL_NORMAL;

            if (BattleFlee_HasReducedCoinLoss(state->object_id)) {
                interval = BATTLE_FLEE_COIN_INTERVAL_REDUCED;
            }
            state->coin_spawn_timer =
                interval * (Random_NextModulo(64) + 224) / 256;
            --*(u32 *)(gSaveData + BATTLE_FLEE_SAVE_COIN_COUNT_OFFSET);
            BattleCoinLoss_SpawnParticle(object);
        }
    }

    if (state->input_boost_timer != 0) {
        --state->input_boost_timer;
        movement_q8 = BATTLE_FLEE_SPEED_BOOSTED_Q8;
    } else {
        movement_q8 = BATTLE_FLEE_SPEED_NORMAL_Q8;
    }
    position_q8 =
        state->x_fraction + (object->x << 8) - movement_q8;
    object->x = position_q8 / 256;
    object->motion_target_x = object->x;
    state->x_fraction = (u8)position_q8;

    if (object->x < 8) {
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID), -1, -1);
        task->callback = BattleFlee_MoveActorOffscreen;
        return;
    }

    if (state->escape_delay != 0) {
        --state->escape_delay;
        if (state->escape_delay == 0) {
            int coin_count;
            int object_id;

            BattleScreenEffect_StartPrimary(6, 8, 12);
            BattleSceneObject_SetAnimation(
                BattleSceneObject_GetById(BATTLE_FLEE_HELPER_OBJECT_ID),
                -1, -1);
            object_id = state->object_id;
            coin_count = Random_NextModulo(4) + 2;
            if (BattleFlee_HasReducedCoinLoss(object_id)) {
                coin_count = 10 * coin_count / 100;
            }
            BattleCoinLoss_Start(object->actor_id, coin_count);
            BattleSceneObject_SetAnimation(object, 1, -1);

            switch (actor->formation_index) {
                case BATTLE_FORMATION_MARIO:
                    BattleSound_Play(BATTLE_FLEE_IMPACT_SOUND_ID, 12, 0, 0);
                    func_ov002_020a2728(16, 12, object);
                    break;
                case BATTLE_FORMATION_LUIGI:
                    BattleSound_Play(BATTLE_FLEE_IMPACT_SOUND_ID, 12, 0, 0);
                    func_ov002_020a2728(16, 12, object);
                    break;
                case BATTLE_FORMATION_BABY_MARIO:
                    BattleSound_Play(BATTLE_FLEE_IMPACT_SOUND_ID, 12, 0, 0);
                    func_ov002_020a2728(17, 12, object);
                    break;
                case BATTLE_FORMATION_BABY_LUIGI:
                    BattleSound_Play(BATTLE_FLEE_IMPACT_SOUND_ID, 12, 0, 0);
                    func_ov002_020a2728(17, 12, object);
                    break;
                case BATTLE_FORMATION_MARIO_CARRYING:
                    BattleSound_Play(BATTLE_FLEE_IMPACT_SOUND_ID, 12, 0, 0);
                    func_ov002_020a2728(16, 12, object);
                    break;
                case BATTLE_FORMATION_LUIGI_CARRYING:
                    BattleSound_Play(BATTLE_FLEE_IMPACT_SOUND_ID, 12, 0, 0);
                    func_ov002_020a2728(16, 12, object);
                    break;
                default:
                    break;
            }
            switch (actor->formation_index) {
                case BATTLE_FORMATION_MARIO:
                case BATTLE_FORMATION_MARIO_CARRYING:
                    BattleSound_Play(BATTLE_FLEE_MARIO_VOICE_ID, 0, 0, 0);
                    break;
                case BATTLE_FORMATION_LUIGI:
                case BATTLE_FORMATION_LUIGI_CARRYING:
                    BattleSound_Play(BATTLE_FLEE_LUIGI_VOICE_ID, 0, 0, 0);
                    break;
                case BATTLE_FORMATION_BABY_MARIO:
                    BattleSound_Play(
                        BATTLE_FLEE_BABY_MARIO_VOICE_ID, 0, 0, 0);
                    break;
                case BATTLE_FORMATION_BABY_LUIGI:
                    BattleSound_Play(
                        BATTLE_FLEE_BABY_LUIGI_VOICE_ID, 0, 0, 0);
                    break;
                default:
                    break;
            }
            object->primary_model->flags =
                (object->primary_model->flags &
                 ~BATTLE_MODEL_ANIMATION_MODE_MASK) |
                0x1000;
            state->flags.bits.ready_for_partner = 0;
            task->callback = BattleFlee_PreparePartnerExit;
        }
    }
}
