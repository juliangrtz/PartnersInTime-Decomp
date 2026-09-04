#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>
#include <game/save_data.h>

enum BattleEffectSequenceOffset {
    BATTLE_EFFECT_SEQUENCE_ACTIVE_OFFSET = 0xE4,
    BATTLE_EFFECT_SEQUENCE_TIMER_OFFSET = 0xFC,
    BATTLE_EFFECT_SEQUENCE_Y_OFFSET = 0xFE,
    BATTLE_EFFECT_SEQUENCE_INPUT_OFFSET = 0x104,
    BATTLE_EFFECT_SEQUENCE_OBJECT_OFFSET = 0x6784,
    SAVE_PARTY_MEMBERS_OFFSET = 0x3F8,
    SAVE_PARTY_MEMBER_SIZE = 0x24
};

typedef struct BattleLevelUpBonusApplyState {
    u8 unknown_00[8];
    s16 settle_delay;
    union {
        u8 stat_flags;
        struct {
            u8 selected_stat : 7;
            u8 unknown_stat_flag : 1;
        };
    };
    u8 remaining_ticks;
    u8 total_bonus;
    u8 display_x;
    u8 display_y;
} BattleLevelUpBonusApplyState;

typedef struct BattleEffectObjectMotion {
    u8 unknown_00[4];
    void *update_callback;
    u8 unknown_08[4];
    s16 x;
    s16 scaled_z;
    u8 unknown_10[9];
    u8 keep_active;
} BattleEffectObjectMotion;

extern const s16 gBattleLevelUpBonusPhaseOrder[8];
extern const s16 gBattleLevelUpBonusPhaseOffsets[8];
extern BattleSpriteTransform gBattleLevelUpBonusApplyTransform;

s32 _s32_div_f(s32 numerator, s32 denominator);

void BattleLevelUpBonus_UpdateAppliedStats(BattleAITask *task) {
    BattleLevelUpBonusApplyState *state =
        (BattleLevelUpBonusApplyState *)task->state;
    u8 *task_tail = (u8 *)task + 0xC;
    s32 target_x =
        *(s16 *)(gBattleContext + BATTLE_EFFECT_SEQUENCE_TIMER_OFFSET);
    u32 current_x = state->display_x;

    if ((s32)current_x < target_x && current_x < 220) {
        state->display_x = target_x;
        if (state->display_x > 224) {
            state->display_x = 224;
        }
    }
    if (state->display_y <
        *(s16 *)(gBattleContext + BATTLE_EFFECT_SEQUENCE_Y_OFFSET) + 2) {
        state->display_y =
            *(s16 *)(gBattleContext + BATTLE_EFFECT_SEQUENCE_Y_OFFSET) + 2;
    }

    if (*(s32 *)(task_tail + 4) > 0) {
        if (*(u16 *)(gBattleContext + BATTLE_EFFECT_SEQUENCE_INPUT_OFFSET) !=
            0) {
            *(s32 *)(task_tail + 4) = 0;
        } else {
            --*(s32 *)(task_tail + 4);
        }
    }

    {
        s32 finish_delay = *(s32 *)(task_tail + 4);
        u8 ticks = state->remaining_ticks;

        if (finish_delay == 0) {
            if (ticks != 0) {
                --state->remaining_ticks;
                if (ticks % 8 == 0) {
                    SavePartyMember *member;

                    BattleSound_Play(10, 0, 0, 0);
                    member = (SavePartyMember *)(
                        gSaveData + SAVE_PARTY_MEMBERS_OFFSET +
                        SAVE_PARTY_MEMBER_SIZE *
                            *(s16 *)(gBattleContext + 0xDA));
                    switch (state->selected_stat) {
                    case 0:
                        ++member->base_max_hp;
                        break;
                    case 1:
                        ++member->base_power;
                        break;
                    case 2:
                        ++member->base_defense;
                        break;
                    case 3:
                        ++member->base_speed;
                        break;
                    case 4:
                        ++member->base_stache;
                        break;
                    }
                }
            } else {
                if (state->settle_delay > 0) {
                    --state->settle_delay;
                }
                if (*(u16 *)(
                        gBattleContext + BATTLE_EFFECT_SEQUENCE_INPUT_OFFSET) !=
                        0 ||
                    state->settle_delay <= 0) {
                    SavePartyMember *member =
                        (SavePartyMember *)(
                            gSaveData + SAVE_PARTY_MEMBERS_OFFSET +
                            SAVE_PARTY_MEMBER_SIZE *
                                *(s16 *)(gBattleContext + 0xDA));

                    member->max_hp = member->base_max_hp;
                    member->power = member->base_power;
                    member->defense = member->base_defense;
                    member->speed = member->base_speed;
                    member->stache = member->base_stache;
                    state->display_x = 0;
                    task->callback = 0;
                    return;
                }
            }
        }
    }

    gBattleLevelUpBonusApplyTransform.x = state->display_x << 8;
    gBattleLevelUpBonusApplyTransform.y = state->display_y << 8;
    gBattleLevelUpBonusApplyTransform.scale = 32;
    BattleNumber_DrawDecimal(state->total_bonus, 31,
                             &gBattleLevelUpBonusApplyTransform, 0, 0x7FFF,
                             0, 0);
    gBattleLevelUpBonusApplyTransform.x = (state->display_x - 16) << 8;
    BattleSprite_DrawFrame(18, 31, &gBattleLevelUpBonusApplyTransform, 0, 0, 3,
                           0x7FFF);
}

void BattleLevelUpBonus_UpdateRisingObject(BattleEffectObjectMotion *motion) {
    s16 *position = &motion->x;

    if (*(s16 *)(gBattleContext + BATTLE_EFFECT_SEQUENCE_TIMER_OFFSET) < 300) {
        BattleSceneObject *object;

        position[0] += 3;
        object = *(BattleSceneObject **)(
            gBattleContext + BATTLE_EFFECT_SEQUENCE_OBJECT_OFFSET
        );
        BattleSceneObject_AdjustPosition(
            object, position[0] - object->x, 253 - object->y,
            253 - position[1] / 16 - object->z
        );
    } else if (*((u8 *)position + 0xD) == 0) {
        motion->update_callback = 0;
        *(u16 *)(gBattleContext + BATTLE_EFFECT_SEQUENCE_ACTIVE_OFFSET) = 0;
    }
}

int BattleLevelUpBonus_MapPhaseToValue(s8 phase, int offset) {
    s16 ordered_phase = gBattleLevelUpBonusPhaseOrder[phase & 7];
    s16 phase_from_end;
    int result =
        (offset +
         _s32_div_f(0x500 * gBattleLevelUpBonusPhaseOffsets[ordered_phase],
                    gBattleLevelUpBonusPhaseOffsets[7] -
                        gBattleLevelUpBonusPhaseOffsets[0])) /
        0x100;

    phase_from_end = ordered_phase - 6;

    if (result < 1) {
        result = 1;
    }
    if (result > 6) {
        result = 6;
    }
    if ((u16)phase_from_end <= 1) {
        if (result < 2) {
            result = 2;
        }
    } else if ((u16)ordered_phase <= 1 && result > 5) {
        result = 5;
    }
    return result;
}
