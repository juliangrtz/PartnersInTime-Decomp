#ifndef PIT_GAME_OVERLAY014_PROJECTILE_H
#define PIT_GAME_OVERLAY014_PROJECTILE_H
#include <game/battle_scene.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <game/battle_actor.h>
#include <game/battle_impact_effect.h>
#ifdef __cplusplus
}
#endif
typedef struct Overlay14AttackFlags {
    u8 unknown0 : 1, stop : 1, unknown2 : 6;
} Overlay14AttackFlags;
typedef struct Overlay14Projectile {
    BattleSceneObject object;
    u8 phase;
    struct {
        u8 flag0 : 1, level : 2, unknown3 : 5;
    } flags;
    u8 unknown106, unknown107;
    s16 damage;
    u16 target_id;
} Overlay14Projectile;
typedef struct Overlay14Participant {
    BattlePartyActor *actor;
    Overlay14Projectile *projectile;
    int timer, particle_timer, unknown10;
    u16 resource;
    u8 phase;
    s8 remaining;
    struct {
        u8 side : 1, flag1 : 1, unknown2 : 6;
    } flags;
    s8 animation_frame;
    u8 unknown1a[18];
} Overlay14Participant;
typedef char Overlay14Projectile_SizeCheck[sizeof(Overlay14Projectile) == 268 ? 1 : -1];
typedef char Overlay14Participant_SizeCheck[sizeof(Overlay14Participant) == 44 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

void Overlay14Projectile_StartBounce(Overlay14Projectile *projectile);
void Overlay14Projectile_BeginHide(Overlay14Projectile *projectile);
void Overlay14Projectile_SelectAnimation(Overlay14Projectile *projectile, int active);
void Overlay14Projectile_SetLevel(Overlay14Projectile *projectile, int level);
void Overlay14Projectile_UpdateStopRequest(Overlay14Projectile *projectile);
void Overlay14Projectile_Reset(Overlay14Projectile *projectile);
void Overlay14Projectile_ReleaseModel(BattleSceneObject *object);
void Overlay14Projectile_Initialize(Overlay14Projectile *projectile, int resource);
void Overlay14Participant_BeginEntry(Overlay14Participant *participant);
void Overlay14Participant_UpdateParticles(Overlay14Participant *participant);
void Overlay14Participant_UpdateParticle(BattleAITask *base);
void Overlay14Participant_BeginRecovery(Overlay14Participant *participant);
void Overlay14Participant_BeginSecondaryThrow(Overlay14Participant *participant);
void Overlay14Participant_BeginPrimaryThrow(Overlay14Participant *participant);
int Overlay14Attack_HasTarget(void);
void Overlay14Participant_PositionProjectile(Overlay14Participant *participant);
void Overlay14Participant_SetProjectileMode(Overlay14Participant *participant, int mode);
int Overlay14Participant_IsIdle(Overlay14Participant *participant);
void Overlay14Participant_RestoreActor(Overlay14Participant *participant);
void Overlay14Participant_Initialize(Overlay14Participant *participant, BattlePartyActor *actor, s16 resource,
                                     s8 side);
#ifdef __cplusplus
}
#endif
#endif
