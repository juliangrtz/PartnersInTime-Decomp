#include <game/battle_effect.h>
#include <game/save_data.h>

enum BattleResultsDisplayConstant {
    BATTLE_RESULTS_ROW_RESOURCE_ID = 0x34,
    BATTLE_RESULTS_EXPERIENCE_OFFSET = 24,
    BATTLE_RESULTS_NEXT_LEVEL_OFFSET = 256,
    BATTLE_RESULTS_NEXT_LEVEL_Y_OFFSET = 8,
    BATTLE_RESULTS_COUNTER_VALUE_OFFSET = 0x1C
};

extern BattleSpriteTransform gBattleResultsRowTransform;

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
