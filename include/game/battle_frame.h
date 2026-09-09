#ifndef PIT_GAME_BATTLE_FRAME_H
#define PIT_GAME_BATTLE_FRAME_H
#include <game/battle_actor.h>
#include <game/battle_scene.h>
#include <game/battle_ai.h>
#include <game/sprite_animation.h>
#include <game/matrix_animation.h>

/* Shared battle allocation through the palette buffers used by the frame loop.
 * Interface storage stays opaque here; each UI subsystem owns its detailed view. */
typedef struct BattleFrameContextView {
    u8 unknown_00000[4];
    void (*update)(void);
    u8 unknown_00008[12];
    u32 frame;
    int turn_state;
    int paused_turn_state;
    u8 unknown_00020[0x8];
    s16 render_values_28[4];
    u8 unknown_00030[0xea];
    s16 selected_command;
    u8 unknown_0011c[0xc];
    u16 command_state_128;
    u8 unknown_0012a[0x4ece];
    BattlePartyActor *party[4];
    u8 unknown_05008[0x1520];
    BattleSceneObject *camera[2];
    u8 command_wheel[56];
    u8 target_cursor[16];
    u8 hp_members[4][28];
    u8 hp_panel[84];
    u8 command_menu[84];
    u8 target_label[164];
    u8 target_marker[16];
    u8 party_indicators[2][12];
    u8 results[32];
    u8 unknown_0677c[0x14c];
    BattleModel *party_animation_model;
    void *party_animation_data;
    u8 unknown_068d0[0x2274];
    BattleTaskPool task_list_8b44;
    u8 unknown_08b4c[0x700];
    BattleTaskPool task_list_924c;
    u8 unknown_09254[0xe0];
    BattleTaskPool task_list_9334;
    u8 unknown_0933c[0x120];
    BattleTaskPool task_list_945c;
    u8 unknown_09464[0x2100];
    BattleTaskPool task_list_b564;
    u8 unknown_0b56c[0x1200];
    BattleTaskPool task_list_c76c;
    u8 unknown_0c774[0x428];
    s16 view_position[4];
    s16 camera_offset[2];
    u8 unknown_0cba8[0x8];
    s8 scroll_step[8];
    s16 scroll_position[8];
    s16 image_effect_frames;
    s16 image_effect_period;
    u16 *palette[2];
    u8 unknown_0cbd4[0x4];
    const s16 *palette_commands[2];
    u8 unknown_0cbe0[0x10];
    GameSpriteAnimation *sprite_animation;
    GameMatrixAnimation *matrix_animation;
    u8 unknown_0cbf8[0x54c10];
    u16 main_palette[512];
    u16 sub_palette[512];
} BattleFrameContextView;
typedef char BattleFrameContextViewSizeCheck[sizeof(BattleFrameContextView) == 401416 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void BattleMain_Update(void);
#ifdef __cplusplus
}
#endif
#endif
