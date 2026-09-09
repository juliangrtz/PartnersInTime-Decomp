#ifndef PIT_GAME_BATTLE_FRAME_H
#define PIT_GAME_BATTLE_FRAME_H
#include <game/battle_actor.h>
#include <nitro/rtc.h>
#include <game/battle_scene.h>
#include <game/battle_ai.h>
#include <game/sprite_animation.h>
#include <game/matrix_animation.h>

/* Shared battle allocation including the palette buffers used by the frame loop.
 * Interface storage stays opaque here; each UI subsystem owns its detailed view. */
typedef struct BattleFrameContextView {
    u8 unknown_00000[4];
    void (*update)(void);
    void (*vblank)(void);
    u8 unknown_0000c[8];
    u32 frame;
    int turn_state;
    int paused_turn_state;
    u8 unknown_00020[2];
    /* Brightness level 0..32 maps to hardware -16..0. */
    s16 main_brightness_level, sub_brightness_level;
    u8 unknown_00026[2];
    s16 render_values_28[4];
    s16 render_values_30[4];
    u8 unknown_00038[6];
    u16 initial_state_3e;
    u8 unknown_00040[16];
    RtcTime clock;
    u8 unknown_0005c[8];
    u32 texture_banks[4];
    u32 palette_banks[4];
    u8 unknown_00084[0x96];
    s16 selected_command;
    u8 unknown_0011c[0xc];
    u16 command_state_128;
    u8 unknown_0012a[0x486];
    BattleSceneObject scene[70];
    u8 unknown_04cc8[0x330];
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
    u8 unknown_0677c[24];
    BattleModel *resource_models[72];
    u8 unknown_068b4[20];
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
    s8 parallax_scale[8];
    s8 scroll_step[8];
    s16 scroll_position[8];
    s16 image_effect_frames;
    s16 image_effect_period;
    u16 *palette[2];
    u16 *sub_palette_tail;
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
BattleFrameContextView *BattleMain_Create(u32 heap_start);
BattleFrameContextView *BattleMain_InitTask(BattleFrameContextView *battle, int priority);
void *BattleMain_Destroy(void *task);
void BattleMain_VBlank(void);
int BattleMain_UploadTextures(void *task);
#ifdef __cplusplus
}
#endif
#endif
