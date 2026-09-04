#ifndef PIT_GAME_BATTLE_EFFECT_H
#define PIT_GAME_BATTLE_EFFECT_H

#include <game/battle_scene.h>
#include <nitro.h>

typedef struct BattleEffect BattleEffect;
typedef struct BattlePosition BattlePosition;
typedef struct BattleSpriteTransform BattleSpriteTransform;

struct BattlePosition {
    s16 x;
    s16 y;
    s16 z;
    u16 padding_06;
};

struct BattleEffect {
    u8 unknown_00[4];
    void (*update_callback)(BattleEffect *effect);
    u8 unknown_08[6];
    s16 x;
    s16 y;
    s16 z;
    u8 unknown_14[0x14];
    BattleEffect *(*completion_callback)(BattleEffect *effect);
    BattleEffect **owner_slot;
};

struct BattleSpriteTransform {
    s32 matrix[12];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_3c[4];
};

typedef char BattlePosition_SizeCheck[
    sizeof(BattlePosition) == 8 ? 1 : -1
];
typedef char BattleEffect_SizeCheck[sizeof(BattleEffect) == 0x30 ? 1 : -1];
typedef char BattleSpriteTransform_SizeCheck[
    sizeof(BattleSpriteTransform) == 0x40 ? 1 : -1
];

#ifdef __cplusplus
extern "C" {
#endif

void BattlePosition_StoreViewRelative(BattlePosition *position,
                                      int x, int y, int z,
                                      int use_raw_position, int view_index);
void BattlePosition_StoreBattleRelative(BattlePosition *position,
                                        int x, s16 anchor_y,
                                        s16 offset_y, s16 offset_z,
                                        int use_raw_position, int view_index);
BattleEffect *BattleSpriteEffect_Spawn(int effect_id, int x, int y, int z,
                                       int scale);
BattleEffect *BattleModelEffect_Spawn(int effect_id,
                                      BattleSceneObject *parent,
                                      int x, int y, int z, int scale);
BattleEffect *BattleModelEffect_SpawnAttached(BattleEffect **owner_slot,
                                              int effect_id,
                                              BattleSceneObject *parent,
                                              int x, int y, int z, int scale);
BattleEffect *BattleDamage_SpawnNumber(int value, int x, int y,
                                       int effect_id, int attach_to_actor);
int BattleSound_Play(int sound_id, int argument_1, int argument_2,
                     int argument_3);
int BattleNumber_DrawDecimal(int value, int palette,
                             BattleSpriteTransform *transform, int object,
                             u16 render_flags, int first_digit, int spacing);
void *BattleSprite_DrawFrame(int frame, int palette,
                             BattleSpriteTransform *transform, int object,
                             u16 z, u16 resource_id, u16 render_flags);
int BattleRender_UpdateIntensity(int direction, s16 *intensity);
void BattleReward_ClearCounterEffects(void);
BattleEffect *BattleReward_AdvanceCounterEffect(BattleEffect *effect);
BattleEffect *BattleReward_EnsureCounterEffect(int reward_type);

#ifdef __cplusplus
}
#endif

#endif
