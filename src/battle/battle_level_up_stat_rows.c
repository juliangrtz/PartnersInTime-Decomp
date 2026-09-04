#include <game/battle_ai.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>
#include <game/save_data.h>

enum BattleLevelUpStatRowOffset {
    BATTLE_LEVEL_UP_MEMBER_ID_OFFSET = 0xDA,
    BATTLE_LEVEL_UP_GROWTH_ACTIVE_OFFSET = 0xE4,
    BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET = 0xE6,
    BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET = 0xF0,
    BATTLE_LEVEL_UP_SCREEN_X_OFFSET = 0xFA,
    BATTLE_LEVEL_UP_SCREEN_TARGET_X_OFFSET = 0xFC,
    BATTLE_LEVEL_UP_SCREEN_TARGET_Y_OFFSET = 0xFE,
    BATTLE_LEVEL_UP_SCREEN_STATE_OFFSET = 0x100,
    BATTLE_LEVEL_UP_PRIMARY_OBJECT_OFFSET = 0x6784,
    BATTLE_LEVEL_UP_SECONDARY_OBJECT_OFFSET = 0x6788,
    BATTLE_LEVEL_UP_TERTIARY_OBJECT_OFFSET = 0x678C,
    BATTLE_LEVEL_UP_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_LEVEL_UP_VIEW_X_OFFSET = 0xCB9C,
    BATTLE_LEVEL_UP_VIEW_Y_OFFSET = 0xCB9E,
    SAVE_PARTY_MEMBERS_OFFSET = 0x3F8,
    SAVE_PARTY_MEMBER_SIZE = 0x24
};

typedef struct BattleLevelUpStatRowState {
    u8 phase;
    s8 intensity;
    u8 stat_index;
    u8 unknown_03;
    s16 x;
} BattleLevelUpStatRowState;

typedef struct BattleLevelUpScreenState {
    s16 x;
    s16 y;
    s16 z;
    u8 unknown_06[4];
    u8 flags;
} BattleLevelUpScreenState;

extern BattleSpriteTransform gBattleLevelUpStatRowTransform;
void BattleLevelUpScreen_UpdateController(BattleAITask *task);
void BattleLevelUpScreen_UpdateEntrance(BattleAITask *task);

void BattleLevelUpGrowth_UpdateStatRow(BattleAITask *base_task) {
    BattleLevelUpStatRowState *state =
        (BattleLevelUpStatRowState *)&base_task->state;
    int intensity = state->intensity;

    if (*(s16 *)(gBattleContext + BATTLE_LEVEL_UP_GROWTH_ACTIVE_OFFSET) == 0) {
        intensity -= 2;
    } else {
        intensity += 4;
    }
    if (intensity < 1) {
        base_task->callback = 0;
        return;
    }
    if (intensity > 31) {
        intensity = 31;
    }
    state->intensity = intensity;

    if (state->phase == 1) {
        int delta = 5504 - state->x;

        if (delta < -2048) {
            state->x -= 256;
        } else if (delta < -16) {
            state->x += delta / 8;
        } else if (delta < 0) {
            state->x -= 2;
        } else {
            state->x = 5504;
        }
        if (state->x == 5504) {
            state->phase = 2;
        }
    }

    {
        s16 inverted_y = (s16)(252 - (16 * state->stat_index + 88));
        s16 stat_x = (s16)(state->x / 32);
        s16 stat_y = (s16)(252 - inverted_y);
        s16 x = (s16)(stat_x - *(s16 *)(
            gBattleContext + BATTLE_LEVEL_UP_VIEW_X_OFFSET));
        s16 y = (s16)(stat_y - *(s16 *)(
            gBattleContext + BATTLE_LEVEL_UP_VIEW_Y_OFFSET));
        int stat_value;
        int growth_value;
        int display_value;

        gBattleLevelUpStatRowTransform.x = x << 8;
        gBattleLevelUpStatRowTransform.y = y << 8;
        gBattleLevelUpStatRowTransform.z = 64;
        BattleSprite_DrawFrame(
            (*(s16 *)(gBattleContext + BATTLE_LEVEL_UP_MEMBER_ID_OFFSET) & 1)
                    != 0
                ? 8
                : 7,
            intensity, &gBattleLevelUpStatRowTransform, 0, 0, 0x35,
            0x7FFF);

        gBattleLevelUpStatRowTransform.x = (x - 40) << 8;
        gBattleLevelUpStatRowTransform.y = (y + 2) << 8;
        BattleSprite_DrawFrame(state->stat_index + 2, intensity,
                               &gBattleLevelUpStatRowTransform, 0, 0, 0x37,
                               0x7FFF);

        stat_value = 0;
        growth_value = 0;
        display_value = 0;
        {
            SavePartyMember *member =
                (SavePartyMember *)(
                    gSaveData + SAVE_PARTY_MEMBERS_OFFSET +
                    SAVE_PARTY_MEMBER_SIZE * *(s16 *)(
                        gBattleContext + BATTLE_LEVEL_UP_MEMBER_ID_OFFSET));

            switch (state->stat_index) {
            case 0:
                stat_value = member->base_max_hp;
                display_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 0);
                growth_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 0);
                break;
            case 1:
                stat_value = member->base_power;
                display_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 2);
                growth_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 2);
                break;
            case 2:
                stat_value = member->base_defense;
                display_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 4);
                growth_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 4);
                break;
            case 3:
                stat_value = member->base_speed;
                display_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 6);
                growth_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 6);
                break;
            case 4:
                stat_value = member->base_stache;
                display_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_DISPLAY_VALUES_OFFSET + 8);
                growth_value = *(s16 *)(
                    gBattleContext +
                    BATTLE_LEVEL_UP_GROWTH_VALUES_OFFSET + 8);
                break;
            }
        }

        gBattleLevelUpStatRowTransform.x = (x + 16) << 8;
        gBattleLevelUpStatRowTransform.y = (y + 2) << 8;
        BattleNumber_DrawDecimal(stat_value, intensity,
                                 &gBattleLevelUpStatRowTransform, 0, 0x7FFF,
                                 0, 0);
        if (growth_value >= 0) {
            gBattleLevelUpStatRowTransform.x = (x + 32) << 8;
            BattleSprite_DrawFrame(18, intensity,
                                   &gBattleLevelUpStatRowTransform, 0, 0, 3,
                                   0x7FFF);
            gBattleLevelUpStatRowTransform.x = (x + 48) << 8;
            BattleNumber_DrawDecimal(display_value, intensity,
                                     &gBattleLevelUpStatRowTransform, 0,
                                     0x7FFF, 0, 0);
        }
    }
}

