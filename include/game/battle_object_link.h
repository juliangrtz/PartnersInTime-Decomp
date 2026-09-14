#ifndef PIT_GAME_BATTLE_OBJECT_LINK_H
#define PIT_GAME_BATTLE_OBJECT_LINK_H
#include <game/battle_scene.h>

/* Link state lives in the render-override arena. The shared prefix is followed
 * by 12-byte segments; the two initializers reserve 252 or 508 bytes. Only
 * the larger trail supports the parameter at offset 504. */
typedef int (*BattleObjectLinkDraw)(BattleSceneObject *, int);
typedef struct BattleObjectLinkSegment {
    s16 x, y, z, velocity_x, velocity_y, velocity_z;
} BattleObjectLinkSegment;
typedef struct BattleObjectLinkState {
    u16 target;
    s16 style, count, step, gravity_q8, unknown_0a, unknown_0c, unknown_0e;
    s16 source_offset_x, source_offset_y, source_offset_z, target_offset_x, target_offset_y,
        target_offset_z;
    s32 start_x, start_y, start_z, end_x, end_y, end_z;
    struct {
        u32 follow_source : 1, follow_target : 1, unknown : 30;
    } flags;
    BattleObjectLinkSegment segments[1];
} BattleObjectLinkState;
typedef struct BattleObjectLinkOverride {
    BattleObjectLinkDraw draw;
    BattleObjectLinkState *state;
} BattleObjectLinkOverride;
typedef struct BattleObjectLinkTrailView {
    u8 prefix[504];
    s16 unknown_1f8;
    u16 unknown_1fa;
} BattleObjectLinkTrailView;
#define OBJECT_LINK_STATE(object) (((BattleObjectLinkOverride *)(object)->render_override)->state)

typedef struct BattleFlexibleLinkState {
    BattleObjectLinkSegment segments[16];
    u16 unknown_c0, target;
    s16 unknown_c4, unknown_c6, count, unknown_ca, unknown_cc, unknown_ce;
    s32 unknown_d0, velocity;
    u8 unknown_d8[44];
    struct {
        u32 flag_00 : 1, flag_01 : 1, unknown : 30;
    } flags;
    u16 actor, unknown_10a;
} BattleFlexibleLinkState;
typedef struct BattleFlexibleLinkOverride {
    BattleObjectLinkDraw draw;
    BattleFlexibleLinkState *state;
} BattleFlexibleLinkOverride;
#define FLEXIBLE_LINK_STATE(object)                                                                \
    (((BattleFlexibleLinkOverride *)(object)->render_override)->state)

typedef char BattleObjectLinkSegment_SizeCheck[sizeof(BattleObjectLinkSegment) == 12 ? 1 : -1];
typedef char BattleObjectLinkState_SizeCheck[sizeof(BattleObjectLinkState) == 68 ? 1 : -1];
typedef char BattleObjectLinkTrailView_SizeCheck[sizeof(BattleObjectLinkTrailView) == 508 ? 1 : -1];
typedef char BattleFlexibleLinkState_SizeCheck[sizeof(BattleFlexibleLinkState) == 268 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void BattleObjectLink_SetParameter0E(BattleSceneObject *object, int value);
void BattleObjectLink_SetParameter0C(BattleSceneObject *object, int value);
void BattleObjectLink_SetGroundParameter(BattleSceneObject *object, int value);
void BattleObjectLink_SetParameter0A(BattleSceneObject *object, int value);
void BattleObjectLink_SetGravity(BattleSceneObject *object, int value);
void BattleObjectLink_SetTargetOffset(BattleSceneObject *object, int x, int y, int z);
void BattleObjectLink_SetSourceOffsetZ(BattleSceneObject *object, int value);
void BattleObjectLink_SetSourceOffsetY(BattleSceneObject *object, int value);
void BattleObjectLink_SetSourceOffsetX(BattleSceneObject *object, int value);
BattleObjectLinkState *BattleObjectLink_SetSourceOffset(BattleSceneObject *object, int x, int y, int z);
void BattleObjectLink_SetFollowing(BattleSceneObject *object, int first, int second);
BattleObjectLinkDraw BattleObjectLink_InitializeTrail(BattleSceneObject *object,
                                                      BattleSceneObject *other, int spacing,
                                                      int length, int style);
void BattleObjectLink_FollowSource(BattleSceneObject *object, int unused);
BattleObjectLinkDraw BattleObjectLink_InitializeSegments(BattleSceneObject *object,
                                                         BattleSceneObject *other, int spacing,
                                                         int length, int style, int x, int y,
                                                         int value);
void BattleFlexibleLink_Reset(BattleSceneObject *object, int unused);
void BattleFlexibleLink_SetActor(BattleSceneObject *object, u16 actor);
void BattleObjectLink_SumSegments(s32 *out, const BattleObjectLinkSegment *segments, int count);
#ifdef __cplusplus
}
#endif
#endif
