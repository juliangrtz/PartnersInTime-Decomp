#include <game/battle_ai.h>
#include <game/battle_effect.h>
#include <game/save_data.h>

enum BattleResultsDisplayConstant {
    BATTLE_RESULTS_ROW_RESOURCE_ID = 0x34,
    BATTLE_RESULTS_EXPERIENCE_OFFSET = 24,
    BATTLE_RESULTS_NEXT_LEVEL_OFFSET = 256,
    BATTLE_RESULTS_NEXT_LEVEL_Y_OFFSET = 8,
    BATTLE_RESULTS_COUNTER_VALUE_OFFSET = 0x1C,
    BATTLE_RESULTS_REWARD_OBJECT_OFFSET = 0x675C,
    BATTLE_RESULTS_FADE_OBJECT_OFFSET = 0x677C,
    BATTLE_RESULTS_EXPERIENCE_REWARD_OFFSET = 0x86,
    BATTLE_RESULTS_COIN_REWARD_OFFSET = 0x84,
    BATTLE_RESULTS_PARTY_MEMBERS_OFFSET = 0x3F8,
    BATTLE_RESULTS_PARTY_FORM_OFFSET = 0x558,
    BATTLE_RESULTS_COINS_OFFSET = 0x488,
    BATTLE_RESULTS_ACTIVE_RENDER_FLAGS = 0x7FFF,
    BATTLE_RESULTS_INACTIVE_RENDER_FLAGS = 0x4210
};

typedef struct BattleResultsDisplayState {
    BattleSceneObject *scene_object;
    s16 intensity;
    u16 unknown_006;
    BattleEffect *experience_counters[PARTY_MEMBER_COUNT];
    BattleEffect *coin_counter;
    union {
        u16 raw;
        struct {
            u16 mario : 1;
            u16 luigi : 1;
            u16 baby_mario : 1;
            u16 baby_luigi : 1;
            u16 unknown_04_15 : 12;
        } bits;
    } active_members;
} BattleResultsDisplayState;

extern BattleSpriteTransform gBattleResultsRowTransform;
extern const BattleSpriteTransform gBattleResultsSummaryTransformTemplate;

int BattleResults_DrawPartyExperienceRow(
    int member_frame, BattleEffect *experience_counter,
    SavePartyMember *member, int offset_x, int offset_y,
    BattlePosition *position, int intensity, u16 render_flags
);

#define DRAW_PARTY_EXPERIENCE_ROW(member_id, y, is_active) \
    do { \
        if (is_active) { \
            BattleResults_DrawPartyExperienceRow( \
                member_id, state->experience_counters[member_id], \
                (SavePartyMember *)(gSaveData + \
                    BATTLE_RESULTS_PARTY_MEMBERS_OFFSET + \
                    sizeof(SavePartyMember) * member_id), \
                104, y, &position, intensity, \
                BATTLE_RESULTS_ACTIVE_RENDER_FLAGS); \
        } else { \
            BattleResults_DrawPartyExperienceRow( \
                member_id, state->experience_counters[member_id], \
                (SavePartyMember *)(gSaveData + \
                    BATTLE_RESULTS_PARTY_MEMBERS_OFFSET + \
                    sizeof(SavePartyMember) * member_id), \
                104, y, &position, intensity, \
                BATTLE_RESULTS_INACTIVE_RENDER_FLAGS); \
        } \
    } while (0)

