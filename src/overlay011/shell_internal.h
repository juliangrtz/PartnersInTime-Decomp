#ifndef PIT_OVERLAY011_SHELL_INTERNAL_H
#define PIT_OVERLAY011_SHELL_INTERNAL_H
extern "C" {
#include <nitro.h>
}
#include <game/battle_actor.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
extern "C" {
#include <game/battle_hit.h>
#include <game/overlay011_attack.h>
}

struct Overlay11ControllerConfig {
    s16 initial_x, initial_y, step_x, step_y, limit_x, limit_y, collision_x;
    s16 unknown_0e, home_x, home_y, home_z, unknown_16;
    int resource, buffer_size;
};
struct Overlay11Controller {
    BattlePartyActor *actor;
    BattleActor *target;
    Overlay11ControllerConfig *config;
    s16 offset_x, offset_y, unknown_10, unknown_12;
    u16 button, load_id, actor_resource_id, saved_resource_id;
    s16 bounds[6];
    u8 state : 5, flag_5 : 1, side : 1, flag_7 : 1;
    u8 formation : 3, unknown_29 : 5;
    u8 unknown_2a[2];
};
struct Overlay11Support {
    BattleSceneObject *object;
    int active, delay, duration, speed_q8;
    s16 button, animation, effect_handle;
    u8 state : 5, side : 1, formation : 2;
    u8 reposition : 1, unknown_1b : 7;
};
struct Overlay11Projectile {
    BattleSceneObject *object;
    u8 unknown_04[22];
    u8 state, flags;
    void *unknown_1c;
    Overlay11Support *support;
    s16 collision_x, previous_collision_x;
    u8 unknown_28[8];
};
struct Overlay11AttachedEffect {
    BattleEffect *effect;
    BattleSceneObject *object;
};
struct Overlay11ParticleConfig {
    s16 unknown[8], height;
};
struct Overlay11AttackWork {
    u8 unknown_00[204];
    int speed_index;
    u8 unknown_d0[52];
    int parameter, parameter2;
    u8 unknown_10c[4];
    struct {
        s16 x, y;
    } home[2];
    s16 home_z[2];
    u8 unknown_11c[20];
    Overlay11Controller controllers[2];
    Overlay11Projectile projectile;
    Overlay11Support support;
    int resource;
    u8 variant : 1, unknown_1d8 : 7;
    u8 unknown_1d9[3];
    Overlay11ControllerConfig *controller_config;
    Overlay11ParticleConfig *particle_config;
    Overlay11AttachedEffect effects[16];
};
typedef char WorkSize[sizeof(Overlay11AttackWork) == 612 ? 1 : -1];
typedef char ControllerSize[sizeof(Overlay11Controller) == 44 ? 1 : -1];
extern "C" {
extern Overlay11AttackWork *data_ov002_020c0710;
extern s16 data_ov011_020c6040[2][32];
extern s16 data_ov011_020c5f7c[][3];
extern s16 data_ov011_020c5f7e[][3];
extern s16 data_ov011_020c5f80[][3];
extern u16 data_ov002_020be704[];
void func_ov002_02076178(int);
void func_0200940c(BattleModel *, int);
int func_02010960(int, int, int);
extern u8 *gBattleContext;
extern u8 data_ov011_020c5f20[];
extern u16 data_ov011_020c5f24[];
extern int data_ov011_020c5fd0[][6], data_ov011_020c5fdc[][6];
extern Overlay11ControllerConfig data_ov011_020c6000[], data_ov011_020c6140[][6];
extern Overlay11ParticleConfig data_ov011_020c5fac[];
extern int data_ov011_020c5f64[], data_ov011_020c5f6c[], data_ov011_020c5f74[];
Overlay11AttackWork *func_ov010_020c2978(BattlePartyActor *, int, int);
int func_ov010_020c13bc();
void func_ov011_020c2ae0();
void func_ov011_020c57cc(BattlePartyActor *);
void func_ov010_020c1d14(BattlePartyActor *, void (*)(BattlePartyActor *));

void func_ov002_02071c44(int);

int Overlay11Effects_AreFinished();
void Overlay11Effects_UpdatePositions();
void Overlay11Effects_SpawnAttached(int effect_id, BattleSceneObject *object);
int Overlay11Attack_GetSpeed(int delta);
int func_ov011_020c2e20(BattlePosition *position, Overlay11Controller *controller,
                        Overlay11Projectile *projectile);
int Overlay11Attack_GetTravelDuration(int start, int speed_q8, int target, int step);
void Overlay11Support_Reset(Overlay11Support *support);
void func_ov011_020c2f38(Overlay11Support *support);
void Overlay11Support_ScheduleIntercept(Overlay11Support *support, int duration, int delay, int reposition,
                                        int animation);
void Overlay11Support_StartIntercept(Overlay11Support *support, int duration, int reposition,
                                     int minimum_speed, int animation);
void Overlay11Support_LaunchAway(Overlay11Support *support, int speed_q8);
void Overlay11Support_UpdateSpeed(Overlay11Support *support);
void func_ov011_020c39fc(Overlay11Support *support, BattlePartyActor *user, int configuration, s16 side);
int Overlay11Projectile_IsReady(Overlay11Projectile *projectile, int wait_for_support);
void Overlay11Controller_Initialize(Overlay11Controller *controller, int actor_id, BattlePartyActor *user,
                                    int load_id, u16 saved_id, Overlay11ControllerConfig *rows, int side);
void Overlay11Attack_Initialize(BattlePartyActor *user, int resource, int configuration);
}
#endif