BattleAITask *BattleLevelUpScreen_Start(void) {
    BattleAITask *controller = BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext + BATTLE_LEVEL_UP_TASK_POOL_OFFSET),
        0);
    BattleAITask *entrance;
    BattleLevelUpScreenState *entrance_state;
    BattleSceneObject *object;

    controller->callback = BattleLevelUpScreen_UpdateController;
    entrance = BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext + BATTLE_LEVEL_UP_TASK_POOL_OFFSET),
        0);
    entrance->callback = BattleLevelUpScreen_UpdateEntrance;
    entrance_state = (BattleLevelUpScreenState *)&entrance->state;

    object = *(BattleSceneObject **)(
        gBattleContext + BATTLE_LEVEL_UP_PRIMARY_OBJECT_OFFSET);
    BattleEntity_BindResource(object->actor_id, 54);
    object = *(BattleSceneObject **)(
        gBattleContext + BATTLE_LEVEL_UP_PRIMARY_OBJECT_OFFSET);
    BattleSceneObject_SetAnimation(object, 2, -1);
    (*(BattleSceneObject **)(
         gBattleContext + BATTLE_LEVEL_UP_PRIMARY_OBJECT_OFFSET))
        ->primary_model->animation_state_bits.state = 31;
    *(u16 *)&(*(BattleSceneObject **)(
                  gBattleContext + BATTLE_LEVEL_UP_PRIMARY_OBJECT_OFFSET))
                  ->unk_0f0 = 0x7FFF;

    object = *(BattleSceneObject **)(
        gBattleContext + BATTLE_LEVEL_UP_SECONDARY_OBJECT_OFFSET);
    BattleEntity_BindResource(object->actor_id, 54);
    object = *(BattleSceneObject **)(
        gBattleContext + BATTLE_LEVEL_UP_SECONDARY_OBJECT_OFFSET);
    BattleSceneObject_SetAnimation(object, 3, -1);
    (*(BattleSceneObject **)(
         gBattleContext + BATTLE_LEVEL_UP_SECONDARY_OBJECT_OFFSET))
        ->primary_model->animation_state_bits.state = 31;
    *(u16 *)&(*(BattleSceneObject **)(
                  gBattleContext + BATTLE_LEVEL_UP_SECONDARY_OBJECT_OFFSET))
                  ->unk_0f0 = 0x7FFF;

    object = *(BattleSceneObject **)(
        gBattleContext + BATTLE_LEVEL_UP_TERTIARY_OBJECT_OFFSET);
    BattleEntity_BindResource(object->actor_id, 54);
    object = *(BattleSceneObject **)(
        gBattleContext + BATTLE_LEVEL_UP_TERTIARY_OBJECT_OFFSET);
    BattleSceneObject_SetAnimation(object, 4, -1);
    (*(BattleSceneObject **)(
         gBattleContext + BATTLE_LEVEL_UP_TERTIARY_OBJECT_OFFSET))
        ->primary_model->animation_state_bits.state = 31;
    *(u16 *)&(*(BattleSceneObject **)(
                  gBattleContext + BATTLE_LEVEL_UP_TERTIARY_OBJECT_OFFSET))
                  ->unk_0f0 = 0x7FFF;

    BattleEntity_BindResource(40, 62);
    BattleSceneObject_SetAnimation(BattleSceneObject_GetById(40), -1, -1);
    *(u16 *)&BattleSceneObject_GetById(40)->unk_0f0 = 0x7FFF;

    BattleEntity_BindResource(41, 54);
    BattleSceneObject_SetAnimation(BattleSceneObject_GetById(41), -1, -1);
    *(u16 *)&BattleSceneObject_GetById(41)->unk_0f0 = 0x7FFF;

    BattleEntity_BindResource(42, 54);
    BattleSceneObject_SetAnimation(BattleSceneObject_GetById(42), -1, -1);
    *(u16 *)&BattleSceneObject_GetById(42)->unk_0f0 = 0x7FFF;

    entrance_state->flags &= ~0x7F;
    entrance_state->x = 4480;
    entrance_state->y = 1024;
    entrance_state->flags &= ~0x80;
    *(s16 *)&controller->state = -1;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_SCREEN_X_OFFSET) = 48;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_SCREEN_TARGET_X_OFFSET) =
        entrance_state->x / 16;
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_SCREEN_TARGET_Y_OFFSET) =
        entrance_state->y / 16 - 28 +
        *(s16 *)(gBattleContext + BATTLE_LEVEL_UP_SCREEN_X_OFFSET);
    *(u16 *)(gBattleContext + BATTLE_LEVEL_UP_SCREEN_STATE_OFFSET) = 0;
    return entrance;
}