void BattleResults_DrawScreen(BattleResultsDisplayState *state) {
    BattlePosition position;
    BattleSceneObject *scene_object;
    int intensity;

    scene_object = *(BattleSceneObject **)(
        gBattleContext + BATTLE_RESULTS_REWARD_OBJECT_OFFSET);
    scene_object->effect_anchor_z = -4064;
    intensity = state->intensity;
    if (intensity < 1) {
        return;
    }

    state->scene_object->flags.raw &= ~0x20000;
    scene_object = state->scene_object;
    BattlePosition_StoreViewRelative(
        &position, scene_object->x,
        (s16)(scene_object->y - scene_object->z),
        (s16)(scene_object->effect_anchor_z +
              16 * (256 - scene_object->y)),
        scene_object->flags.bits.use_raw_position,
        scene_object->flags.bits.use_alternate_model
    );

    if (*(s16 *)(gSaveData + BATTLE_RESULTS_PARTY_FORM_OFFSET) == 2) {
        DRAW_PARTY_EXPERIENCE_ROW(
            PARTY_MEMBER_BABY_LUIGI, 124,
            state->active_members.bits.baby_luigi);
        DRAW_PARTY_EXPERIENCE_ROW(
            PARTY_MEMBER_BABY_MARIO, 76,
            state->active_members.bits.baby_mario);
        DRAW_PARTY_EXPERIENCE_ROW(
            PARTY_MEMBER_LUIGI, 144,
            state->active_members.bits.luigi);
        DRAW_PARTY_EXPERIENCE_ROW(
            PARTY_MEMBER_MARIO, 96,
            state->active_members.bits.mario);
    } else if (*(s16 *)(gSaveData + BATTLE_RESULTS_PARTY_FORM_OFFSET) == 1) {
        DRAW_PARTY_EXPERIENCE_ROW(
            PARTY_MEMBER_BABY_LUIGI, 125,
            state->active_members.bits.baby_luigi);
        DRAW_PARTY_EXPERIENCE_ROW(
            PARTY_MEMBER_BABY_MARIO, 95,
            state->active_members.bits.baby_mario);
    } else {
        DRAW_PARTY_EXPERIENCE_ROW(
            PARTY_MEMBER_LUIGI, 125,
            state->active_members.bits.luigi);
        DRAW_PARTY_EXPERIENCE_ROW(
            PARTY_MEMBER_MARIO, 95,
            state->active_members.bits.mario);
    }

    {
        BattleSpriteTransform transform =
            gBattleResultsSummaryTransformTemplate;

        transform.scale = position.z;
        if (*(s16 *)(gSaveData + BATTLE_RESULTS_PARTY_FORM_OFFSET) == 2) {
            transform.x = (position.x + 100) << 8;
            transform.y = (position.y + 44) << 8;
            BattleSprite_DrawFrame(
                4, intensity, &transform, 0, 0,
                BATTLE_RESULTS_ROW_RESOURCE_ID,
                BATTLE_RESULTS_ACTIVE_RENDER_FLAGS
            );

            transform.x = (position.x + 128) << 8;
            transform.y = (position.y + 44) << 8;
            BattleNumber_DrawDecimal(
                *(s16 *)(gBattleContext +
                         BATTLE_RESULTS_EXPERIENCE_REWARD_OFFSET),
                intensity, &transform, 0,
                BATTLE_RESULTS_ACTIVE_RENDER_FLAGS, 0, 0
            );
        } else {
            transform.x = (position.x + 100) << 8;
            transform.y = (position.y + 60) << 8;
            BattleSprite_DrawFrame(
                4, intensity, &transform, 0, 0,
                BATTLE_RESULTS_ROW_RESOURCE_ID,
                BATTLE_RESULTS_ACTIVE_RENDER_FLAGS
            );

            transform.x = (position.x + 128) << 8;
            transform.y = (position.y + 60) << 8;
            BattleNumber_DrawDecimal(
                *(s16 *)(gBattleContext +
                         BATTLE_RESULTS_EXPERIENCE_REWARD_OFFSET),
                intensity, &transform, 0,
                BATTLE_RESULTS_ACTIVE_RENDER_FLAGS, 0, 0
            );
        }

        transform.x = (position.x + 196) << 8;
        transform.y = (position.y + 92) << 8;
        BattleSprite_DrawFrame(
            5, intensity, &transform, 0, 0,
            BATTLE_RESULTS_ROW_RESOURCE_ID,
            BATTLE_RESULTS_ACTIVE_RENDER_FLAGS
        );

        transform.x = (position.x + 216) << 8;
        transform.y = (position.y + 76) << 8;
        BattleNumber_DrawDecimal(
            *(s16 *)(gBattleContext + BATTLE_RESULTS_COIN_REWARD_OFFSET),
            intensity, &transform, 0,
            BATTLE_RESULTS_ACTIVE_RENDER_FLAGS, 0, 0
        );

        transform.x = (position.x + 216) << 8;
        transform.y = (position.y + 114) << 8;
        if (state->coin_counter != 0) {
            *(u32 *)((u8 *)state->coin_counter +
                     BATTLE_RESULTS_COUNTER_VALUE_OFFSET) =
                *(u32 *)(gSaveData + BATTLE_RESULTS_COINS_OFFSET);
        } else {
            BattleNumber_DrawDecimal(
                *(u32 *)(gSaveData + BATTLE_RESULTS_COINS_OFFSET),
                intensity, &transform, 0,
                BATTLE_RESULTS_ACTIVE_RENDER_FLAGS, 0, 0
            );
        }
    }

    scene_object = *(BattleSceneObject **)(
        gBattleContext + BATTLE_RESULTS_FADE_OBJECT_OFFSET);
    if (scene_object != 0 && scene_object->primary_model != 0) {
        BattleModel *model = scene_object->primary_model;

        model->animation_state_bits.state = (u16)intensity;
    }
}

#undef DRAW_PARTY_EXPERIENCE_ROW

int BattleResults_DrawPartyExperienceRow(
    int member_frame, BattleEffect *experience_counter,
    SavePartyMember *member, int offset_x, int offset_y,
    BattlePosition *position, int intensity, u16 render_flags
) {
    gBattleResultsRowTransform.x = (position->x + offset_x) << 8;
    gBattleResultsRowTransform.y = (position->y + offset_y) << 8;
    gBattleResultsRowTransform.scale = position->z;
    BattleSprite_DrawFrame(
        member_frame, intensity, &gBattleResultsRowTransform, 0, 0,
        BATTLE_RESULTS_ROW_RESOURCE_ID, render_flags
    );

    gBattleResultsRowTransform.x =
        (position->x + offset_x + BATTLE_RESULTS_EXPERIENCE_OFFSET) << 8;
    gBattleResultsRowTransform.y = (position->y + offset_y) << 8;
    if (render_flags == 0x7FFF && experience_counter != 0) {
        *(u32 *)((u8 *)experience_counter +
                 BATTLE_RESULTS_COUNTER_VALUE_OFFSET) =
            member->experience.packed >> 8;
    } else {
        BattleNumber_DrawDecimal(
            member->experience.packed >> 8, intensity,
            &gBattleResultsRowTransform, 0, render_flags, 0, 0
        );
    }

    gBattleResultsRowTransform.x =
        (position->x + offset_x + BATTLE_RESULTS_NEXT_LEVEL_OFFSET) << 8;
    gBattleResultsRowTransform.y =
        (position->y + offset_y + BATTLE_RESULTS_NEXT_LEVEL_Y_OFFSET) << 8;
    return BattleNumber_DrawDecimal(
        member->experience_to_next_level.fields.value, intensity,
        &gBattleResultsRowTransform, 0, render_flags, 0, 0
    );
}
