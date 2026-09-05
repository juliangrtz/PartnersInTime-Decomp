#ifndef PARTNERS_IN_TIME_GAME_OVERLAY011_ATTACK_H
#define PARTNERS_IN_TIME_GAME_OVERLAY011_ATTACK_H

#include <game/battle_scene.h>

typedef union Overlay11ProjectileFlags {
    u8 raw;
    struct {
        u8 unknown_0_1 : 2;
        u8 configuration : 2;
        u8 unknown_4_7 : 4;
    } bits;
} Overlay11ProjectileFlags;

typedef union Overlay11ControllerFlags {
    u8 raw;
    struct {
        u8 state : 5;
        u8 unknown_5_7 : 3;
    } bits;
} Overlay11ControllerFlags;

typedef struct Overlay11AttackAnimationState {
    u8 unknown_00[0x16];
    u8 animation_frame;
} Overlay11AttackAnimationState;

typedef struct Overlay11AttackProjectile {
    BattleSceneObject *object;
    int speed_q8;
    Overlay11AttackAnimationState *animation_state;
    s16 animation_scale;
    u8 unknown_0e[8];
    u16 object_data_id;
    u16 configuration_parameter;
    u8 unknown_1a;
    Overlay11ProjectileFlags flags;
} Overlay11AttackProjectile;

typedef struct Overlay11AttackMotionBounds {
    u8 unknown_00[4];
    s16 step_x;
    s16 step_y;
    s16 limit_x;
    s16 limit_y;
    s16 collision_offset_x;
} Overlay11AttackMotionBounds;

typedef struct Overlay11AttackController {
    Overlay11AttackProjectile *projectile;
    u8 unknown_04[4];
    Overlay11AttackMotionBounds *motion_bounds;
    s16 offset_x;
    s16 offset_y;
    u8 unknown_10[8];
    u16 actor_resource_id;
    u8 unknown_1a[0xE];
    Overlay11ControllerFlags flags;
    u8 unknown_29[3];
} Overlay11AttackController;

typedef char Overlay11AttackProjectile_SizeCheck[
    sizeof(Overlay11AttackProjectile) == 0x1C ? 1 : -1];
typedef char Overlay11AttackController_SizeCheck[
    sizeof(Overlay11AttackController) == 0x2C ? 1 : -1];

void Overlay11Attack_InitializeProjectile(Overlay11AttackProjectile *state,
                                          int unused, int configuration);
int Overlay11Attack_GetAnimationDuration(Overlay11AttackProjectile *state);
void Overlay11Attack_BeginFinishAnimation(Overlay11AttackController *state);
int Overlay11Attack_TestProjectileContact(Overlay11AttackController *state);
void Overlay11Attack_UpdateClampedOffset(Overlay11AttackController *state);
int Overlay11Attack_IsIdle(Overlay11AttackController *state);
void Overlay11Attack_RestoreActorResource(Overlay11AttackController *state);

#endif
