extern "C" {
#include <game/battle_actor.h>
#include <game/battle_scene.h>
#include <game/battle_ai.h>
#include <game/equipment.h>
#include <game/save_data.h>
#include <game/matrix_animation.h>
#include <game/sprite_animation.h>
#include <game/heap.h>
#include <game/battle_frame.h>
#include <game/battle_entry.h>
extern u8 *gBattleContext;
extern MtxFx44 data_ov002_020bea54;
void func_ov002_0207707c(void);
int BattleTurnState_Update(int);
void func_ov002_0206a0f0(void *);
void BattlePartyActor_UpdateCommandMovement(BattlePartyActor *);
void BattleCommandWheel_Update(void *, int);
void func_ov002_020ada98(void);
void func_ov002_020afb94(int);
void func_02018ce0(void *, int, int, int);
void func_02036cc0(const MtxFx44 *);
void func_0202cbd4(void *, int, u32);
void BattleHitQueue_Update(void);
void func_ov002_020ad5d0(void);
void func_ov002_02077724(void *);
void BattleTargetCursor_Update(void *);
void BattlePartyHpPanel_UpdateMember(void *);
void BattlePartyHpPanel_Update(void *);
void BattleCommandMenu_Update(void *);
void BattleTargetLabel_Update(void *);
void BattleTargetMarker_Update(void *);
void BattlePartyIndicator_Update(void *);
void BattleResults_UpdateCounters(void *);
void BattleCommandWheel_Draw(void *);
void BattleTargetCursor_Draw(void *);
void BattlePartyHpPanel_Draw(void *);
void func_ov002_02097a74(void *);
void BattleCommandMenu_Draw(void *);
void BattleTargetLabel_Draw(void *);
void BattleTargetOverlay_Draw(void *);
void BattleResults_DrawScreen(void *);

/* Re-read the shared pointers after calls and writes, preserving native aliasing. */
#define BATTLE_FRAME ((BattleFrameContextView *)gBattleContext)
#define SAVED_PARTY ((SavePartyMember *)(gSaveData + 1016))

static inline int SurvivingHp(BattlePartyActor *actor)
{
    int hp = actor->actor.current_hp;
    if (hp <= 0)
        hp = 1;
    return hp;
}

void BattleMain_Update(void)
{
    func_ov002_0207707c();
    if (BATTLE_FRAME->palette_commands[0])
        BattlePalette_Animate(BATTLE_FRAME->palette_commands[0], 256, BATTLE_FRAME->main_palette,
                            &BATTLE_FRAME->palette[0]);
    if (BATTLE_FRAME->palette_commands[1])
        BattlePalette_Animate(BATTLE_FRAME->palette_commands[1], 240, BATTLE_FRAME->sub_palette,
                            &BATTLE_FRAME->palette[1]);
    BattleAI_UpdateAll();
    if (!BATTLE_FRAME->paused_turn_state || BATTLE_FRAME->paused_turn_state != BATTLE_FRAME->turn_state)
        BATTLE_FRAME->turn_state = BattleTurnState_Update(BATTLE_FRAME->turn_state);
    func_ov002_0206a0f0(gBattleContext);
    BattleTaskList_Update(&BATTLE_FRAME->task_list_8b44);
    BattleTaskList_Update(&BATTLE_FRAME->task_list_c76c);
    BATTLE_FRAME->command_state_128 = 0;
    BattlePartyActor_UpdateCommandMovement(BATTLE_FRAME->party[0]);
    BattlePartyActor_UpdateCommandMovement(BATTLE_FRAME->party[1]);
    BattleCommandWheel_Update(BATTLE_FRAME->command_wheel, BATTLE_FRAME->selected_command);
    BattleTargetCursor_Update(BATTLE_FRAME->target_cursor);
    BattlePartyHpPanel_UpdateMember(BATTLE_FRAME->hp_members[0]);
    BattlePartyHpPanel_UpdateMember(BATTLE_FRAME->hp_members[1]);
    BattlePartyHpPanel_UpdateMember(BATTLE_FRAME->hp_members[2]);
    BattlePartyHpPanel_UpdateMember(BATTLE_FRAME->hp_members[3]);
    BattlePartyHpPanel_Update(BATTLE_FRAME->hp_panel);
    BattleCommandMenu_Update(BATTLE_FRAME->command_menu);
    BattleTargetLabel_Update(BATTLE_FRAME->target_label);
    BattleTargetMarker_Update(BATTLE_FRAME->target_marker);
    BattlePartyIndicator_Update(BATTLE_FRAME->party_indicators[0]);
    BattlePartyIndicator_Update(BATTLE_FRAME->party_indicators[1]);
    BattleResults_UpdateCounters(BATTLE_FRAME->results);
    BATTLE_FRAME->render_values_28[1] = 16;
    BATTLE_FRAME->render_values_28[0] = 16;
    BATTLE_FRAME->render_values_28[3] = 1;
    BATTLE_FRAME->render_values_28[2] = 63;
    BATTLE_FRAME->view_position[0] = BATTLE_FRAME->camera[0]->x - BATTLE_FRAME->camera_offset[0];
    BATTLE_FRAME->view_position[1] =
        BATTLE_FRAME->camera[0]->y - BATTLE_FRAME->camera_offset[1] - BATTLE_FRAME->camera[0]->z;
    BATTLE_FRAME->view_position[2] = BATTLE_FRAME->camera[1]->x - BATTLE_FRAME->camera_offset[0];
    BATTLE_FRAME->view_position[3] =
        BATTLE_FRAME->camera[1]->y - BATTLE_FRAME->camera_offset[1] - BATTLE_FRAME->camera[1]->z;
    /* Camera offsets are consumed once, before motion advances to the next frame. */
    BATTLE_FRAME->camera_offset[0] = 0;
    BATTLE_FRAME->camera_offset[1] = 0;
    BattleSceneMotion_UpdateAll();
    BATTLE_FRAME->scroll_position[0] += BATTLE_FRAME->scroll_step[0];
    BATTLE_FRAME->scroll_position[1] += BATTLE_FRAME->scroll_step[1];
    BATTLE_FRAME->scroll_position[2] += BATTLE_FRAME->scroll_step[2];
    BATTLE_FRAME->scroll_position[3] += BATTLE_FRAME->scroll_step[3];
    BATTLE_FRAME->scroll_position[4] += BATTLE_FRAME->scroll_step[4];
    BATTLE_FRAME->scroll_position[5] += BATTLE_FRAME->scroll_step[5];
    BATTLE_FRAME->scroll_position[6] += BATTLE_FRAME->scroll_step[6];
    BATTLE_FRAME->scroll_position[7] += BATTLE_FRAME->scroll_step[7];
    func_ov002_020ada98();
    func_ov002_020afb94(0);
    BattleScene_RenderShadows();
    if (BATTLE_FRAME->image_effect_frames > 0)
        --BATTLE_FRAME->image_effect_frames;
    if (BATTLE_FRAME->image_effect_frames > 0 && BATTLE_FRAME->frame % BATTLE_FRAME->image_effect_period == 0)
        func_02018ce0((void *)0x06898000, 32, 192, 13);
    BattleTaskList_Update(&BATTLE_FRAME->task_list_b564);
    BattleTaskList_Update(&BATTLE_FRAME->task_list_945c);
    BattleTaskList_Update(&BATTLE_FRAME->task_list_9334);
    if (BATTLE_FRAME->sprite_animation) {
        *(vu32 *)0x04000444 = 0;
        *(vu32 *)0x04000440 = 2;
        data_ov002_020bea54._30 = -(BATTLE_FRAME->view_position[0] << 8);
        data_ov002_020bea54._31 = -(BATTLE_FRAME->view_position[1] << 8);
        func_02036cc0(&data_ov002_020bea54);
        GameSpriteAnimation_Update(BATTLE_FRAME->sprite_animation);
        *(vu32 *)0x04000448 = 1;
    }
    if (BATTLE_FRAME->matrix_animation) {
        func_0202cbd4(BATTLE_FRAME->matrix_animation->parameters, 0, 64);
        GameMatrixAnimation_Update(BATTLE_FRAME->matrix_animation);
    }
    BattleCommandWheel_Draw(BATTLE_FRAME->command_wheel);
    BattleTargetCursor_Draw(BATTLE_FRAME->target_cursor);
    BattlePartyHpPanel_Draw(BATTLE_FRAME->hp_members[0]);
    BattlePartyHpPanel_Draw(BATTLE_FRAME->hp_members[1]);
    BattlePartyHpPanel_Draw(BATTLE_FRAME->hp_members[2]);
    BattlePartyHpPanel_Draw(BATTLE_FRAME->hp_members[3]);
    func_ov002_02097a74(BATTLE_FRAME->hp_panel);
    BattleCommandMenu_Draw(BATTLE_FRAME->command_menu);
    BattleTargetLabel_Draw(BATTLE_FRAME->target_label);
    BattleTargetOverlay_Draw(BATTLE_FRAME->target_marker);
    BattleResults_DrawScreen(BATTLE_FRAME->results);
    BattleHitQueue_Update();
    BattleTaskList_Update(&BATTLE_FRAME->task_list_924c);
    BattleScene_RenderModels();
    func_ov002_020ad5d0();
    /* Preserve one HP even for a knocked-out member when returning to the field. */
    if (!BATTLE_FRAME->turn_state) {
        switch (*(s16 *)(gSaveData + 0x558)) {
        case 0:
            SAVED_PARTY[0].current_hp = SurvivingHp(BATTLE_FRAME->party[0]);
            SAVED_PARTY[1].current_hp = SurvivingHp(BATTLE_FRAME->party[1]);
            Equipment_Apply(0, SAVED_PARTY[0].badge_id | 0x3000);
            Equipment_Apply(1, SAVED_PARTY[1].badge_id | 0x3000);
            Equipment_Apply(0, SAVED_PARTY[0].experience_to_next_level.equipment.clothing_id | 0x4000);
            Equipment_Apply(1, SAVED_PARTY[1].experience_to_next_level.equipment.clothing_id | 0x4000);
            break;
        case 1:
            SAVED_PARTY[2].current_hp = SurvivingHp(BATTLE_FRAME->party[0]);
            SAVED_PARTY[3].current_hp = SurvivingHp(BATTLE_FRAME->party[1]);
            Equipment_Apply(2, SAVED_PARTY[2].badge_id | 0x3000);
            Equipment_Apply(3, SAVED_PARTY[3].badge_id | 0x3000);
            Equipment_Apply(2, SAVED_PARTY[2].experience_to_next_level.equipment.clothing_id | 0x4000);
            Equipment_Apply(3, SAVED_PARTY[3].experience_to_next_level.equipment.clothing_id | 0x4000);
            break;
        case 2:
            for (int i = 0; i < 4; ++i) {
                BattlePartyActor *actor = BATTLE_FRAME->party[i];
                switch (actor->formation_index) {
                case 0:
                case 4:
                    SAVED_PARTY[0].current_hp = SurvivingHp(actor);
                    break;
                case 1:
                case 5:
                    SAVED_PARTY[1].current_hp = SurvivingHp(actor);
                    break;
                case 2:
                    SAVED_PARTY[2].current_hp = SurvivingHp(actor);
                    break;
                case 3:
                    SAVED_PARTY[3].current_hp = SurvivingHp(actor);
                    break;
                }
                Equipment_Apply(i, SAVED_PARTY[i].badge_id | 0x3000);
                Equipment_Apply(i, SAVED_PARTY[i].experience_to_next_level.equipment.clothing_id | 0x4000);
            }
            break;
        }
        u8 *allocation = gBattleContext;
        if (allocation) {
            func_ov002_02077724(allocation);
            GameHeap_Delete(allocation);
        }
    }
}
}
