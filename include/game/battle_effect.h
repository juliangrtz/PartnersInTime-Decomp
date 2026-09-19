#ifndef PIT_GAME_BATTLE_EFFECT_H
#define PIT_GAME_BATTLE_EFFECT_H

#include <game/battle_scene.h>
#include <nitro.h>

/* Transient visuals in a battle: hit sparks, numbers, status icons, the
   graphics an attack throws around. Two kinds exist and they are NOT the same
   record even though both are reached through a BattleEffect ** slot - a
   sprite effect is 48 bytes and a model effect 56, so use the allocation the
   factory actually returned.

   Every effect stores the address of the pointer that owns it (owner_slot), so
   when it finishes it can clear the owner's reference itself. That is why an
   effect may be freed without the owner being told. */

typedef struct BattleEffect BattleEffect;
typedef struct BattleModelEffect BattleModelEffect;
typedef struct BattlePosition BattlePosition;
typedef struct BattleSpriteTransform BattleSpriteTransform;

struct BattlePosition {
    s16 x;
    s16 y;
    s16 z;
    u16 padding_06;
};

/* A sprite effect. `complete` is what the owner polls; update_callback runs it
   each frame and completion_callback is the teardown hook. */
struct BattleEffect {
    u8 unknown_00[4];
    void (*update_callback)(BattleEffect *effect);
    u8 unknown_08[6];
    s16 x;
    s16 y;
    s16 z;
    union {
        u8 unknown_14[6];
        struct {
            union {
                u16 sprite_flags;
                struct {
                    u16 unknown_sprite_flags_00_13 : 14;
                    u16 script_flag : 1;
                    u16 unknown_sprite_flag_15 : 1;
                };
            };
            u8 unknown_16[4];
        };
    };
    union {
        u16 flags;
        struct {
            u16 attached_script_flag : 1;
            u16 unknown_flags_01_03 : 3;
            u16 complete : 1;
            u16 unknown_flags_05_15 : 11;
        };
    };
    u8 unknown_1c[4];
    u32 user_value;
    u8 unknown_24[4];
    BattleEffect *(*completion_callback)(BattleEffect *effect);
    BattleEffect **owner_slot;
};

/* A model effect. Positioned relative to `parent`, with its scale in Q4. */
struct BattleModelEffect {
    u8 unknown_00[0x12];
    s16 scale_q4;
    s16 x;
    s16 y;
    s16 z;
    u8 unknown_1a[2];
    u32 user_value;
    u8 unknown_20[0x0C];
    BattleSceneObject *parent;
    u8 unknown_30[4];
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
typedef char BattleModelEffect_SizeCheck[
    sizeof(BattleModelEffect) == 0x38 ? 1 : -1];
typedef char BattleSpriteTransform_SizeCheck[
    sizeof(BattleSpriteTransform) == 0x40 ? 1 : -1
];

#ifdef __cplusplus
extern "C" {
#endif

void BattlePosition_StoreViewRelative(BattlePosition *position,
                                      int x, int y, int z,
                                      int use_raw_position, int view_index);
void *BattleText_GetEntry(int archive_id, u16 resource_id);
void BattlePosition_StoreBattleRelative(BattlePosition *position,
                                        int x, s16 anchor_y,
                                        s16 offset_y, s16 offset_z,
                                        int use_raw_position, int view_index);
void BattleTransform_BuildBetweenPoints(
    BattleSpriteTransform *transform,
    s16 start_x, s16 start_y, s16 start_z,
    s16 end_x, s16 end_y, s16 end_z,
    u16 longitudinal_scale, u16 lateral_scale);
BattleSceneObject *BattleSceneObject_StretchBetweenAnchors(
    int object_id,
    int first_object_id,
    s16 first_offset_x, s16 first_offset_y, int first_offset_z,
    int first_anchor_z,
    s16 second_object_id,
    s16 second_offset_x, int second_offset_y, int second_offset_z,
    int second_anchor_z,
    u16 longitudinal_scale, u16 lateral_scale,
    u16 flatten);
/* Optional reference object supplies the initial view-relative position.
 * Offsets use full ABI words and narrow to signed halfwords at the spawn. */
BattleEffect *BattleSpriteEffect_SpawnRelative(
    int effect_id, BattleSceneObject *reference,
    int offset_x, int offset_y, int offset_z, int scale);
BattleEffect *BattleModelEffect_SpawnRelative(
    int effect_id, BattleSceneObject *reference, BattleSceneObject *parent,
    int offset_x, int offset_y, int offset_z, int scale);
BattleEffect *BattleSpriteEffect_Spawn(int effect_id, int x, int y, int z,
                                       int scale);
BattleEffect *BattleSpriteEffect_SpawnAttached(BattleEffect **owner_slot,
                                               int effect_id,
                                               int x, int y, s16 z,
                                               int scale);
int BattleSpriteEffect_SpawnInFreeSlot(int effect_id,
                                       int x, int y, int z, int scale);
BattleEffect *BattleModelEffect_Spawn(int effect_id,
                                      BattleSceneObject *parent,
                                      s16 x, s16 y, s16 z, int scale);
BattleEffect *BattleModelEffect_SpawnAttached(BattleEffect **owner_slot,
                                              int effect_id,
                                              BattleSceneObject *parent,
                                              s16 x, s16 y, s16 z, int scale);
int BattleModelEffect_SpawnInFreeSlot(int effect_id,
                                      BattleSceneObject *parent,
                                      s16 x, s16 y, s16 z, int scale);
struct BattleAITask *BattleModelEffect_SpawnFromResource(
    int resource_id, int animation_id, int x, int y, s16 z, int scale);
struct BattleAITask *BattleModelEffect_SpawnFromResourceAttached(
    struct BattleAITask **owner_slot, int resource_id, int animation_id,
    int x, s16 y, s16 z, int scale);
int BattleModelEffect_SpawnFromResourceInFreeSlot(
    int resource_id, int animation_id, int x, s16 y, s16 z, int scale);
/* These tasks own a palette record and a 304-byte alternate renderer. */
struct BattleAITask *BattleAlternateModelEffect_Spawn(
    int resource_id, int animation_id, int x, int y, s16 z, int scale);
struct BattleAITask *BattleAlternateModelEffect_SpawnAttached(
    struct BattleAITask **owner_slot, int resource_id, int animation_id,
    int x, s16 y, s16 z, int scale);
int BattleAlternateModelEffect_SpawnInFreeSlot(int resource, int animation, int x, s16 y, s16 z, int scale);
int BattleParty_SpawnLaunchImpact(struct BattleActor *actor);
void BattleObjectEffect_SpawnDelayed(int animation_id, int delay,
                                     BattleSceneObject *object);
BattleEffect *BattleDamage_SpawnNumber(int value, int x, int y,
                                       int effect_id, u16 attached_actor_id);
int BattleSound_Play(int sound_id, int argument_1, int argument_2,
                     int argument_3);
int BattleScreenEffect_StartPrimary(int kind, int duration, int delay);
int BattleScreenEffect_StartSecondary(int kind, int duration, int delay);
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
