#ifndef PIT_OVERLAY014_FLOWER_INTERNAL_H
#define PIT_OVERLAY014_FLOWER_INTERNAL_H
#include "../overlay010/attack_work.h"
#include <game/overlay014_projectile.h>
extern "C" {
#include <game/overlay014_attack.h>
#include <game/battle_object.h>
#include <game/battle_enemy_data.h>
#include <game/random.h>
}
struct Overlay14Work {
    Overlay10Work common;
    Overlay14Participant participants[2];
    Overlay14Projectile projectiles[32];
    Overlay14AttackObjectState attack;
    int timer, delay;
    Overlay14Participant *active, *next;
    u8 unknown_2324[8];
    BattleSceneObject effects[6][3];
    s16 bounds[6][2];
    u8 variant, enemy_trait[6], unknown_3593[6], rating, feedback, unknown_359b;
};
struct Overlay14ParticipantFade {
    u8 prefix[28];
    int (*render)(BattleSceneObject *, int);
    u8 unknown20[8];
    s16 frame;
    u8 stop : 1, unknown2a : 7;
    u8 pad;
};
struct Overlay14Fade {
    int (*render)(BattleSceneObject *, int);
    u8 unknown_04[8];
    s16 frame;
    u8 stop : 1, unknown_0e : 7;
    u8 unknown_0f;
};
typedef char Overlay14WorkSize[sizeof(Overlay14Work) == 13724 ? 1 : -1];
extern "C" {
extern Overlay10Work *data_ov002_020c0710;
int Overlay10Enemy_IsSelectable(u16);
Overlay10Work *func_ov010_020c2978(Overlay10ActionActor *,int,int);
void Overlay10Object_GetGraphicBounds(s16 *,BattleSceneObject *);
int Overlay10Party_HasBadgeTwo();
void Overlay10Attack_BeginEntry(Overlay10ActionActor *,void(*)(Overlay10ActionActor *));
void func_ov002_020722ac(Overlay10ActionActor *,void(*)(Overlay10ActionActor *));
extern int data_ov014_020c6124;
extern u8 data_ov014_020c61c8[], data_ov014_020c61c9[], data_ov014_020c61ca[];
extern int data_ov014_020c6198[4];
extern u32 data_ov014_020c6130[], data_ov014_020c6138[], data_ov014_020c6168[];
extern s16 data_ov014_020c614c[][3];
void func_ov002_0206f1b8(BattleModel *);
void BattleFeedback_LoadResource();
int func_ov002_020720fc(Overlay10ActionActor *, BattleEnemyActor *);
void BattleParty_RestoreIdleAnimation(Overlay10ActionActor *, BattleSceneObject *);
int func_ov014_020c2bb4(BattleSceneObject *, int);
void Overlay14Attack_UpdateEntry(Overlay10ActionActor *);
void Overlay14Attack_UpdateSequence(Overlay10ActionActor *);
void func_ov014_020c53d4(Overlay14Participant *, Overlay14Participant *);
void func_ov014_020c4464(Overlay14Projectile *);
void func_ov014_020c4f10(Overlay14Participant *, int);
void func_ov014_020c2d24(u16, int);
extern u16 data_ov014_020c6158[][4];
struct Overlay14ScreenParameter {
    s16 value, pad[2];
};
extern Overlay14ScreenParameter data_ov014_020c61b0[], data_ov014_020c61b2[], data_ov014_020c61b4[];

void Overlay14Attack_ExtendDuration();
int func_ov014_020c2af8();
void Overlay14Participant_EndFade(Overlay14Participant *participant);
void Overlay14Participant_StartFade(Overlay14Participant *participant);
void Overlay14Attack_ReleaseEnemyEffects();
void Overlay14Attack_InitializeEnemyEffects(int resource);
void func_ov014_020c348c(s16 *position, BattleSceneObject *object, int location);
void Overlay14Attack_ScheduleNextParticipant(Overlay14Participant *participant);
int Overlay14Attack_PickTarget(int filter);
int Overlay14Projectiles_AreIdle(Overlay14Projectile *projectiles, int count);
int Overlay14Attack_FindFreeProjectile();
void func_ov014_020c5e2c(Overlay10ActionActor *user, int variant);
void func_ov014_020c60e4(Overlay10ActionActor *user);
void func_ov014_020c60f4(Overlay10ActionActor *user);
void Overlay14Attack_UpdateEntry(Overlay10ActionActor *user);
void Overlay14Attack_UpdateSequence(Overlay10ActionActor *user);
}
#endif
